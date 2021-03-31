#pragma once
#include "Scene.h"

class Player;
class Sableye;
class TrailManager;
class Electrode;
class Voltorb;
class Jigglypuff;
class Scene1 : public Scene
{
	TileMap* mTileMap;
	Player* mPlayer;
	Sableye* mEnemy;
	TrailManager* mTrailManager;
	Electrode* mElectrode;
	Voltorb* mVoltorb;
	Jigglypuff* mJigglypuff;


public:
	void Init()override;
	void Release()override;
	void Update()override;
	void Render(HDC hdc)override;

public:
	TileMap* GetTileMap() { return mTileMap; }
};

