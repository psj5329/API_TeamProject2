#include "pch.h"
#include "Scene2.h"
#include "TileMap.h"

void Scene2::Init()
{
	mTileMap = new TileMap();
	mTileMap->Init(TileCountX, TileCountY, TileSize);
	mTileMap->LoadMap();
  
	for (int i = 0; i < 3; i++)
	{
		Voltorb* temp = new Voltorb;
		temp->Init();
		temp->SetX(WINSIZEX / 2 - 50 * i);
		mVoltorbVec.push_back(temp);

		OBJECTMANAGER->AddObject(ObjectLayer::ITEM, mVoltorbVec[i]);
	}

	mVoltorbVec[0]->SetImage(0);
	mVoltorbVec[1]->SetImage(1);
	mVoltorbVec[2]->SetImage(1);

	mVoltorbVec[0]->SetY(WINSIZEY / 2 - 10);

	mMachop = new Machop;
	mMachop->Init();

	mAbra = new Abra;
	mAbra->Init();

	//기차 레이어 추가 요망
	OBJECTMANAGER->AddObject(ObjectLayer::ITEM, mAbra);
	OBJECTMANAGER->AddObject(ObjectLayer::ITEM, mMachop);
}

void Scene2::Release()
{
}

void Scene2::Update()
{
	OBJECTMANAGER->Update();
}

void Scene2::Render(HDC hdc)
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


	wstring strScene = L"이건 2번 씬";
	TextOut(hdc, WINSIZEX / 2 - 15, WINSIZEY / 2, strScene.c_str(), strScene.length());



	OBJECTMANAGER->Render(hdc);
}
