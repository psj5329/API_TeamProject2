#pragma once
#include "Scene.h"

#define TileCountX 10 // 나중에 define 옮기면 지워주기
#define TileCountY 10 // 나중에 define 옮기면 지워주기

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