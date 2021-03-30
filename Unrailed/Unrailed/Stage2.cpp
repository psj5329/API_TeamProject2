#include "pch.h"
#include "Stage2.h"
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

void Stage2::Init()
{
	mTileMap = new TileMap();
	mTileMap->LoadMap(2);
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

void Stage2::Release()
{

}

void Stage2::Update()
{
	mTileMap->Update();
	mTrailManager->Update();
	OBJECTMANAGER->Update();
	CAMERAMANAGER->GetMainCamera()->Update();
}

void Stage2::Render(HDC hdc)
{
	mTileMap->Render(hdc);
	mTrailManager->Render(hdc);
	OBJECTMANAGER->Render(hdc);
}

void Stage2::PlaceTrail()
{

	mTrailManager->PlaceTrail(7, 0, ItemType::Green, 3); // 0: down 1: up 2: left 3: right
	mTrailManager->PlaceTrail(7, 1, ItemType::Green, 3);
	mTrailManager->SetStartIndex(7, 0);
	mTrailManager->PlaceTrail(7, 2, ItemType::Green, 3);
	mTrailManager->PlaceTrail(7, 3, ItemType::Green, 3);
	mTrailManager->PlaceTrail(7, 4, ItemType::Green, 3);
	mTrailManager->PlaceTrail(7, 5, ItemType::Green, 3);
	//mTrailManager->PlaceTrail(6, 5, ItemType::Green, 3);
	//mTrailManager->PlaceTrail(6, 6, ItemType::Green, 3);
	//mTrailManager->PlaceTrail(6, 7, ItemType::Green, 3);
	//mTrailManager->PlaceTrail(6, 8, ItemType::Green, 3);

	mTrailManager->PlaceTrail(9, 20, ItemType::Green, 1);
	mTrailManager->SetEndIndex(9, 20);
}

void Stage2::InitTrain()
{
	//열차
	vector <vector <Trail*>>* temp = mTrailManager->GetTrailListPtr();

	mElectrode = new Electrode;
	mElectrode->Init(120, 350);
	mElectrode->SetTrail(temp);
	mElectrode->SetJigglypuff(mJigglypuff);

	for (int i = 0; i < 3; i++)
	{
		Voltorb* voltorb = new Voltorb;
		voltorb->Init(80 - 35 * i, 350);
		//voltorb->SetElectrode(mElectrode);
		mVoltorbVec.push_back(voltorb);
		mVoltorbVec[i]->SetTrail(temp);
	}

	mElectrode->SetNextTrain(mVoltorbVec[0]);

	for (int i = 0; i < 3; i++)
	{
		if (i != 2)
		{
			mVoltorbVec[i]->SetNextTrain(mVoltorbVec[i + 1]);
			mVoltorbVec[i]->SetJigglypuff(mJigglypuff);
		}
		else
		{
			mVoltorbVec[i]->SetNextTrain(nullptr);
			mVoltorbVec[i]->SetJigglypuff(mJigglypuff);
		}
	}

	mMachop = new Machop;
	mMachop->Init(-30, 350, 1);
	mMachop->SetTrail(temp);

	mAbra = new Abra;
	mAbra->Init(-70, 350, 1);
	mAbra->SetTrail(temp);

}

void Stage2::InitJigglypuff()
{
	mJigglypuff = new Jigglypuff();
	mJigglypuff->Init(200, 400, false);
	vector<vector<Tile*>> tileList = *mTileMap->GetTileListPtr();
	mJigglypuff->SetTileListPtr(tileList);
	vector<vector<MapObject*>> mapObjectList = *mTileMap->GetObjectListPtr();
	mJigglypuff->SetMapObjectListPtr(mapObjectList);
}