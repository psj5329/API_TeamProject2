#include "pch.h"
#include "Scene6.h"
#include "TrailManager.h"
#include "TileMap.h"

void Scene6::Init()
{
	mTileMap = new TileMap();
	mTileMap->Init(TileCountX, TileCountY, TileSize);
	mTileMap->LoadMap();

	mTrailManager = new TrailManager();
	mTrailManager->Init();
	mTrailManager->InsertTrail(0, 6, 1, 3);
	mTrailManager->InsertTrail(1, 6, 1, 3);
	mTrailManager->InsertTrail(2, 6, 1, 3);
	mTrailManager->InsertTrail(3, 6, 1, 0);
	mTrailManager->InsertTrail(3, 7, 1, 3);
	mTrailManager->InsertTrail(4, 7, 1, 3);
	mTrailManager->InsertTrail(5, 7, 1, 3);
	mTrailManager->InsertTrail(6, 7, 1, 3);
	mTrailManager->InsertTrail(7, 7, 1, 1);
	mTrailManager->InsertTrail(7, 6, 1, 1);
	mTrailManager->InsertTrail(7, 5, 1, 3);
	mTrailManager->InsertTrail(8, 5, 1, 3);


	//±âÂ÷ÀÎÀÕ

}

void Scene6::Release()
{
	mTrailManager->Release();
	mTileMap->Release();
}

void Scene6::Update()
{
	mTileMap->Update();
	mTrailManager->Update();
}

void Scene6::Render(HDC hdc)
{
	mTileMap->Render(hdc);

	OBJECTMANAGER->Render(hdc);

	mTrailManager->Render(hdc);
}