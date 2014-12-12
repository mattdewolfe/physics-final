// Attach to entities to create moving platform type objects
#ifndef MOVING_PLATFORM_COMPONENT_H
#define MOVING_PLATFORM_COMPONENT_H

#include "Entity.h"
#include "PhysicsComponent.h"

class MovingPlatformComponent : public PhysicsComponent
{
public:
	MovingPlatformComponent() :
		PhysicsComponent() {}
	virtual ~MovingPlatformComponent() {}

	void Init(float _maxOffSet, float _moveSpeed);
	virtual void Shutdown() {}

	virtual void Update(float deltaTime);
	
	virtual void Render() {}
	virtual void OnAddSingleResult(btManifoldPoint& cp,int partId0,int index0,const btCollisionObjectWrapper* collidedObjWrap,int collidedObjPartId,int collidedObjIndex) override;
	virtual void OnContactProcCallback(btManifoldPoint& cp, PhysicsComponent* collider) override;

protected:
	static Entity::EVector3f colours[4];
	float offSet;
	float maxOffSet;
	float moveSpeed;
};

#endif