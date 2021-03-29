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

void Scene1::Init()
{
	mTileMap = new TileMap();
	mTileMap->Init(TileCountX, TileCountY, TileSize);
	mTileMap->LoadMap(); 

	mPlayer = new Player("Player_test", 0, 0);
	mPlayer->SetX(48 * 2);
	mPlayer->SetY(48 * 5);
	OBJECTMANAGER->AddObject(ObjectLayer::PLAYER, mPlayer);

	mPlayer->SetTileCount(TileCountX, TileCountY);

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

	if (!mIsPause)
	{
		OBJECTMANAGER->Update();
		mTileMap->Update();
		COLLISIONMANAGER->TileCollision(mPlayer, mTileMap);
		COLLISIONMANAGER->MapObjectCollision(mPlayer, mTileMap);
		CAMERAMANAGER->Update();
	}
}

void Scene1::Render(HDC hdc)
{
	mTileMap->Render(hdc);
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
}

