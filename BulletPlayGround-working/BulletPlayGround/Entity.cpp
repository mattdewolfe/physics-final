#include "Entity.h"
#include "Component.h"
#include "PhysicsComponent.h"
#include "GraphicsComponent.h"
#include "btBulletDynamicsCommon.h"

#include "../BulletCollision/CollisionDispatch/btCollisionWorld.h"

void Entity::SetBodyPosition()
{
	if (m_Components.at(CT_PHYSICS) != nullptr)
	{
		PhysicsComponent* temp = dynamic_cast<PhysicsComponent*>(m_Components[CT_PHYSICS]);
		btVector3 position = btVector3(m_Position.x, m_Position.y, m_Position.z);
		temp->GetRigidBody()->clearForces();
		temp->GetRigidBody()->setAngularVelocity(btVector3(0, 0, 0));
		temp->GetRigidBody()->setWorldTransform(btTransform(btQuaternion(0, 0, 0, 1), position));
	}
}
void Entity::AddComponent(Component* component)
{
	if(component->GetOwner() == nullptr)
		component->SetOwner(this);

	m_Components[component->GetComponentType()] = component;
}

void Entity::Update(float deltaTime) 
{
	for(std::map<int, Component*>::iterator it = m_Components.begin(); it != m_Components.end(); ++it)
	{
		// Should check to ensure a component exists
		(*it).second->Update(deltaTime);
	}
}

// Apply a central force
void Entity::ApplyForceAtCentre(Entity::EVector3f _force)
{
	if (m_Components[CT_PHYSICS] != nullptr)
	{
		PhysicsComponent* temp = dynamic_cast<PhysicsComponent*>(m_Components[CT_PHYSICS]);
		temp->ApplyForceAtCentre(_force);
	}
}

// Apply an impulse force
void Entity::ApplyImpulseForce(Entity::EVector3f _force)
{
	if (m_Components[CT_PHYSICS] != nullptr)
	{
		PhysicsComponent* temp = dynamic_cast<PhysicsComponent*>(m_Components[CT_PHYSICS]);
		temp->ApplyImpulseForce(_force);
	}
}

// Alter the force of linear gravity
// Use this for smooth linear motion
void Entity::SetGravity(Entity::EVector3f _force)
{
	if (m_Components[CT_PHYSICS] != nullptr)
	{
		PhysicsComponent* temp = dynamic_cast<PhysicsComponent*>(m_Components[CT_PHYSICS]);
		temp->SetGravity(_force);
	}
}

void Entity::Reset()
{
	
}

void Entity::ToggleColor()
{
	GraphicsComponent* temp = dynamic_cast<GraphicsComponent*>(m_Components[CT_GRAPHIC]);
	temp->ToggleColor();
}

void Entity::Render()
{
	for(std::map<int, Component*>::iterator it = m_Components.begin(); it != m_Components.end(); ++it)
	{
		(*it).second->Render();
	}
}


