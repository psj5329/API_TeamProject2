#pragma once
#include "Train.h"

class Image;
class Animation;

class Machop : public Train
{
	Image* mImage;

	Animation* mCurrentAnimation;
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