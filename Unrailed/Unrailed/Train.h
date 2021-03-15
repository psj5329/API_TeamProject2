#pragma once
#include "GameObject.h"

enum class Direction : int
{
	Down,
	Up,
	Left,
	Right
};

enum class State : int
{
	Move,
	Shock,
	Sleep,
	Intercept,
	Synthesis
};

class Train : public GameObject
{
protected:
	State mState;
	Direction mDirection;
	float mSpeed;
	float mTimer;
	int mLevel;
	bool mStop;

public:
	virtual void Init()override = 0;
	virtual void Release()override = 0;
	virtual void Update()override = 0;
	virtual void Render(HDC hdc)override = 0;

	virtual void ReadyAnimation() = 0;

	virtual void SetAnimation() = 0;
};

