#pragma once
#include "Scene.h"
class Sableye;
class Stage4 :
	public Scene
{
	TileMap* mTileMap;
	class TrailManager* mTrailManager;

	Player* mTempPlayer; // ���� �� ���ο��� �����ؼ� �ӽ÷� ���� ������, ���߿� �÷��̾� 1ȸ ���� �� ������ ���� ������ �����ؾ� ��

	class Jigglypuff* mJigglypuff;
	vector <class Voltorb*> mVoltorbVec;
	class Electrode* mElectrode;
	class Abra* mAbra;
	class Machop* mMachop;
	vector<Sableye*> mEnemyVec;

public:
	void Init()override;
	void Release()override;
	void Update()override;
	void Render(HDC hdc)override;


	void InitTrain()override;
	void InitJigglypuff()override;
	void PlaceTrail() override;
};
