#pragma once
#include "Scene.h"

class TileMap;
class TrailManager;

class Scene4 : public Scene
{
	TileMap* mTileMap;
	TrailManager* mTrailManager;

	Player* mTempPlayer; // ���� �� ���ο��� �����ؼ� �ӽ÷� ���� ������, ���߿� �÷��̾� 1ȸ ���� �� ������ ���� ������ �����ؾ� ��

public:
	void Init()override;
	void Release()override;
	void Update()override;
	void Render(HDC hdc)override;
};