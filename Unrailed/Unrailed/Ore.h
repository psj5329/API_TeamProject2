#pragma once
#include "GameObject.h"

enum class OreType
{
	Green,
	Blue,
	Red
};
class Image;
class Ore :
	public GameObject
{
	OreType mType;
	Image* mImage;

	int mFrameX;

public:
	void Init()override;
	void Release()override;
	void Update()override;
	void Render(HDC hdc)override;

	int GetOreTypeInt() { return (int)mType; }
	OreType GetOreType() { return mType; }

	void SetOreType(int type) { mType = (OreType)(type + 1); }
	void Drop(int x, int y, int type);
	int PickUp();
	void Place(int x, int y);
};

