#pragma once
#include "GameObject.h"

//enum class ItemType
//{
//	None,
//	Green,
//	Blue,
//	Red
//};
class Image;

class Ore :
	public GameObject
{
	ItemType mType;
	Image* mImage;
	int mCount;
	int mFrameX;

public:
	void Init()override;
	void Release()override;
	void Update()override;
	void Render(HDC hdc)override;

	int GetOreTypeInt() { return (int)mType; }
	ItemType GetOreType() { return mType; }
	int GetCount() { return mCount; }

	void SetOreType(ItemType type) { mType = type; }
	void Drop(int x, int y, ItemType type);
	ItemType PickUp();
	void Place(int x, int y);

	void PlusCount() { mCount += 1; }
};

