#include "pch.h"
#include "Scene4.h"

#include "Player.h"
#include "Camera.h"

#include "TileMap.h"

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

	vector<vector<Tile*>>* tileListPtr = mTileMap->GetTileListPtr();
	mTempPlayer->SetTileListPtr(tileListPtr);
	vector<vector<MapObject*>>* mapObjectListPtr = mTileMap->GetObjectListPtr();
	mTempPlayer->SetMapObjectListPtr(mapObjectListPtr);
}

void Scene4::Release()
{
	OBJECTMANAGER->Release();
}

void Scene4::Update()
{
	RECT* playerColBoxPtr = mTempPlayer->GetColBoxPtr();
	OBJECTMANAGER->Update();
	mTileMap->Update();
	COLLISIONMANAGER->TileCollision(mTempPlayer, playerColBoxPtr, mTileMap);
	COLLISIONMANAGER->MapObjectCollision(mTempPlayer, playerColBoxPtr, mTileMap);
}

void Scene4::Render(HDC hdc)
{
	mTileMap->Render(hdc);
	OBJECTMANAGER->Render(hdc);

	// {{ 완성본에서 지워야 할 내용 시작
	wstring strScene = L"이건 4번 씬 / 방향키: 이동 / 이동 중 shift: 대시 / 8, 9, 0, 7: 변신";
	TextOut(hdc, WINSIZEX / 2 - 15, 100, strScene.c_str(), strScene.length());
	// 완성본에서 지워야 할 내용 끝 }}
}