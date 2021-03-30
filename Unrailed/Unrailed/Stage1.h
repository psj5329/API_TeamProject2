#pragma once
#include "Scene.h"
class Stage1 :
	public Scene
{
	TileMap* mTileMap;
	class TrailManager* mTrailManager;

	Player* mTempPlayer; // 현재 씬 내부에서 생성해서 임시로 만든 변수임, 나중에 플레이어 1회 생성 후 가져다 쓰는 구조로 변경해야 함

	class Jigglypuff* mJigglypuff;
	vector <class Voltorb*> mVoltorbVec;
	class Electrode* mElectrode;
	class Abra* mAbra;
	class Machop* mMachop;

public:
	void Init()override;
	void Release()override;
	void Update()override;
	void Render(HDC hdc)override;

	void InitTrain()override;
	void InitJigglypuff()override;
	void PlaceTrail() override;


};

