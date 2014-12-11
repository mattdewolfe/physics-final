#include "PhysicsComponent.h"
#include "PhysicsManager.h"
#include "MovingPlatformComponent.h"
#include "GraphicsComponent.h"
#include "btBulletDynamicsCommon.h"
#include "Entity.h"

using namespace std;

void PhysicsComponent::Init(RigidBodyShapeTypes type, float _mass)
{
	Component::Init();
	Entity::EVector3f pos = m_Owner->GetPosition();
	switch (type)
	{
	case PhysicsComponent::RBST_Plane:
		CreatePlane(pos, _mass);
		break;
	case PhysicsComponent::RBST_Sphere:
		CreateSphere(pos, _mass);
		break;
	case PhysicsComponent::RBST_Box:
		CreateBox(pos, _mass);
		break;
	default:
		break;
	}
}

void PhysicsComponent::CreateSphere(Entity::EVector3f _pos, float _mass)
{
	btCollisionShape* sphereShape = new btSphereShape(1.0f);
	
	btVector3 position = btVector3(_pos.x, _pos.y, _pos.z);
	btDefaultMotionState* fallMotionState =
                new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), position));

    btVector3 fallInertia(0, 0, 0);
    sphereShape->calculateLocalInertia(_mass, fallInertia);

	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(_mass, fallMotionState, sphereShape, fallInertia);
    m_rigidBody = new btRigidBody(fallRigidBodyCI);
		
	m_rigidBody->setUserPointer(this);
	//add physicsbody to world
	PhysicsManager::GetInstance()->AddPhysicsComponent(this);
}

void PhysicsComponent::CreateBox(Entity::EVector3f _pos, float _mass)
{
	btCollisionShape* boxShape = new btBoxShape(btVector3(1.0, 1.0, 1.0));

	btVector3 position = btVector3(_pos.x, _pos.y, _pos.z);
	btDefaultMotionState* boxMotionState =
                new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), position));

    btVector3 fallInertia(0, 0, 0);
    boxShape->calculateLocalInertia(_mass, fallInertia);

	btRigidBody::btRigidBodyConstructionInfo
                boxRigidBodyCI(_mass, boxMotionState, boxShape, fallInertia);

    m_rigidBody = new btRigidBody(boxRigidBodyCI);
	m_rigidBody->setUserPointer(this);
	//add physicsbody to world
	PhysicsManager::GetInstance()->AddPhysicsComponent(this);
}

void PhysicsComponent::CreatePlane(Entity::EVector3f _pos, float _mass)
{
	btCollisionShape* planeShape = new btStaticPlaneShape(btVector3(0,1,0),1.0f);
	
	btVector3 position = btVector3(_pos.x, _pos.y, _pos.z);
	btDefaultMotionState* planeMotionState =
                new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), position));
	
	btRigidBody::btRigidBodyConstructionInfo
                planeRigidBodyCI(1, planeMotionState, planeShape, btVector3(0, 0, 0));

    m_rigidBody = new btRigidBody(planeRigidBodyCI);
	m_rigidBody->setUserPointer(this);
	//add physicsbody to world
	PhysicsManager::GetInstance()->AddPhysicsComponent(this);
}

void PhysicsComponent::Shutdown()
{
	//remove rigidBody from physics world
	PhysicsManager::GetInstance()->RemovePhysicsComponent(this);
}

void PhysicsComponent::ApplyForceAtCentre(Entity::EVector3f _force)
{
	m_rigidBody->activate();
	btVector3 tempForce = btVector3(_force.x, _force.y, _force.z);
	m_rigidBody->applyCentralForce(tempForce);
}

void PhysicsComponent::ApplyImpulseForce(Entity::EVector3f _force)
{
	m_rigidBody->activate();
	btVector3 tempForce = btVector3(_force.x, _force.y, _force.z);
	m_rigidBody->applyCentralImpulse(tempForce);
}

void PhysicsComponent::SetGravity(Entity::EVector3f _force)
{
	m_rigidBody->activate();
	btVector3 tempForce = btVector3(_force.x, _force.y, _force.z);
	m_rigidBody->setGravity(tempForce);
}

void PhysicsComponent::Update(float DeltaTime)
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

void PhysicsComponent::OnContactProcCallback(btManifoldPoint& cp, PhysicsComponent* collider)
{
	Entity* ent = collider->GetOwner();
	MovingPlatformComponent* check = static_cast<MovingPlatformComponent*>(m_Owner->GetComponent(CT_PLATFORM));
	if(ent && check == nullptr)
	{
		btRigidBody* rigidBody = collider->GetRigidBody();
		float mass = rigidBody->getInvMass();
		// check to see if we collided with a static object
		if(mass < 0.1f)
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

void PhysicsComponent::OnAddSingleResult(btManifoldPoint& cp,int partId0,int index0,const btCollisionObjectWrapper* collidedObjWrap,int collidedObjPartId,int collidedObjIndex)
{
	const btCollisionObject* colObj = collidedObjWrap->getCollisionObject();
	PhysicsComponent* physComp = static_cast<PhysicsComponent*>(colObj->getUserPointer());
	if(physComp)
	{
		Entity* entOwner = physComp->GetOwner();
		//entOwner->OnHit();
	}
}

void PhysicsComponent::DebugPrint()
{
	if(m_rigidBody)
	{
		btVector3 worldPos = m_rigidBody->getWorldTransform().getOrigin();
		cout << "Rigid Body Position X:" << worldPos.getX() 
			<< " Y:" << worldPos.getY() << " Z:" << worldPos.getZ() << endl; 
	}
	else
	{
		cout << "Rigid body not instatiated" << endl;
	}
}

void PhysicsComponent::RotateBody(float _degrees)
{
	btTransform tr;
	tr.setIdentity();
	btQuaternion quat;
	quat.setEuler(_degrees, 0, 0); //or quat.setEulerZYX depending on the ordering you want
	tr.setRotation(quat);

	m_rigidBody->setCenterOfMassTransform(tr);
}