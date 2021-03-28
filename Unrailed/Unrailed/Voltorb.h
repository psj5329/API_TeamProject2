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

	class Electrode* mElectrode;

public:
	void Init()override {};
	void Init(int x, int y);
	void Release()override;
	void Update()override;
	void Render(HDC hdc)override;

	void ReadyAnimation()override;
	void SetAnimation()override;
	
	void EndExplode()override;

	//머리설정
	void SetElectrode(Electrode* electrode) { mElectrode = electrode; }
};


