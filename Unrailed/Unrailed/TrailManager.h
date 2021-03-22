#pragma once

#include "Trail.h"

//class Trail;
class TrailManager
{
	//vector<Trail*> mTrailList;
	vector<vector<Trail*>> mTrailList;

	int XTileCount;
	int YTileCount;

	int mStartX;
	int mStartY;

public:
	void Init();
	void Init(int YTileCount, int XTileCount);
	void Release();
	void Update();
	void Render(HDC hdc);

	void SetStartIndex(int y, int x) { mStartX = x; mStartY = y; }

	void InsertTrail(int indexY, int indexX, int type, int dir);	//�ʿ��� Ʈ��������
	void TurnTrail(int indexY, int indexX);		//�÷��̾ Ʈ���� ������
	TrailType PickUpTrail(int indexY, int indexX);			//Ʋ���̾ Ʈ�����ݱ�
	bool PlaceTrail(int indexY, int indexX, int type, int dir);		//�÷��̾ Ʈ���ϼ�ġ

	vector<vector<Trail*>>* GetTrailListPtr() { return &mTrailList; }

private:
	void SetTrailTail(int indexY, int indexX);
};