#pragma once

#include "Trail.h"

//class Trail;
class TrailManager
{
	//vector<Trail*> mTrailList;
	vector<vector<Trail*>> mTrailList;

	int mStartX;
	int mStartY;

public:
	void Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	void SetStartIndex(int y, int x) { mStartX = x; mStartY = y; }

	void InsertTrail(int indexY, int indexX, int type, int dir);	//맵에서 트레일인잇
	void TurnTrail(int indexY, int indexX);		//플레이어가 트레일 돌리기
	TrailType PickUpTrail(int indexY, int indexX);			//틀레이어가 트레일줍기
	bool PlaceTrail(int indexY, int indexX, int type, int dir);		//플레이어가 트레일설치

	vector<vector<Trail*>>* GetTrailListPtr() { return &mTrailList; }

	void SetTrailTail(int indexY, int indexX);
};