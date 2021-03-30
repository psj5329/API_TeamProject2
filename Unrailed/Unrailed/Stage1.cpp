#include "pch.h"
#include "Stage1.h"
#include "TileMap.h"
#include "MapToolScene.h"
#include "Camera.h"
#include "TrailManager.h"
#include "Player.h"
#include "Jigglypuff.h"
#include "Electrode.h"
#include "Machop.h"
#include "Abra.h"
#include "Voltorb.h"

void Stage1::Init()
{
	mTileMap = new TileMap();
	mTileMap->LoadMap(4);
	int x = mTileMap->GetXTileCount();
	int y = mTileMap->GetYTileCount();

	mTrailManager = new TrailManager();
	mTrailManager->Init(y, x);

	//플레이어 설정
	//mTempPlayer->SetTileCount(x, y);

	PlaceTrail();
	InitJigglypuff();
	InitTrain();
}

void Stage1::Release()
{

}

void Stage1::Update()
{
	mTileMap->Update();
	mTrailManager->Update();
	OBJECTMANAGER->Update();
	CAMERAMANAGER->GetMainCamera()->Update();
}

void Stage1::Render(HDC hdc)
{
	mTileMap->Render(hdc);
	mTrailManager->Render(hdc);
	OBJECTMANAGER->Render(hdc);
}

void Stage1::PlaceTrail()
{

	mTrailManager->PlaceTrail(13, 0, ItemType::Green, 3); // 0: down 1: up 2: left 3: right
	mTrailManager->PlaceTrail(13, 1, ItemType::Green, 3);
	mTrailManager->SetStartIndex(13, 0);
	mTrailManager->PlaceTrail(13, 2, ItemType::Green, 3);
	mTrailManager->PlaceTrail(13, 3, ItemType::Green, 3);
	mTrailManager->PlaceTrail(13, 4, ItemType::Green, 3);
	mTrailManager->PlaceTrail(13, 5, ItemType::Green, 3);
	mTrailManager->PlaceTrail(13, 6, ItemType::Green, 3);
	mTrailManager->PlaceTrail(13, 7, ItemType::Green, 3);


	mTrailManager->PlaceTrail(15, 39, ItemType::Green, 1);
	mTrailManager->SetEndIndex(15, 39);
}

void Stage1::InitTrain()
{
	//노기차

}

void Stage1::InitJigglypuff()
{
	//노푸린
}