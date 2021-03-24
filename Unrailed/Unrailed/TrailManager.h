#pragma once

#include "Trail.h"

class TrailManager
{
	vector<vector<Trail*>> mTrailList;

	int XTileCount;
	int YTileCount;

	int mStartX;
	int mStartY;

	Animation* mDown;
	Animation* mUp;
	Animation* mLeft;
	Animation* mRight;
	Animation* mCurrentAnimation;

	int mOrder;

public:
	void Init();
	void Init(int YTileCount, int XTileCount);
	void Release();
	void Update();
	void Render(HDC hdc);

	int GetXTileCount() { return XTileCount; }
	int GetYTileCount() { return YTileCount; }
	Animation* GetDownAnimationPtr() { return mDown; }
	Animation* GetUpAnimationPtr() { return mUp; }
	Animation* GetLeftAnimationPtr() { return mLeft; }
	Animation* GetRightAnimationPtr() { return mRight; }
	Animation* GetCurrentAnimation() { return mCurrentAnimation; }

	void SetStartIndex(int y, int x) { mStartX = x; mStartY = y; }
	void InsertTrail(int indexY, int indexX, ItemType type, int dir);	//맵에서 트레일인잇
	void TurnTrail(int indexY, int indexX);		//플레이어가 트레일 돌리기
	//void TurnTrail2(int indexY, int indexX);
	ItemType PickUpTrail(int indexY, int indexX);			//틀레이어가 트레일줍기
	bool PlaceTrail(int indexY, int indexX, ItemType type, int dir);		//플레이어가 트레일설치


	vector<vector<Trail*>>* GetTrailListPtr() { return &mTrailList; }

	void FindTail(int* y, int* x);

private:
	//void SetTrailTail2(int indexY, int indexX);
	void SetTrailTail(int indexY, int indexX);
	bool GetNextTrailIndex(int indexX, int indexY, POINT* pOutput);
	bool CheckIsLoop(int indexX, int indexY);
	void AnimationInit();
};