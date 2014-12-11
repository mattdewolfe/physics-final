#ifndef ENTITY_H
#define ENTITY_H

#include <map>
#include "Component.h"

class Component;

class Entity 
{
public:
	struct EVector3f 
	{
	public:
		EVector3f(float _x = 0, float _y = 0, float _z = 0) : x(_x), y(_y), z(_z) { }
		float x;
		float y;
		float z;
	};

	Entity() 
	{
		playerWon = false;
	}
	~Entity() {}

	void AddComponent(Component *component);

	void RemoveComponent(Component* component)
	{
		for(std::map<int, Component*>::iterator it = m_Components.begin(); it != m_Components.end(); ++it)
		{
			if( (*it).second == component)
			{
				it = m_Components.erase(it);
				break;
			}
		}
	}

	void RemoveComponentByType(CTYPE _type)
	{
		for(std::map<int, Component*>::iterator it = m_Components.begin(); it != m_Components.end(); ++it)
		{
			if( (*it).first == _type)
			{
				it = m_Components.erase(it);
				break;
			}
		}
	}

	void RemoveAllComponents()
	{
		m_Components.clear();
	}
	Component* GetComponent(CTYPE _type) { return m_Components[(int)_type]; }

	void Update(float deltaTime);
	void Render();
	void PlayerWon() { playerWon = true; }
	bool CheckPlayerWon() { return playerWon; }

	// Wrappers for physics calls, this allows alteration of the physics engine without
	// having to redifine all functions
	void ApplyForceAtCentre(Entity::EVector3f _force);
	void ApplyImpulseForce(Entity::EVector3f _force);
	void SetGravity(Entity::EVector3f _force);
	void SetBodyPosition();
	void Reset();
	void ClearForces();

	// Toggle the color on this entities graphic component
	void ToggleColor();

	EVector3f GetPosition() const {return m_Position;}
	void SetPosition(EVector3f pos) 
	{ 
		m_Position = pos;
	}
	void SetPosition(float _x, float _y, float _z)
	{
		m_Position.x = _x;
		m_Position.y = _y;
		m_Position.z = _z;
	}

private:
	std::map<int, Component*> m_Components;
	EVector3f m_Position;
	bool playerWon;
};


#endif