#pragma once
#include "Scene.h"

class Image;
class Player;
class Scene1 : public Scene
{
	TileMap* mTileMap;
	Player* mPlayer;

public:
	void Init()override;
	void Release()override;
	void Update()override;
	void Render(HDC hdc)override;

public:
	TileMap* GetTileMap() { return mTileMap; }

public:
	void LoadResources();
};

