#include "pch.h"
#include "Scene3.h"
//#include "Player.h"
#include "TileMap.h"
#include "MapToolScene.h"
#include "Camera.h"

void Scene3::Init()
{
	//mPlayer = new Player();
	//mPlayer->Init();

	mTileMap = new TileMap();
	//mTileMap->Init(TileCountX, TileCountY, TileSize);
	mTileMap->LoadMap(3);
}

void Scene3::Release()
{
}

void Scene3::Update()
{
	mTileMap->Update();

	if (Input::GetInstance()->GetKeyDown(VK_SPACE))
	{
		mTileMap->LoadMap();
	}

	OBJECTMANAGER->Update();
	CAMERAMANAGER->GetMainCamera()->Update();

}

void Scene3::Render(HDC hdc)
{
	float worldTime = Time::GetInstance()->GetWorldTime();
	float deltaTime = Time::GetInstance()->DeltaTime();
	ULONG fps = Time::GetInstance()->GetmFrameRate();
	wstring strWorldTime = L"WorldTime : " + to_wstring(worldTime);
	wstring strDeltaTime = L"DeltaTime : " + to_wstring(deltaTime);
	wstring strFPS = L"FPS : " + to_wstring(fps);

	TextOut(hdc, 10, 10, strWorldTime.c_str(), strWorldTime.length());
	TextOut(hdc, 10, 25, strDeltaTime.c_str(), strDeltaTime.length());
	TextOut(hdc, 10, 40, strFPS.c_str(), strFPS.length());

	mTileMap->Render(hdc);

	OBJECTMANAGER->Render(hdc);


	wstring str3 = L"SPACE ¸Ê·Îµå";
	TextOut(hdc, _mousePosition.x + 10, _mousePosition.y + 40, str3.c_str(), str3.length());



	//wstring strScene = L"ÀÌ°Ç 3¹ø ¾À";
	//TextOut(hdc, WINSIZEX / 2 - 15, WINSIZEY / 2, strScene.c_str(), strScene.length());
}
