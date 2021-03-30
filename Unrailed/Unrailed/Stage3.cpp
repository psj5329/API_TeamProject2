#include "pch.h"
#include "Stage3.h"
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

void Stage3::Init()
{
	mTileMap = new TileMap();
	mTileMap->LoadMap(3);
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

void Stage3::Release()
{

}

void Stage3::Update()
{
	mTileMap->Update();
	mTrailManager->Update();
	OBJECTMANAGER->Update();
	CAMERAMANAGER->GetMainCamera()->Update();
}

void Stage3::Render(HDC hdc)
{
	mTileMap->Render(hdc);
	mTrailManager->Render(hdc);
	OBJECTMANAGER->Render(hdc);
}

void Stage3::PlaceTrail()
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

void Stage3::InitTrain()
{
	//열차
	int y = 650;
	vector <vector <Trail*>>* temp = mTrailManager->GetTrailListPtr();

	mElectrode = new Electrode;
	mElectrode->Init(120, y);
	mElectrode->SetTrail(temp);
	mElectrode->SetJigglypuff(mJigglypuff);

	for (int i = 0; i < 3; i++)
	{
		Voltorb* voltorb = new Voltorb;
		voltorb->Init(80 - 35 * i, y);
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
	mMachop->Init(-30, y, 1);
	mMachop->SetTrail(temp);

	mAbra = new Abra;
	mAbra->Init(-70, y, 1);
	mAbra->SetTrail(temp);

}

void Stage3::InitJigglypuff()
{
	mJigglypuff = new Jigglypuff();
	mJigglypuff->Init(200, 730, false);
	vector<vector<Tile*>> tileList = *mTileMap->GetTileListPtr();
	mJigglypuff->SetTileListPtr(tileList);
	vector<vector<MapObject*>> mapObjectList = *mTileMap->GetObjectListPtr();
	mJigglypuff->SetMapObjectListPtr(mapObjectList);
}