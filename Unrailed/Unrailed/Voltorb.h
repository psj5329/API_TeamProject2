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
	void EndExplode()override;
	void SetImage(int type);

	Direction CheckTrailDirection(); //현재 올라가있는 기차길의 방향을 확인
	void Move(int indexY, int indexX, Direction dir); //기차길[y][x]의 중간까지 기차를 이동시킬
};


