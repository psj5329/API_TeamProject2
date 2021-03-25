#include "pch.h"
#include "Scene4.h"
#include "Player.h"
#include "Camera.h"
#include "TileMap.h"
#include "TrailManager.h"
#include "Machop.h"

void Scene4::Init()
{
	// 플레이어도 사실 메인게임으로 가야 함
	mTempPlayer = new Player("Player", TileSize * 4.5, TileSize * 4.5);
	OBJECTMANAGER->AddObject(ObjectLayer::PLAYER, mTempPlayer);

	// 각 씬에서는 얘를 사용해야 함
	//Player* player = (Player*)(OBJECTMANAGER->FindObject("Player"));

	/* 얘는 메인으로 가야 함
	Camera* camera = new Camera();
	camera->SetX(100);
	camera->SetY(WINSIZEY / 2);
	camera->SetTarget(player);
	CAMERAMANAGER->SetMainCamera(camera);
	*/
	//OBJECTMANAGER->AddObject(ObjectLayer::PLAYER, camera);

	OBJECTMANAGER->Init();

	mTileMap = new TileMap();
	mTileMap->Init(TileCountX, TileCountY, TileSize);
	mTileMap->LoadMap();

	mTrailManager = new TrailManager();
	mTrailManager->Init(mTileMap->GetYTileCount(), mTileMap->GetXTileCount());
	mTrailManager->PlaceTrail(0, 5, ItemType::Green, 0); // 0: down 1: up 2: left 3: right
	mTrailManager->PlaceTrail(1, 5, ItemType::Green, 0);
	mTrailManager->SetStartIndex(0, 5);

	mTrailManager->PlaceTrail(2, 5, ItemType::Green, 0);
	mTrailManager->PlaceTrail(3, 5, ItemType::Green, 0);
				   
	mTrailManager->PlaceTrail(4, 5, ItemType::Green, 3);
	mTrailManager->PlaceTrail(4, 6, ItemType::Green, 3);
				   
	mTrailManager->PlaceTrail(4, 7, ItemType::Green, 0);
	mTrailManager->PlaceTrail(5, 7, ItemType::Green, 0);
	mTrailManager->PlaceTrail(6, 7, ItemType::Green, 0);
				   
	mTrailManager->PlaceTrail(7, 7, ItemType::Green, 2);
	mTrailManager->PlaceTrail(7, 6, ItemType::Green, 2);
	mTrailManager->PlaceTrail(7, 5, ItemType::Green, 2);
	mTrailManager->PlaceTrail(7, 4, ItemType::Green, 2);
				   
	mTrailManager->PlaceTrail(7, 3, ItemType::Green, 1);
	mTrailManager->PlaceTrail(6, 3, ItemType::Green, 1);
	mTrailManager->PlaceTrail(5, 3, ItemType::Green, 1);

//	Machop* machop = new Machop();
//	machop->Init();
//	machop->SetX(5 * 48 + 24);
//	machop->SetY(24);
//	OBJECTMANAGER->AddObject(ObjectLayer::TRAIN, machop);
//	vector<vector<Trail*>>* trailListPtr = mTrailManager->GetTrailListPtr();
//	machop->SetTrail(trailListPtr);

	vector<vector<Tile*>>* tileListPtr = mTileMap->GetTileListPtr();
	mTempPlayer->SetTileListPtr(tileListPtr);
	vector<vector<MapObject*>>* mapObjectListPtr = mTileMap->GetObjectListPtr();
	mTempPlayer->SetMapObjectListPtr(mapObjectListPtr);

	mTempPlayer->SetTrailManagerPtr(mTrailManager);
}

void Scene4::Release()
{
	mTileMap->Release();
	mTrailManager->Release();
	OBJECTMANAGER->Release();
}

void Scene4::Update()
{
	mTileMap->Update();
	mTrailManager->Update();
	//mTrailManager->FindTail(&mTailY, &mTailX);

	OBJECTMANAGER->Update();
}

void Scene4::Render(HDC hdc)
{
	mTileMap->Render(hdc);
	mTrailManager->Render(hdc);
	OBJECTMANAGER->Render(hdc);

	//wstring tailY = L"tailY : " + to_wstring(mTailY);
	//wstring tailX = L"tailX : " + to_wstring(mTailX);
	//wstring strFPS = L"FPS : " + to_wstring(fps);

	//TextOut(hdc, 10, 55, tailY.c_str(), tailY.length());
	//TextOut(hdc, 10, 70, tailX.c_str(), tailX.length());

	// {{ 완성본에서 지워야 할 내용 시작
	wstring strScene = L"이건 4번 씬 / 방향키: 이동 / 이동 중 shift: 대시 / C: 변신";
	TextOut(hdc, WINSIZEX / 2 - 15, 100, strScene.c_str(), (int)strScene.length());
	// 완성본에서 지워야 할 내용 끝 }}
}