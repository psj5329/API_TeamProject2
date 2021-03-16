#pragma once
#include "Scene.h"

//class Player;
class TileMap;
class Scene3 : public Scene
{
	//Player* mPlayer;
	TileMap* mTileMap;

public:
	void Init()override;
	void Release()override;
	void Update()override;
	void Render(HDC hdc)override;
};

