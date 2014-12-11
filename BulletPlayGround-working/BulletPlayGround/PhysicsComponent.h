#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "Entity.h"
#include "Component.h"
#include "../BulletCollision/CollisionDispatch/btCollisionWorld.h"

class btRigidBody;

class PhysicsComponent : public Component
{
public:

	enum RigidBodyShapeTypes { RBST_Plane = 0, RBST_Sphere, RBST_Box };

	PhysicsComponent() : 
		Component(CT_PHYSICS), 
		m_shapeType(RBST_Plane) { }
	virtual ~PhysicsComponent() {}

	void Init(RigidBodyShapeTypes type, float _mass);
	void Shutdown();

	virtual void Update(float deltaTime);

	virtual void OnAddSingleResult(btManifoldPoint& cp,int partId0,int index0,const btCollisionObjectWrapper* collidedObjWrap,int collidedObjPartId,int collidedObjIndex);
	virtual void OnContactProcCallback(btManifoldPoint& cp, PhysicsComponent* collider);
	
	void ApplyForceAtCentre(Entity::EVector3f _force);
	void ApplyImpulseForce(Entity::EVector3f _force);
	void SetGravity(Entity::EVector3f _force);
	void DebugPrint();
	void RotateBody(float _degrees);

	btRigidBody* GetRigidBody() { return m_rigidBody; }

	void SetShapeType(RigidBodyShapeTypes type) { m_shapeType = type;}
	RigidBodyShapeTypes GetShapeType() const { return m_shapeType; }

private:
	void CreateBox(Entity::EVector3f _pos, float _mass);
	void CreatePlane(Entity::EVector3f _pos, float _mass);
	void CreateSphere(Entity::EVector3f _pos, float _mass);
protected:
	//physics variables
	btRigidBody* m_rigidBody;
	RigidBodyShapeTypes m_shapeType;
};

#endif