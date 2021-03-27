#include "pch.h"
#include "Scene3.h"
//#include "Player.h"
#include "TileMap.h"
#include "MapToolScene.h"
#include "Camera.h"
#include "TrailManager.h"
#include "Image.h" //Hut빼면 빼도 ㄱㅊ

void Scene3::Init()
{
	//mPlayer = new Player();
	//mPlayer->Init();

	mTileMap = new TileMap();
	//mTileMap->Init(TileCountX, TileCountY, TileSize);
	mTileMap->LoadMap(3);
	int x = mTileMap->GetXTileCount();
	int y = mTileMap->GetYTileCount();

	mTrailManager = new TrailManager();
	mTrailManager->Init(y,x);
	mTrailManager->InsertTrail(6, 0, ItemType::Green, 3);
	mTrailManager->InsertTrail(6, 1, ItemType::Green, 3);
	mTrailManager->SetStartIndex(6, 0);

	mImage = IMAGEMANAGER->FindImage(L"Hut");
	//mImage = IMAGEMANAGER->FindImage(L"ChikoritaHut");
	//mImage = IMAGEMANAGER->FindImage(L"CharmanderHut");
}
	
void Scene3::Release()
{
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
			mTrailManager->PlaceTrail(indexY, indexX, ItemType::Green, 3);
			mTrailManager->FindTail(&mTailY, &mTailX);
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
			mTrailManager->TurnTrail(indexY, indexX);
			mTrailManager->FindTail(&mTailY, &mTailX);
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

	mImage->ScaleRender(hdc, WINSIZEX / 2, WINSIZEY / 2, mImage->GetFrameWidth()*2, mImage->GetFrameHeight()*2);
	//wstring strScene = L"이건 3번 씬";
	//TextOut(hdc, WINSIZEX / 2 - 15, WINSIZEY / 2, strScene.c_str(), strScene.length());
}
