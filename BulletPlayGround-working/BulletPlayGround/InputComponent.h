// Allow key input to manipulate this entity
#ifndef INPUT_COMPONENT_H
#define INPUT_COMPONENT_H

#include "Entity.h"
#include "Component.h"

class InputComponent : public Component
{
public:
	InputComponent() : 
		Component(CT_INPUT), 
		releaseBall(false),
		ballReleased(false), 
		leftWasReleased(false), 
		rightWasReleased(false)
	{}
	virtual ~InputComponent() {}

	void Init();
	virtual void Shutdown() {}
	virtual void Update(float deltaTime) override;
	virtual void Render() {}

	void Reset()
	{
		releaseBall = false;
		ballReleased= false; 
		leftWasReleased = false; 
		rightWasReleased = false;
	}

	void LeftReleased();
	void RightReleased();
	void SetRelease();

protected:
	bool leftWasReleased;
	bool rightWasReleased;
	bool releaseBall;
	bool ballReleased;
};

#endif