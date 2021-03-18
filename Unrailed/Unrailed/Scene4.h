#pragma once
#include "Scene.h"

class TileMap;

class Scene4 : public Scene
{
	TileMap* mTileMap;



	Player* mTempPlayer; // 현재 씬 내부에서 생성해서 임시로 만든 변수임, 나중에 플레이어 1회 생성 후 가져다 쓰는 구조로 변경해야 함

public:
	void Init()override;
	void Release()override;
	void Update()override;
	void Render(HDC hdc)override;
};