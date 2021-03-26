#pragma once
#include "BagItem.h"

class Bag
{
	vector<BagItem*> mBagItemList;
	int mOwnerX;
	int mOwnery;

public:
	void Update(int ownerX, int ownerY);
	void Render(HDC hdc);

public:
	vector<BagItem*>* GetBagItemListPtr() { return &mBagItemList; }
	size_t GetBagItemSize() { return mBagItemList.size(); }
	ItemName GetBagItemName() { return mBagItemList[0]->GetName(); }
	ItemType GetBagItemType(int index) { return mBagItemList[index]->GetType(); }

	void SetBagItemListPtr(vector<BagItem*>* bagItemListPtr) { mBagItemList = *bagItemListPtr; }
};