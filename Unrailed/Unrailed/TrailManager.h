#pragma once

//기차길을 다룰곳
class Trail;
class TrailManager
{
	//vector <Trail* > mTrailList;
	vector <vector <Trail*>> mTrailList;
public:
	void Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	
	void InsertTrail(int x, int y, int type, int dir);	//인잇에서넣기
	void TurnTrail(int indexY, int indexX);		//돌리기
	void PickUpTrail(int indexY, int indexX);			//줍기
	void PlaceTrail(int x, int y, int type, int dir);		//설치하기

	vector <vector <Trail*>>* GetTrailListPtr() { return &mTrailList; }

};

