#pragma once
#include "Scene.h"

class Player;
class Sableye;
class Scene1 : public Scene
{
	TileMap* mTileMap;
	Player* mPlayer;
	Sableye* mEnemy;

public:
	void Init()override;
	void Release()override;
	void Update()override;
	void Render(HDC hdc)override;

public:
	TileMap* GetTileMap() { return mTileMap; }
};

