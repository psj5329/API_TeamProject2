#pragma once
#include "Scene.h"

class TileMap;

class Scene4 : public Scene
{
	TileMap* mTileMap;

public:
	void Init()override;
	void Release()override;
	void Update()override;
	void Render(HDC hdc)override;
};