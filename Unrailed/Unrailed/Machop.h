#pragma once
#include "Train.h"

class Animation;

class Machop : public Train
{
	Animation* mDownMove;
	Animation* mUpMove;
	Animation* mLeftMove;
	Animation* mRightMove;
	Animation* mLeftSleep;
	Animation* mRightSleep;
	Animation* mDownIntercept;
	Animation* mUpIntercept;
	Animation* mLeftIntercept;
	Animation* mRightIntercept;

public:
	void Init()override;
	void Release()override;
	void Update()override;
	void Render(HDC hdc)override;

	void ReadyAnimation();

	void SetAnimation();
};