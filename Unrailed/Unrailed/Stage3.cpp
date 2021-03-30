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
	OBJECTMANAGER->GetPlayer()->SetX(9.5 * TileSize);
	OBJECTMANAGER->GetPlayer()->SetY(11.5 * TileSize);
	OBJECTMANAGER->GetPlayer()->SetTileCount(x, y);
	OBJECTMANAGER->GetPlayer()->SetTileListPtr(mTileMap->GetTileListPtr());
	OBJECTMANAGER->GetPlayer()->SetMapObjectListPtr(mTileMap->GetObjectListPtr());
	OBJECTMANAGER->GetPlayer()->SetTrailManagerPtr(mTrailManager);

	PlaceTrail();
	InitJigglypuff();
	InitTrain();
	WindowInit();		// 일시정지 창 이닛

	Camera* camera = CAMERAMANAGER->GetMainCamera();
	camera->ChangeMode(Camera::Mode::Follow);
	camera->SetTarget(mElectrode);

	SOUNDMANAGER->Play(L"Blazing Beats", 0.2f);
}

void Stage3::Release()
{
	OBJECTMANAGER->GetPlayer()->EmptyInven();
	OBJECTMANAGER->ReleaseInScene();
	mJigglypuff = NULL;
	SOUNDMANAGER->Stop(L"Blazing Beats");
}

void Stage3::Update()
{
	SOUNDMANAGER->Update();
	if (SOUNDMANAGER->GetPosition(L"Blazing Beats") >= SOUNDMANAGER->GetWholePosition(L"Roll Mart"))
	{
		SOUNDMANAGER->Stop(L"Blazing Beats");
		SOUNDMANAGER->Play(L"Blazing Beats", 0.2f);
	}

	if (INPUT->GetKeyDown(VK_ESCAPE))
		mIsPause = !mIsPause;

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

void Stage3::Render(HDC hdc)
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