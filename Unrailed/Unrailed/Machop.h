#pragma once
#include "Train.h"
#include "Electrode.h"
#include "Ore.h"
#include "Bag.h"

class Animation;
class Bag;

class Machop : public Train
{
	vector <Ore*> mOreList;

	Electrode* mElectrode;

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
	Animation* mDownHurt;
	Animation* mUpHurt;
	Animation* mLeftHurt;
	Animation* mRightHurt;

	Bag mBag;
	vector<BagItem*>* mBagItemListPtr;

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
	void EndIntercept();

	void InterceptOre();
	void InterceptOre(ItemType itemType);
	int GetOreCount() { return mOreCount; }
	void SetOreCount(int i) { mOreCount = i; }
	vector <Ore*> GetOreList() { return mOreList; }
	ItemType OreErase();
};