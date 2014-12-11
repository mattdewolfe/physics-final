#include "BallComponent.h"
#include "PhysicsComponent.h"
#include "PhysicsManager.h"
#include "MovingPlatformComponent.h"
#include "GraphicsComponent.h"
#include "btBulletDynamicsCommon.h"
#include "Entity.h"

void BallComponent::Update(float DeltaTime)
{
	btVector3 worldPos = m_rigidBody->getWorldTransform().getOrigin();
	Entity::EVector3f pos;
	pos.x = worldPos.getX();
	pos.y = worldPos.getY();
	pos.z = worldPos.getZ();

	m_Owner->SetPosition(pos);

	if(PhysicsManager::GetInstance()->IsDebugOn())
	{
		DebugPrint();
	}
}


void BallComponent::OnContactProcCallback(btManifoldPoint& cp, PhysicsComponent* collider)
{
	Entity* ent = collider->GetOwner();
	if(ent)
	{
		btRigidBody* rigidBody = collider->GetRigidBody();
		float mass = rigidBody->getInvMass();
		// check to see if we collided with a static object
		if(mass < 1.0f)
		{
			m_rigidBody->applyCentralImpulse(btVector3(0,1,0));
		}
		else
		{
			// First attempt to get a moving platform component from collision
			MovingPlatformComponent* test = static_cast<MovingPlatformComponent*>(ent->GetComponent(CT_PLATFORM));
			if (test)
			{
				// If we find one, then we can grab the graphics component to compare colors
				GraphicsComponent* graphic = static_cast<GraphicsComponent*>(ent->GetComponent(CT_GRAPHIC));
				// Store color of hit object
				GraphicColor hitColor = graphic->GetColor();
				// Then grab this graphic and compare colors
				graphic = static_cast<GraphicsComponent*>(m_Owner->GetComponent(CT_GRAPHIC));
				if (graphic && graphic->GetColor() == hitColor)
				{
					m_Owner->PlayerWon();
				}
				else
				{
					m_rigidBody->applyCentralImpulse(btVector3(0,10,0));
				}
			}
		}
	}
}

void BallComponent::OnAddSingleResult(btManifoldPoint& cp,int partId0,int index0,const btCollisionObjectWrapper* collidedObjWrap,int collidedObjPartId,int collidedObjIndex)
{
	const btCollisionObject* colObj = collidedObjWrap->getCollisionObject();
	PhysicsComponent* physComp = static_cast<PhysicsComponent*>(colObj->getUserPointer());
	if(physComp)
	{
		Entity* entOwner = physComp->GetOwner();
	}
}
