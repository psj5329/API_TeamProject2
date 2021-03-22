#pragma once
#include "Train.h"

class Animation;

class Voltorb : public Train
{
	Animation* mDownMove;
	Animation* mUpMove;
	Animation* mLeftMove;
	Animation* mRightMove;
	Animation* mSleep;

public:
	void Init()override;
	void Release()override;
	void Update()override;
	void Render(HDC hdc)override;

	void ReadyAnimation()override;
	void SetAnimation()override;
	void SetImage(int type);

	void EndExplode()override;
};


