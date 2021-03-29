#pragma once
#include "Train.h"
#include "Electrode.h"

class Animation;

class Voltorb : public Train
{
	Electrode* mElectrode;

	Animation* mDownMove;
	Animation* mUpMove;
	Animation* mLeftMove;
	Animation* mRightMove;
	Animation* mSleep;
	Animation* mDownHurt;
	Animation* mUpHurt;
	Animation* mLeftHurt;
	Animation* mRightHurt;


public:
	void Init()override {};
	void Init(int x, int y);
	void Release()override;
	void Update()override;
	void Render(HDC hdc)override;

	void ReadyAnimation()override;
	void SetAnimation()override;
	
	void EndExplode()override;

};


