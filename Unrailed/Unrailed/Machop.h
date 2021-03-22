#pragma once
#include "Train.h"
#include "Ore.h"

class Animation;

class Machop : public Train
{
	vector <Ore*> mOreList;
	int mGreenOreCount;
	int mBlueOreCount;
	int mRedOreCount;

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

	void ReadyAnimation()override;
	void SetAnimation()override;

	void EndExplode()override;
	void EndIntercept();

	void TakeOre();
};