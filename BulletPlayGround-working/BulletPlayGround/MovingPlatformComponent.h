// Attach to entities to create moving platform type objects
#ifndef MOVING_PLATFORM_COMPONENT_H
#define MOVING_PLATFORM_COMPONENT_H

#include "Entity.h"
#include "Component.h"

class MovingPlatformComponent : public Component
{
public:
	MovingPlatformComponent() :
		Component(CT_PLATFORM) {}
	virtual ~MovingPlatformComponent() {}

	void Init(float _maxOffSet, float _moveSpeed);
	virtual void Shutdown() {}

	virtual void Update(float deltaTime) override;
	
	virtual void Render() {}

protected:
	static Entity::EVector3f colours[4];
	float offSet;
	float maxOffSet;
	float moveSpeed;
};

#endif