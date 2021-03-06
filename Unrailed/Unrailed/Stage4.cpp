#include "pch.h"
#include "Stage4.h"
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
#include "Sableye.h"

void Stage4::Init()
{
	mTileMap = new TileMap();
	mTileMap->LoadMap(4);
	int x = mTileMap->GetXTileCount();
	int y = mTileMap->GetYTileCount();

	mTrailManager = new TrailManager();
	mTrailManager->Init(y, x);

	mSizeX = x * TileSize;
	mSizeY = y * TileSize;

	//플레이어 설정
	OBJECTMANAGER->GetPlayer()->SetX(9.5 * TileSize);
	OBJECTMANAGER->GetPlayer()->SetY(11.5 * TileSize);
	OBJECTMANAGER->GetPlayer()->SetTileCount(x, y);
	OBJECTMANAGER->GetPlayer()->SetTileListPtr(mTileMap->GetTileListPtr());
	OBJECTMANAGER->GetPlayer()->SetMapObjectListPtr(mTileMap->GetObjectListPtr());
	OBJECTMANAGER->GetPlayer()->SetTrailManagerPtr(mTrailManager);

	Sableye* sableye1 = new Sableye;
	sableye1->Init();
	sableye1->SetX(17.5 * TileSize);
	sableye1->SetY(9.5 * TileSize);
	OBJECTMANAGER->AddObject(ObjectLayer::ENEMY, sableye1);

	Sableye* sableye2 = new Sableye;
	sableye2->Init();
	sableye2->SetX(30.5 * TileSize);
	sableye2->SetY(7.5 * TileSize);
	OBJECTMANAGER->AddObject(ObjectLayer::ENEMY, sableye2);

	vector<GameObject*> object = OBJECTMANAGER->GetObjectList(ObjectLayer::ENEMY);
	for (int i = 0; i < object.size(); ++i)
	{
		dynamic_cast<Enemy*>(object[i])->SetTileList(*mTileMap->GetTileListPtr());
		dynamic_cast<Enemy*>(object[i])->SetMapObjectListPtr(*mTileMap->GetObjectListPtr());
	}

	PlaceTrail();
	InitJigglypuff();
	InitTrain();
	WindowInit();		// 일시정지 창 이닛

	Camera* camera = CAMERAMANAGER->GetMainCamera();
	camera->ChangeMode(Camera::Mode::Follow);
	camera->SetTarget(mElectrode);

	SOUNDMANAGER->Play(L"Steampipe Sonata", _BackgroundSound);
}

void Stage4::Release()
{
	OBJECTMANAGER->GetPlayer()->EmptyInven();
	OBJECTMANAGER->ReleaseInScene();
	mJigglypuff = NULL;
	SOUNDMANAGER->Stop(L"Steampipe Sonata");
}

void Stage4::Update()
{
	SOUNDMANAGER->Update();
	if (SOUNDMANAGER->GetPosition(L"Steampipe Sonata") >= SOUNDMANAGER->GetWholePosition(L"Steampipe Sonata"))
	{
		SOUNDMANAGER->Stop(L"Steampipe Sonata");
		SOUNDMANAGER->Play(L"Steampipe Sonata", _BackgroundSound);
	}

	if (INPUT->GetKeyDown(VK_ESCAPE))
		mIsPause = !mIsPause;

	vector<vector<Trail*>>* trailListPtr = mTrailManager->GetTrailListPtr();
	int x = mElectrode->GetX() / TileSize;
	int y = mElectrode->GetY() / TileSize;

	Trail* currentTrail = (*trailListPtr)[y][x];
	if (currentTrail->GetisFinish())
		mIsClear = true;
	else if (!mVoltorbVec.back()->GetIsActive())
		mIsGameOver = true;

	if (!mIsPause && !mIsClear && !mIsGameOver)
	{
		mTileMap->Update();
		mTrailManager->Update();
		OBJECTMANAGER->Update();
		CAMERAMANAGER->GetMainCamera()->Update();
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

void Stage4::Render(HDC hdc)
{
	mTileMap->Render(hdc);
	mTrailManager->Render(hdc);
	OBJECTMANAGER->Render(hdc);

	if (mIsPause)
	{
		if (!mIsOption)
		{
			mPauseWindow->ScaleRender(hdc, WINSIZEX / 2 - mPauseWindow->GetWidth() / 2, WINSIZEY / 2 - mPauseWindow->GetHeight() / 2, mPauseWindow->GetWidth(), mPauseWindow->GetHeight());
			mContinueButton->ScaleRender(hdc, 0.9f, 0.9f);
			mOptionButton->ScaleRender(hdc, 0.9f, 0.9f);
			mMainButton->ScaleRender(hdc, 0.9f, 0.9f);
		}
		else
		{
			mPauseOptionWindow->ScaleRender(hdc, WINSIZEX / 2 - mPauseWindow->GetWidth() / 2, WINSIZEY / 2 - mPauseWindow->GetHeight() / 2, mPauseWindow->GetWidth(), mPauseWindow->GetHeight());
			mXButton->ScaleRender(hdc, 0.9f, 0.9f);
			mVolumeEffectBar->Render(hdc);
			mVolumeEffectButton->Render(hdc);
			mVolumeBackgroundBar->Render(hdc);
			mVolumeBackgroundButton->Render(hdc);
		}
	}

	GAMEEVENTMANAGER->Render(hdc);
}

void Stage4::PlaceTrail()
{

	mTrailManager->PlaceTrail(12, 0, ItemType::Green, 3); // 0: down 1: up 2: left 3: right
	mTrailManager->PlaceTrail(12, 1, ItemType::Green, 3);
	mTrailManager->SetStartIndex(12, 0);
	mTrailManager->PlaceTrail(12, 2, ItemType::Green, 3);
	mTrailManager->PlaceTrail(12, 3, ItemType::Green, 3);
	mTrailManager->PlaceTrail(12, 4, ItemType::Green, 3);
	mTrailManager->PlaceTrail(12, 5, ItemType::Green, 3);
	mTrailManager->PlaceTrail(12, 6, ItemType::Green, 3);
	mTrailManager->PlaceTrail(12, 7, ItemType::Green, 3);
	mTrailManager->PlaceTrail(12, 8, ItemType::Green, 3);
	mTrailManager->PlaceTrail(12, 9, ItemType::Green, 3);
	mTrailManager->PlaceTrail(12, 10, ItemType::Green, 3);


	mTrailManager->PlaceTrail(10, 43, ItemType::Green, 1);
	mTrailManager->SetEndIndex(10, 43);
}

void Stage4::InitTrain()
{
	//열차
	int y = 600;
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

void Stage4::InitJigglypuff()
{
	mJigglypuff = new Jigglypuff();
	mJigglypuff->Init(200, 650, false);
	vector<vector<Tile*>> tileList = *mTileMap->GetTileListPtr();
	mJigglypuff->SetTileListPtr(tileList);
	vector<vector<MapObject*>> mapObjectList = *mTileMap->GetObjectListPtr();
	mJigglypuff->SetMapObjectListPtr(mapObjectList);
}