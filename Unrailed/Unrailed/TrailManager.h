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

	Animation* mDown;
	Animation* mUp;
	Animation* mLeft;
	Animation* mRight;
	Animation* mCurrentAnimation;


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
	void InsertTrail(int indexY, int indexX, int type, int dir);	//�ʿ��� Ʈ��������
	void TurnTrail(int indexY, int indexX);		//�÷��̾ Ʈ���� ������
	void TurnTrail2(int indexY, int indexX);
	TrailType PickUpTrail(int indexY, int indexX);			//Ʋ���̾ Ʈ�����ݱ�
	bool PlaceTrail(int indexY, int indexX, int type, int dir);		//�÷��̾ Ʈ���ϼ�ġ

	bool CheckIsLoop(int indexX, int indexY);
	bool GetNextTrailIndex(int indexX, int indexY, POINT* pOutput);

	vector<vector<Trail*>>* GetTrailListPtr() { return &mTrailList; }

	void FindTail(int* y, int* x);

private:
	void SetTrailTail(int indexY, int indexX);
	void SetTrailTail2(int indexY, int indexX);

	void AnimationInit();
};