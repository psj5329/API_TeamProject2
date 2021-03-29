#include "pch.h"
#include "Scene3.h"
//#include "Player.h"
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

void Scene3::Init()
{
	mTempPlayer = new Player("Player", TileSize * 5.5, TileSize * 8.5);
	OBJECTMANAGER->AddObject(ObjectLayer::PLAYER, mTempPlayer);
	OBJECTMANAGER->Init();

	mTileMap = new TileMap();
	//mTileMap->Init(TileCountX, TileCountY, TileSize);
	mTileMap->LoadMap(2);
	int x = mTileMap->GetXTileCount();
	int y = mTileMap->GetYTileCount();
	//플레이어 에게 타일숫자 전달

	mTempPlayer->SetTileCount(x, y);

	mTrailManager = new TrailManager();
	mTrailManager->Init(y,x);
	mTrailManager->PlaceTrail(7, 0, ItemType::Green, 3); // 0: down 1: up 2: left 3: right
	mTrailManager->PlaceTrail(7, 1, ItemType::Green, 3);
	mTrailManager->SetStartIndex(7, 0);


	mTrailManager->PlaceTrail(7, 2, ItemType::Green, 3);
	mTrailManager->PlaceTrail(7, 3, ItemType::Green, 3);
	mTrailManager->PlaceTrail(7, 4, ItemType::Green, 3);
	mTrailManager->PlaceTrail(7, 5, ItemType::Green, 1);
	mTrailManager->PlaceTrail(6, 5, ItemType::Green, 3);
	mTrailManager->PlaceTrail(6, 6, ItemType::Green, 3);
	mTrailManager->PlaceTrail(6, 7, ItemType::Green, 3);
	mTrailManager->PlaceTrail(6, 8, ItemType::Green, 3);

	//플레이어
	vector<vector<Tile*>>* tileListPtr = mTileMap->GetTileListPtr();
	mTempPlayer->SetTileListPtr(tileListPtr);
	vector<vector<MapObject*>>* mapObjectListPtr = mTileMap->GetObjectListPtr();
	mTempPlayer->SetMapObjectListPtr(mapObjectListPtr);

	mTempPlayer->SetTrailManagerPtr(mTrailManager);


	//푸린
	mJigglypuff = new Jigglypuff();
	mJigglypuff->Init(200, 400, false);
	vector<vector<Tile*>> tileList = *mTileMap->GetTileListPtr();
	mJigglypuff->SetTileListPtr(tileList);
	vector<vector<MapObject*>> mapObjectList = *mTileMap->GetObjectListPtr();
	mJigglypuff->SetMapObjectListPtr(mapObjectList);


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
	
void Scene3::Release()
{
	SafeDelete(mJigglypuff);
}

void Scene3::Update()
{
	//트레일 가지고놀아보기
	float x = CAMERAMANAGER->GetMainCamera()->GetX();
	float y = CAMERAMANAGER->GetMainCamera()->GetY();

	//설치
	if (Input::GetInstance()->GetKeyDown(VK_LBUTTON))
	{
		int indexX = (_mousePosition.x + (x - WINSIZEX / 2)) / TileSize;
		int indexY = (_mousePosition.y + (y - WINSIZEY / 2)) / TileSize;

		if (indexX >= 0 && indexX < mTrailManager->GetXTileCount() &&
			indexY >= 0 && indexY < mTrailManager->GetYTileCount())
		{
			//mTrailManager->PlaceTrail(indexY, indexX, ItemType::Green, 3);
			//mTrailManager->FindTail(&mTailY, &mTailX);
			mJigglypuff->PlaceMike(indexX, indexY);
			
		}
	}
	//돌리기
	if (Input::GetInstance()->GetKeyDown(VK_RBUTTON))
	{
		int indexX = (_mousePosition.x + (x - WINSIZEX / 2)) / TileSize;
		int indexY = (_mousePosition.y + (y - WINSIZEY / 2)) / TileSize;

		if (indexX >= 0 && indexX < mTrailManager->GetXTileCount() &&
			indexY >= 0 && indexY < mTrailManager->GetYTileCount())
		{
			//mTrailManager->TurnTrail(indexY, indexX);
			//mTrailManager->FindTail(&mTailY, &mTailX);
			mJigglypuff->TakeMike();
		}
	}	
	//빼기
	if (Input::GetInstance()->GetKeyDown(VK_BACK))
	{
		int indexX = (_mousePosition.x + (x - WINSIZEX / 2)) / TileSize;
		int indexY = (_mousePosition.y + (y - WINSIZEY / 2)) / TileSize;

		if (indexX >= 0 && indexX < mTrailManager->GetXTileCount() &&
			indexY >= 0 && indexY < mTrailManager->GetYTileCount())
		{
			mTrailManager->PickUpTrail(indexY, indexX);
			mTrailManager->FindTail(&mTailY, &mTailX);
		}
	}
	mTileMap->Update();
	mTrailManager->Update();
	OBJECTMANAGER->Update();
	CAMERAMANAGER->GetMainCamera()->Update();
}

void Scene3::Render(HDC hdc)
{

	//ULONG fps = Time::GetInstance()->GetmFrameRate();
	wstring tailY = L"tailY : " + to_wstring(mTailY);
	wstring tailX = L"tailX : " + to_wstring(mTailX);
	//wstring strFPS = L"FPS : " + to_wstring(fps);

	TextOut(hdc, 10, 55, tailY.c_str(), tailY.length());
	TextOut(hdc, 10, 70, tailX.c_str(), tailX.length());
	//TextOut(hdc, 10, 40, strFPS.c_str(), strFPS.length());

	mTileMap->Render(hdc);

	mTrailManager->Render(hdc);

	OBJECTMANAGER->Render(hdc);

	//wstring strScene = L"이건 3번 씬";
	//TextOut(hdc, WINSIZEX / 2 - 15, WINSIZEY / 2, strScene.c_str(), strScene.length());
}
