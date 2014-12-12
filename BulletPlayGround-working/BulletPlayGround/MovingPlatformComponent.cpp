#include "MovingPlatformComponent.h"
#include "GraphicsComponent.h"

void MovingPlatformComponent::Init(float _maxOffSet, float _moveSpeed)
{
	maxOffSet = _maxOffSet; 
	moveSpeed = _moveSpeed;
	offSet = 0;
	PhysicsComponent::Init(RBST_Box, 0.0f);
}

void MovingPlatformComponent::Update(float deltaTime)
{
	float frame = moveSpeed * deltaTime;
	offSet += frame;
	if ((offSet >= maxOffSet && moveSpeed > 0) || 
		(offSet <= maxOffSet && moveSpeed < 0))
	{
		moveSpeed *= -1;
		maxOffSet *= -1;
	//	m_Owner->ToggleColor();
	}
	m_Owner->SetPosition(Entity::EVector3f(offSet, -15, 0));
	m_Owner->SetBodyPosition();

	//PhysicsComponent::Update(deltaTime);
}

void MovingPlatformComponent::OnAddSingleResult(btManifoldPoint& cp,int partId0,int index0,const btCollisionObjectWrapper* collidedObjWrap,int collidedObjPartId,int collidedObjIndex)
{

}

void MovingPlatformComponent::OnContactProcCallback(btManifoldPoint& cp, PhysicsComponent* collider)
{
	Entity* tempEnt = collider->GetOwner();
	if (tempEnt)
	{
		// If we find one, then we can grab the graphics component to compare colors
		GraphicsComponent* graphic = static_cast<GraphicsComponent*>(tempEnt->GetComponent(CT_GRAPHIC));
		// Store color of hit object
		GraphicColor hitColor = graphic->GetColor();
		// Then grab this graphic and compare colors
		graphic = static_cast<GraphicsComponent*>(m_Owner->GetComponent(CT_GRAPHIC));
		if (graphic && graphic->GetColor() == hitColor)
		{
			tempEnt->PlayerWon();
		}
		else
		{
			
		}
	}
}