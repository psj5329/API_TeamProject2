#pragma once
#include "BagItem.h"

class Image;

class Inven
{
	Image* mImage;
	int mSizeX;
	int mSizeY;

	vector<BagItem*> mBagItemList;
	//int mOwnerX;
	//int mOwnery;

	int mHiddenItem; // 0: not find 1: find 2: used

public:
	void Init();
	void Update();
	void Render(HDC hdc);

public:
	vector<BagItem*>* GetBagItemListPtr() { return &mBagItemList; }
	size_t GetBagItemSize() { return mBagItemList.size(); }
	ItemName GetBagItemName() { return mBagItemList[0]->GetName(); }
	ItemType GetBagItemType(int index) { return mBagItemList[index]->GetType(); }

	void SetBagItemListPtr(vector<BagItem*>* bagItemListPtr) { mBagItemList = *bagItemListPtr; }
};