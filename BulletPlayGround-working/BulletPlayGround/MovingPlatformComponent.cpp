#include "MovingPlatformComponent.h"

void MovingPlatformComponent::Init(float _maxOffSet, float _moveSpeed)
{
	maxOffSet = _maxOffSet; 
	moveSpeed = _moveSpeed;
	offSet = 0;
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
		m_Owner->ClearForces();
		m_Owner->ToggleColor();
	}
	m_Owner->ApplyImpulseForce(Entity::EVector3f(frame, 0, 0));
}