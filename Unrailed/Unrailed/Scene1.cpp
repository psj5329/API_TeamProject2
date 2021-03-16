#include "pch.h"
#include "Scene1.h"

#include "Image.h"
#include "Camera.h"

#include "Scene.h"

#include "TileMap.h"
#include "Player.h"

void Scene1::Init()
{
	//IMAGEMANAGER->LoadFromFile(L"Stone", Resources(L"Item/Stone"), 39, 12, 3, 1, true);
	//mImage = IMAGEMANAGER->FindImage(L"Stone");

	mTileMap = new TileMap();
	mTileMap->Init(TileCountX, TileCountY, TileSize);
	mTileMap->LoadMap();

	IMAGEMANAGER->LoadFromFile(L"ditto", Resources(L"ditto"), 144, 1200, 3, 25, true);
	mPlayer = new Player("Player_test", 0, 0);
	mPlayer->SetX(48 * 2);
	mPlayer->SetY(48*5);
	OBJECTMANAGER->AddObject(ObjectLayer::PLAYER, mPlayer);

	OBJECTMANAGER->Init();
}

void Scene1::Release()
{
	OBJECTMANAGER->Release();
}

void Scene1::Update()
{
	if (INPUT->GetKeyDown(VK_SPACE))
		SCENEMANAGER->LoadScene(L"Scene2");

	OBJECTMANAGER->Update();
	mTileMap->Update();
	COLLISIONMANAGER->TileCollision(mPlayer, mTileMap);
	COLLISIONMANAGER->MapObjectCollision(mPlayer, mTileMap);
}

void Scene1::Render(HDC hdc)
{
	float worldTime = TIME->GetWorldTime();
	float deltaTime = TIME->DeltaTime();
	ULONG fps = TIME->GetmFrameRate();
	wstring strWorldTime = L"WorldTime : " + to_wstring(worldTime);
	wstring strDeltaTime = L"DeltaTime : " + to_wstring(deltaTime);
	wstring strFPS = L"FPS : " + to_wstring(fps);

	TextOut(hdc, 10, 10, strWorldTime.c_str(), strWorldTime.length());
	TextOut(hdc, 10, 25, strDeltaTime.c_str(), strDeltaTime.length());
	TextOut(hdc, 10, 40, strFPS.c_str(), strFPS.length());

	mTileMap->Render(hdc);
	OBJECTMANAGER->Render(hdc);

	wstring strScene = L"이건 1번 씬, 후에 메인 메뉴가 될거야";
	TextOut(hdc, WINSIZEX / 2 - 150, WINSIZEY / 2, strScene.c_str(), strScene.length());
	wstring strScene2 = L"이 씬에서 게임 시작을 누를 시 로딩한다";
	TextOut(hdc, WINSIZEX / 2 - 150, WINSIZEY / 2 + 20, strScene2.c_str(), strScene2.length());

	/*if (on)
	{
		name = IMAGEMANAGER->GetKeyName(mImage);
		TextOut(hdc, WINSIZEX / 2 - 15, WINSIZEY / 2 - 20, name.c_str(), name.length());
	}*/
}

void Scene1::LoadResources()
{
}
