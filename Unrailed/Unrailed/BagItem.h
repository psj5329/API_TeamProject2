#pragma once
#include "UI.h"

enum ItemName
{
	ItemOre,
	ItemTrail,
	ItemNameEnd
};

class BagItem : public UI
{
	ItemName mItemName;
	ItemType mItemType;

	int mFrameIndexX;
	int mFrameIndexY;

	//int mCount;

public:
	void Init()override;
	void Init(const wstring& key, int x, int y, int sizeX, int sizeY, int frameIndexX, int frameIndexY);
	void Init(ItemName name, ItemType type); // 반드시 해줘야 함
	void Release()override;
	void Update()override;
	void Render(HDC hdc)override;

public:
	ItemName GetName() const { return mItemName; }
	ItemType GetType() const { return mItemType; }
//	int GetCount() const { return mCount; }

	//void SetImage(wstring key) { mImage = IMAGEMANAGER->FindImage(key); }
	void SetLocation(int x, int y) { mX = x; mY = y; }
	//void SetLocation(int x, int y, int sizeX, int sizeY) { mX = x; mY = y; mSizeX = sizeX; mSizeY = sizeY; }
	void SetName(ItemName name) { mItemName = name; }
	void SetType(ItemType type) { mItemType = type; }
//	void SetCount(int count) { mCount = count; }

//	void PlusCount() { ++mCount; }
//	void MinusCount() { --mCount; }
};