#pragma once

//�̾��� �������� �ٷ��
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

	void InsertTrail(int x, int y, int type, int dir);
	void TurnTrail();
	void PickUpTrail();
	
	vector <vector <Trail*>>* GetTrailListPtr() { return &mTrailList; }
};

