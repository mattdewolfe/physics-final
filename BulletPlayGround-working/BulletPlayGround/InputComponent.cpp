#include "InputComponent.h"
#include "PhysicsComponent.h"

void InputComponent::Init()
{

}

// Check input flags, make alterations based on them
void InputComponent::Update(float deltaTime)
{
	float force = 0;
	if (leftWasReleased == true)
	{
		leftWasReleased = false;
		force -= 0.5;
	}
	if (rightWasReleased == true)	
	{
		rightWasReleased = false;
		force += 0.5;
	}
	if (force != 0)
	{
		m_Owner->ApplyImpulseForce(Entity::EVector3f(force, 0, 0));
	}
	if (releaseBall == true && ballReleased == false)
	{
		ballReleased = true;
		m_Owner->SetGravity(Entity::EVector3f(0, -10, 0));
	}
}

void InputComponent::LeftReleased()
{ 
	leftWasReleased = true; 
	std::cout << "Left Released\n"; 
}

void InputComponent::RightReleased()
{ 
	rightWasReleased = true; 
	std::cout << "Right Released\n"; 
}

void InputComponent::SetRelease()
{ 
	releaseBall = true;
	std::cout << "Release Ball\n"; 
}