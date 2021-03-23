#pragma once
#include "Scene.h"

//class Player;
class TileMap;
class Scene3 : public Scene
{
	//Player* mPlayer;
	TileMap* mTileMap;
	class TrailManager* mTrailManager;

	int mTailX;
	int mTailY;

public:
	void Init()override;
	void Release()override;
	void Update()override;
	void Render(HDC hdc)override;
};

