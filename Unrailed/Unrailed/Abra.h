#pragma once
#include "Train.h"

class Animation;

class Abra : public Train
{
	Animation* mDownMove;
	Animation* mUpMove;
	Animation* mLeftMove;
	Animation* mRightMove;
	Animation* mLeftSleep;
	Animation* mRightSleep;
	Animation* mDownSynthesis;
	Animation* mUpSynthesis;
	Animation* mLeftSynthesis;
	Animation* mRightSynthesis;

public:
	void Init()override;
	void Release()override;
	void Update()override;
	void Render(HDC hdc)override;

	void ReadyAnimation();

	void SetAnimation();
};