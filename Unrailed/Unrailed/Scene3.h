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

	Player* mTempPlayer; // 현재 씬 내부에서 생성해서 임시로 만든 변수임, 나중에 플레이어 1회 생성 후 가져다 쓰는 구조로 변경해야 함

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

