#ifndef COMPONENT_H
#define COMPONENT_H

#include <iostream>

class Entity;

enum CTYPE
{
	CT_GRAPHIC, 
	CT_PHYSICS, 
	CT_INPUT, 
	CT_PLATFORM, 
	CT_COUNT
};


class Component
{
	
public:
	Component(CTYPE _cT) : 
		compType(_cT), 
		m_Owner(nullptr) {}
	virtual ~Component() {}

	virtual void Init() {}
	virtual void Shutdown() {}

	virtual void Update(float deltaTime) {}
	virtual void Render() {}

	void SetOwner(Entity* owner) {m_Owner = owner;}
	Entity* GetOwner() const {return m_Owner;}

	CTYPE GetComponentType() { return compType; }

protected:
	Entity* m_Owner;
	CTYPE compType;
};

#endif