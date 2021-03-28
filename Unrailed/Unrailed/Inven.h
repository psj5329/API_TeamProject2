#pragma once
#include "BagItem.h"

class Image;

class Inven
{
	Image* mImage;
	int mSizeX;
	int mSizeY;
	int mHiddenItem; // 0: not find 1: find 2: used

	vector<BagItem*> mBagItemList;

public:
	void Init();
	void Update();
	void Render(HDC hdc);

public:
	vector<BagItem*>* GetBagItemListPtr() { return &mBagItemList; }
	size_t GetBagItemSize() { return mBagItemList.size(); }
	ItemName GetBagItemName() { return mBagItemList[0]->GetName(); }
	ItemType GetBagItemType(int index) { return mBagItemList[index]->GetType(); }
	int GetHiddenItem() { return mHiddenItem; }

	void SetBagItemListPtr(vector<BagItem*>* bagItemListPtr) { mBagItemList = *bagItemListPtr; }
	void SetHiddenItem(int hiddenItem) { mHiddenItem = hiddenItem; }
};