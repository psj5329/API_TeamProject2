#pragma once
#include "Scene.h"

class TrailManager;
class TileMap;
class Scene6 : public Scene
{
	//Player* mPlayer;
	TileMap* mTileMap;
	TrailManager* mTrailManager;

public:
	void Init()override;
	void Release()override;
	void Update()override;
	void Render(HDC hdc)override;
};
