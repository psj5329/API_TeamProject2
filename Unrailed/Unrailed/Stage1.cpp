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
	mTileMap->LoadMap(2);
	int x = mTileMap->GetXTileCount();
	int y = mTileMap->GetYTileCount();

	mTrailManager = new TrailManager();
	mTrailManager->Init(y, x);

	mSizeX = x * TileSize;
	mSizeY = y * TileSize;

	//플레이어 설정
	OBJECTMANAGER->GetPlayer()->SetX(5.5 * TileSize);
	OBJECTMANAGER->GetPlayer()->SetY(9.5 * TileSize);
	OBJECTMANAGER->GetPlayer()->SetTileCount(x, y);
	OBJECTMANAGER->GetPlayer()->SetTileListPtr(mTileMap->GetTileListPtr());
	OBJECTMANAGER->GetPlayer()->SetMapObjectListPtr(mTileMap->GetObjectListPtr());
	OBJECTMANAGER->GetPlayer()->SetTrailManagerPtr(mTrailManager);

	Camera* camera = CAMERAMANAGER->GetMainCamera();
	camera->ChangeMode(Camera::Mode::Follow);
	camera->SetTarget(OBJECTMANAGER->GetPlayer());		// 1스테는 기차가 업서서

	PlaceTrail();
	InitJigglypuff();
	InitTrain();
	WindowInit();		// 일시정지 창 이닛

	SOUNDMANAGER->Play(L"The First Track", 0.2f);
}

void Stage1::Release()
{
	OBJECTMANAGER->ReleaseInScene();
	SOUNDMANAGER->Stop(L"The First Track");
}

void Stage1::Update()
{
	SOUNDMANAGER->Update();
	if (SOUNDMANAGER->GetPosition(L"The First Track") >= SOUNDMANAGER->GetWholePosition(L"Roll Mart"))
	{
		SOUNDMANAGER->Stop(L"The First Track");
		SOUNDMANAGER->Play(L"The First Track", 0.2f);
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
		mTileMap->Update();
		mTrailManager->Update();
		OBJECTMANAGER->Update();
		CAMERAMANAGER->GetMainCamera()->Update();
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

void Stage1::Render(HDC hdc)
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

void Stage1::PlaceTrail()
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

void Stage1::InitTrain()
{
	//노기차

}

void Stage1::InitJigglypuff()
{
	//노푸린
}