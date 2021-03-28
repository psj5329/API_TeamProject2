#pragma once
#include "UI.h"

class BagItem : public UI
{
	ItemName mItemName;
	ItemType mItemType;

	int mFrameIndexX;
	int mFrameIndexY;

public:
	void Init()override;
	void Init(ItemName name, ItemType type); // 반드시 해줘야 함
	void Release()override;
	void Update()override;
	void Render(HDC hdc)override;

public:
	ItemName GetName() const { return mItemName; }
	ItemType GetType() const { return mItemType; }

	void SetLocation(int x, int y) { mX = x; mY = y; }
	void SetName(ItemName name) { mItemName = name; }
	void SetType(ItemType type) { mItemType = type; }
};