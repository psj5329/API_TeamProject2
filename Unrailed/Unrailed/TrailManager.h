#pragma once


class Trail;
class TrailManager
{
	vector <Trail> mTrailList;

public:
	void Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	void InsertTrail();
	void TurnTrail();
};

