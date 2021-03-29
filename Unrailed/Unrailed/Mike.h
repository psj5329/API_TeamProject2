#pragma once
#include "GameObject.h"


enum class MikeState
{
	OnGround,
	Player,
	Jigglypuff
};

class Mike :
	public GameObject
{
	class Image* mImage;

	MikeState mState;

	int indexX;
	int indexY;

public:

	void Init()override;
	void Release()override;
	void Update()override;
	void Render(HDC hdc)override;

	MikeState GetMikeState() { return mState; }
	void SetMikeState(MikeState state) { mState = state; }

	bool PlaceMikeFloat(float x, float y);
	bool PlaceMike(int indexX, int indexY);

	bool PickUpMikePlayer(int indexX, int indexY);
	bool PickUpMikeJigglypff();

	int GetIndexX() { return indexX; }
	int GetIndexY() { return indexY; }

};

