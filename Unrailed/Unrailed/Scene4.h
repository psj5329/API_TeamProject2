#pragma once
#include "Scene.h"

class TileMap;

class Scene4 : public Scene
{
	TileMap* mTileMap;



	Player* mTempPlayer; // ���� �� ���ο��� �����ؼ� �ӽ÷� ���� ������, ���߿� �÷��̾� 1ȸ ���� �� ������ ���� ������ �����ؾ� ��

public:
	void Init()override;
	void Release()override;
	void Update()override;
	void Render(HDC hdc)override;
};