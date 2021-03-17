#pragma once

//±âÂ÷±æÀ» ´Ù·ê°÷
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

	void InsertTrail(int x, int y, int type, int dir);	//ÀÎÀÕ¿¡¼­³Ö±â
	void TurnTrail(int indexY, int indexX);		//µ¹¸®±â
	void PickUpTrail(int indexY, int indexX);			//ÁÝ±â
	void PlaceTrail(int x, int y, int type, int dir);		//¼³Ä¡ÇÏ±â

	vector <vector <Trail*>>* GetTrailListPtr() { return &mTrailList; }
};

