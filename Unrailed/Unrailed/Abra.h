#pragma once
#include "Train.h"
#include "Machop.h"
#include "Electrode.h"
#include "Ore.h"

class Animation;

struct CreatedTrail
{
	Image* image;
	ItemType trailType;
	bool isCreated;
};

class Abra : public Train
{
	vector <CreatedTrail*> mCreatedTrailList;

	Machop* mMachop;
	Electrode* mElectrode;

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
	Animation* mDownHurt;
	Animation* mUpHurt;
	Animation* mLeftHurt;
	Animation* mRightHurt;

public:
	void Init()override {};
	void Init(int x, int y, int image);
	void Release()override;
	void Update()override;
	void Render(HDC hdc)override;

	void ReadyAnimation()override;
	void SetAnimation()override;
	void SetImage(int level);

	void EndExplode()override;
	void EndSynthesis();

	void SynthesisOre();

	ItemType Receive();
};