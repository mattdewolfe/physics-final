#ifndef BALLCOMPONENT_H
#define BALLCOMPONENT_H

#include "Entity.h"
#include "Component.h"
#include "PhysicsComponent.h"
#include "../BulletCollision/CollisionDispatch/btCollisionWorld.h"

class btRigidBody;

class BallComponent : public PhysicsComponent
{
public:

	enum RigidBodyShapeTypes { RBST_Plane = 0, RBST_Sphere, RBST_Box };

	BallComponent() : 
		PhysicsComponent() { }
	virtual ~BallComponent() {}

	virtual void Update(float deltaTime);

	virtual void OnAddSingleResult(btManifoldPoint& cp,int partId0,int index0,const btCollisionObjectWrapper* collidedObjWrap,int collidedObjPartId,int collidedObjIndex);
	virtual void OnContactProcCallback(btManifoldPoint& cp, PhysicsComponent* collider);
	
};

#endif