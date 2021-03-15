#include "pch.h"
#include "Scene4.h"

#include "Player.h"
#include "Camera.h"

#include "TileMap.h"

void Scene4::Init()
{
	/*
	IMAGEMANAGER->LoadFromFile(L"charmander", Resources(L"charmander"), 192, 1968, 4, 41, true);
	IMAGEMANAGER->LoadFromFile(L"chikorita", Resources(L"chikorita"), 192, 1968, 4, 41, true);
	IMAGEMANAGER->LoadFromFile(L"ditto", Resources(L"ditto"), 144, 1200, 3, 25, true);
	IMAGEMANAGER->LoadFromFile(L"jigglypuff", Resources(L"jigglypuff"), 192, 816, 4, 17, true);
	IMAGEMANAGER->LoadFromFile(L"lapras", Resources(L"lapras"), 192, 624, 4, 13, true);
	IMAGEMANAGER->LoadFromFile(L"totodile", Resources(L"totodile"), 240, 1968, 5, 41, true);
	*/

	// 플레이어도 사실 메인게임으로 가야 함
	Player* player = new Player("Player", WINSIZEX / 2, WINSIZEY / 2);
	OBJECTMANAGER->AddObject(ObjectLayer::PLAYER, player);

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
}

void Scene4::Release()
{
	OBJECTMANAGER->Release();
}

void Scene4::Update()
{
	OBJECTMANAGER->Update();
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