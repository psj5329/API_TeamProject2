#pragma once
#include "Scene.h"
#include "Voltorb.h"

class TrailManager;
class TileMap;
class Scene6 : public Scene
{
	//Player* mPlayer;
	TileMap* mTileMap;
	TrailManager* mTrailManager;
	
	vector <Voltorb*> mVoltorbVec;

public:
	void Init()override;
	void Release()override;
	void Update()override;
	void Render(HDC hdc)override;
};
