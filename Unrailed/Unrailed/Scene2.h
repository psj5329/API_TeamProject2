#pragma once
#include "Scene.h"
#include "Voltorb.h"
#include "Electrode.h"
#include "Machop.h"
#include "Abra.h"
#include "Jigglypuff.h"

class TrailManager;
class TileMap;
class Scene2 : public Scene
{
	//Player* mPlayer;
	TileMap* mTileMap;
	TrailManager* mTrailManager;

	vector <Voltorb*> mVoltorbVec;
	Electrode* mElectrode;
	Abra* mAbra;
	Machop* mMachop;
	Jigglypuff* mJigglypuff;

public:
	void Init()override;
	void Release()override;
	void Update()override;
	void Render(HDC hdc)override;
};
