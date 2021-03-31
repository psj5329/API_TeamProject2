#include "pch.h"
#include "Scene1.h"

#include "Image.h"
#include "Camera.h"

#include "Scene.h"

#include "TileMap.h"
#include "Camera.h"
#include "Player.h"
#include "Sableye.h"
#include "Button.h"
#include "TrailManager.h"
#include "Electrode.h"
#include "Voltorb.h"
#include "Trail.h"
#include "GameEvent.h"

void Scene1::Init()
{
	mTileMap = new TileMap();
	mTileMap->Init(mTileMap->GetXTileCount(), mTileMap->GetYTileCount(), TileSize);
	mTileMap->LoadMap(1); 

	mPlayer = new Player("Player_test", TileSize * 5.5, TileSize * 9.5);
	/*mPlayer->SetX(48 * 5);
	mPlayer->SetY(48 * 5);*/
	OBJECTMANAGER->AddObject(ObjectLayer::PLAYER, mPlayer);

	mPlayer->SetTileCount(mTileMap->GetXTileCount(), mTileMap->GetYTileCount());

	vector<vector<Tile*>>* tileListPtr = mTileMap->GetTileListPtr();
	mPlayer->SetTileListPtr(tileListPtr);
	vector<vector<MapObject*>>* mapObjectListPtr = mTileMap->GetObjectListPtr();
	mPlayer->SetMapObjectListPtr(mapObjectListPtr);

	mEnemy = new Sableye;
	mEnemy->Init();
	mEnemy->SetX(0);
	mEnemy->SetY(0);
	OBJECTMANAGER->AddObject(ObjectLayer::ENEMY, mEnemy);

	vector<vector<Tile*>> tileList = *mTileMap->GetTileListPtr();
	mEnemy->SetTileListPtr(tileList);
	vector<vector<MapObject*>> mapObjectList = *mTileMap->GetObjectListPtr();
	mEnemy->SetMapObjectListPtr(mapObjectList);

	Camera* main = CAMERAMANAGER->GetMainCamera();
	main->ChangeMode(Camera::Mode::Free);
	main->SetTarget(mPlayer);
	main->SetX(WINSIZEX / 2);
	main->SetY(WINSIZEY / 2);
	CAMERAMANAGER->SetMainCamera(main);

	OBJECTMANAGER->Init();

	vector<GameObject*> object = OBJECTMANAGER->GetObjectList(ObjectLayer::ENEMY);
	for (int i = 0; i < object.size(); ++i)
		dynamic_cast<Enemy*>(object[i])->SetTileList(*mTileMap->GetTileListPtr());

	mTrailManager = new TrailManager();
	mTrailManager->Init(mTileMap->GetYTileCount(), mTileMap->GetXTileCount());
	mTrailManager->PlaceTrail(10, 5, ItemType::Green, 3); // 0: down 1: up 2: left 3: right
	mTrailManager->PlaceTrail(10, 4, ItemType::Green, 3);
	mTrailManager->PlaceTrail(10, 7, ItemType::Green, 1);
	//mTrailManager->PlaceTrail(12, 24, ItemType::Green, 1);
	mTrailManager->SetStartIndex(10, 5);
	vector <vector <Trail*>>* temp = mTrailManager->GetTrailListPtr();
	(*temp)[10][7]->SetisFinish(true);



	//푸린
	mJigglypuff = new Jigglypuff;
	mJigglypuff->Init();
	mJigglypuff->SetX(200);
	mJigglypuff->SetY(WINSIZEY / 2);

	// 열차
	mElectrode = new Electrode;
	mElectrode->Init(TileSize * 5.5, TileSize * 10.5);
	mElectrode->SetTrail(temp);
	mElectrode->SetJigglypuff(mJigglypuff);

	mVoltorb = new Voltorb;
	mVoltorb->Init(TileSize * 4.5, TileSize * 10.5);
	mVoltorb->SetTrail(temp);
	mVoltorb->SetJigglypuff(mJigglypuff);

	mElectrode->SetNextTrain(mVoltorb);

	WindowInit();		// 일시정지 창 이닛
}

void Scene1::Release()
{
	//OBJECTMANAGER->Release();
	OBJECTMANAGER->ReleaseInScene();	// 플레이어, 아이템 제외 지우기
}

void Scene1::Update()
{
	if (INPUT->GetKeyDown(VK_SPACE))
	{
		GameObject* item = COLLISIONMANAGER->ItemCollision(mPlayer->GetColBoxPtr());
		if (item != nullptr)
			item->SetIsActive(false);
	}

	if (INPUT->GetKeyDown(VK_ESCAPE))
		mIsPause = !mIsPause;

	if (INPUT->GetKeyDown(MK_LBUTTON))
	{
		int x = _mousePosition.x / TileSize;
		int y = _mousePosition.y / TileSize;

		mTrailManager->PlaceTrail(y, x, ItemType::Green, 0);
	}

	if (INPUT->GetKeyDown(MK_RBUTTON))
	{
		int x = _mousePosition.x / TileSize;
		int y = _mousePosition.y / TileSize;

		vector<vector<Trail*>>* trailListPtr = mTrailManager->GetTrailListPtr();
		Trail* currentTrail = (*trailListPtr)[y][x];

		if (currentTrail->GetTrailType() == ItemType::None)
			return;

		mTrailManager->TurnTrail(y, x);
	}

	vector<vector<Trail*>>* trailListPtr = mTrailManager->GetTrailListPtr();
	int x = mElectrode->GetX() / TileSize;
	int y = mElectrode->GetY() / TileSize;
	
	Trail* currentTrail = (*trailListPtr)[y][x];
	if (currentTrail->GetisFinish())
		mIsClear = true;
	else if (!mElectrode->CheckNextTrailType())
		mIsGameOver = true;

	if (mIsPause && !mIsOption)
	{
		if (mContinueButton != nullptr)
			mContinueButton->Update();
		if (mOptionButton != nullptr)
			mOptionButton->Update();
		if (mMainButton != nullptr)
			mMainButton->Update();
	}

	if (mIsPause)
	{
		if (mXButton != nullptr)
			mXButton->Update();
		if (mVolumeEffectBar != nullptr)
		{
			mVolumeEffectBar->Update();
			mVolumeEffectButton->Update();
		}
		if (mVolumeEffectButton != nullptr)
			mVolumeEffectButton->Update();
		if (mVolumeBackgroundBar != nullptr)
		{
			mVolumeBackgroundBar->Update();
			mVolumeBackgroundButton->Update();
		}
		if (mVolumeBackgroundButton != nullptr)
			mVolumeBackgroundButton->Update();
	}

	if (!mIsPause && !mIsClear && !mIsGameOver)
	{
		OBJECTMANAGER->Update();
		mTileMap->Update();
		POINT tilecount = mPlayer->GetTileCount();
		mTrailManager->Update();
		COLLISIONMANAGER->TileCollision(mPlayer, mTileMap, tilecount.x, tilecount.y);
		COLLISIONMANAGER->MapObjectCollision(mPlayer, mTileMap, tilecount.x, tilecount.y);
		CAMERAMANAGER->Update();
	}

	if (mIsClear)
	{
		if (!mIsClearFont)
		{
			mIsClearFont = true;
			GAMEEVENTMANAGER->PushEvent(new IClearStage());
		}
		else
		{
			if (GAMEEVENTMANAGER->GetEventCount() == 0)
				SCENEMANAGER->LoadScene(L"SceneSelect");
		}
	}

	if (mIsGameOver)
	{
		if (!mIsGameOverFont)
		{
			mIsGameOverFont = true;
			GAMEEVENTMANAGER->PushEvent(new IGameoverStage());
		}
		else
		{
			if (GAMEEVENTMANAGER->GetEventCount() == 0)
				SCENEMANAGER->LoadScene(L"SceneSelect");
		}
	}

	GAMEEVENTMANAGER->Update();
}

void Scene1::Render(HDC hdc)
{
	mTileMap->Render(hdc);
	mTrailManager->Render(hdc);
	OBJECTMANAGER->Render(hdc);

	if (mIsPause)
	{
		if (!mIsOption)
		{
			CAMERAMANAGER->GetMainCamera()->ScaleRender(hdc, mPauseWindow, WINSIZEX / 2 - mPauseWindow->GetWidth() / 2, WINSIZEY / 2 - mPauseWindow->GetHeight() / 2, mPauseWindow->GetWidth(), mPauseWindow->GetHeight());
			mContinueButton->ScaleRender(hdc, 0.9f, 0.9f);
			mOptionButton->ScaleRender(hdc, 0.9f, 0.9f);
			mMainButton->ScaleRender(hdc, 0.9f, 0.9f);
		}
		else
		{
			CAMERAMANAGER->GetMainCamera()->ScaleRender(hdc, mPauseOptionWindow, WINSIZEX / 2 - mPauseWindow->GetWidth() / 2, WINSIZEY / 2 - mPauseWindow->GetHeight() / 2, mPauseWindow->GetWidth(), mPauseWindow->GetHeight());
			mXButton->ScaleRender(hdc, 0.9f, 0.9f);
			mVolumeEffectBar->Render(hdc);
			mVolumeEffectButton->Render(hdc);
			mVolumeBackgroundBar->Render(hdc);
			mVolumeBackgroundButton->Render(hdc);
		}
	}

	GAMEEVENTMANAGER->Render(hdc);
}

