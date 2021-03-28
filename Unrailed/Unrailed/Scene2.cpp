#include "pch.h"
#include "Scene2.h"
#include "TrailManager.h"
#include "TileMap.h"

void Scene2::Init()
{
	mTileMap = new TileMap();
	mTileMap->Init(TileCountX, TileCountY, TileSize);
	mTileMap->LoadMap();
	//지금 밟고 있는 타일 방향 확인
	//그 다음타일의 중간까지 타일 방향대로 이동
	//반복

	//인덱스 가져오기 : 현재 x값 나누기 타일사이즈
	mTrailManager = new TrailManager();
	mTrailManager->Init(mTileMap->GetYTileCount(), mTileMap->GetXTileCount());
	mTrailManager->InsertTrail(6, 0, ItemType::Green, 3);
	mTrailManager->InsertTrail(6, 1, ItemType::Green, 3);
	mTrailManager->InsertTrail(6, 2, ItemType::Green, 3);
	mTrailManager->InsertTrail(6, 3, ItemType::Green, 3);
	mTrailManager->InsertTrail(6, 4, ItemType::Green, 1);
	//mTrailManager->InsertTrail(5, 4, ItemType::Green, 0);
	mTrailManager->InsertTrail(4, 4, ItemType::Green, 3);
	mTrailManager->InsertTrail(4, 5, ItemType::Green, 3);
	mTrailManager->InsertTrail(4, 6, ItemType::Green, 3);
	mTrailManager->InsertTrail(4, 7, ItemType::Green, 0);
	mTrailManager->InsertTrail(5, 7, ItemType::Green, 0);
	mTrailManager->InsertTrail(6, 7, ItemType::Green, 0);
	mTrailManager->InsertTrail(7, 7, ItemType::Green, 2);
	mTrailManager->InsertTrail(7, 6, ItemType::Green, 2);
	mTrailManager->InsertTrail(7, 5, ItemType::Green, 2);
	mTrailManager->InsertTrail(7, 4, ItemType::Green, 1);


	//열차
	vector <vector <Trail*>>* temp = mTrailManager->GetTrailListPtr();

	mElectrode = new Electrode;
	mElectrode->Init(120, WINSIZEY / 2 - 40);
	mElectrode->SetTrail(temp);

	for (int i = 0; i < 3; i++)
	{
		Voltorb* voltorb = new Voltorb;
		voltorb->Init(100 - 50 * i, WINSIZEY / 2 - 40);
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
		}
		else
		{
			mVoltorbVec[i]->SetNextTrain(nullptr);
		}
	}

	mMachop = new Machop;
	mMachop->Init(-35, WINSIZEY / 2 - 40, 1);
	mMachop->SetTrail(temp);

	mAbra = new Abra;
	mAbra->Init(-85, WINSIZEY / 2 - 40, 1);
	mAbra->SetTrail(temp);
	
}

void Scene2::Release()
{
	mTrailManager->Release();
	mTileMap->Release();
}

void Scene2::Update()
{
	mTileMap->Update();
	mTrailManager->Update();

	OBJECTMANAGER->Update();
}

void Scene2::Render(HDC hdc)
{
	mTileMap->Render(hdc);
	mTrailManager->Render(hdc);

	OBJECTMANAGER->Render(hdc);
}