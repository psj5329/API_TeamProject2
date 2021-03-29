#pragma once
#include "Scene.h"

//class Player;
class TileMap;
class Jigglypuff;
class Scene3 : public Scene
{
	//Player* mPlayer;
	TileMap* mTileMap;
	class TrailManager* mTrailManager;

	Player* mTempPlayer; // ���� �� ���ο��� �����ؼ� �ӽ÷� ���� ������, ���߿� �÷��̾� 1ȸ ���� �� ������ ���� ������ �����ؾ� ��

	int mTailX;
	int mTailY;

	class Image* mImage;
	Jigglypuff* mJigglypuff;

public:
	void Init()override;
	void Release()override;
	void Update()override;
	void Render(HDC hdc)override;
};

