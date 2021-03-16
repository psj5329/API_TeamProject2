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
	mTrailManager->InsertTrail(0, 7, 1, 3);
	mTrailManager->InsertTrail(1, 7, 1, 3);


	//ÀûÀÎÀÕ

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