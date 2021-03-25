#pragma once
#include "Train.h"
#include "Ore.h"

class Animation;

class Machop : public Train
{
	vector <Ore*> mOreList;

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
	//Machop();

	void Init()override;
	void Release()override;
	void Update()override;
	void Render(HDC hdc)override;

	void ReadyAnimation()override;
	void SetAnimation()override;

	void EndExplode()override;
	void EndIntercept();

	void InterceptOre();
	void InterceptOre(ItemType itemType); // ���� �߰�
	int GetOreCount() { return mOreCount; }
	void SetOreCount(int i) { mOreCount = i; }
	vector <Ore*> GetOreList() { return mOreList; }
	ItemType OreErase();
};