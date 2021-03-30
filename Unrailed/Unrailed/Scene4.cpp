#include "pch.h"
#include "Scene4.h"
#include "Player.h"
#include "Camera.h"
#include "TileMap.h"
#include "TrailManager.h"

#include "Electrode.h"
#include "Machop.h"
#include "Abra.h"
#include "Sableye.h"

void Scene4::Init()
{
	// �÷��̾ ��� ���ΰ������� ���� ��
	mTempPlayer = new Player("Player", TileSize * 4.5, TileSize * 9.5);
	OBJECTMANAGER->AddObject(ObjectLayer::PLAYER, mTempPlayer);

	// �� �������� �긦 ����ؾ� ��
	//Player* player = (Player*)(OBJECTMANAGER->FindObject("Player"));

	/* ��� �������� ���� ��
	Camera* camera = new Camera();
	camera->SetX(100);
	camera->SetY(WINSIZEY / 2);
	camera->SetTarget(player);
	CAMERAMANAGER->SetMainCamera(camera);
	*/
	//OBJECTMANAGER->AddObject(ObjectLayer::PLAYER, camera);

	OBJECTMANAGER->Init();

	mTileMap = new TileMap();
	//mTileMap->Init(TileCountX, TileCountY, TileSize);
	//mTileMap->LoadMap();
	mTileMap->LoadMap(5);
	mTempPlayer->SetTileCount(mTileMap->GetXTileCount(), mTileMap->GetYTileCount()); // �� �Լ� �Ű������� Ÿ�� ���� �־��ָ� ��

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

	vector<vector<Tile*>>* tileListPtr = mTileMap->GetTileListPtr();
	mTempPlayer->SetTileListPtr(tileListPtr);
	vector<vector<MapObject*>>* mapObjectListPtr = mTileMap->GetObjectListPtr();
	mTempPlayer->SetMapObjectListPtr(mapObjectListPtr);

	mTempPlayer->SetTrailManagerPtr(mTrailManager);

	Jigglypuff* jigglypuff = new Jigglypuff();
	jigglypuff->Init(TileSize * 4.5, TileSize * 10.5, false);
	jigglypuff->SetTileListPtr(*tileListPtr);
	jigglypuff->SetMapObjectListPtr(*mapObjectListPtr);

	vector<vector<Trail*>>* trailListPtr = mTrailManager->GetTrailListPtr();
	/*
	Electrode* electrode = new Electrode();
	electrode->Init(5 * 48 + 24, 72);
	electrode->SetTrail(trailListPtr);
	electrode->SetJigglypuff(jigglypuff);

	Machop* machop = new Machop();
	machop->Init(5 * 48 + 24, 24, 1);
	machop->SetTrail(trailListPtr);

	Abra* abra = new Abra();
	abra->Init(5 * 48 + 24, -24, 1);
	abra->SetTrail(trailListPtr);*/

	Sableye* enemy = new Sableye();
	enemy->Init();
	enemy->SetX(12.5 * TileSize);
	enemy->SetY(2.5 * TileSize);
	OBJECTMANAGER->AddObject(ObjectLayer::ENEMY, enemy);
	enemy->SetTileListPtr(*tileListPtr);
	enemy->SetMapObjectListPtr(*mapObjectListPtr);
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
	OBJECTMANAGER->Update();
	CAMERAMANAGER->GetMainCamera()->Update();
}

void Scene4::Render(HDC hdc)
{
	mTileMap->Render(hdc);
	mTrailManager->Render(hdc);
	OBJECTMANAGER->Render(hdc);

	// {{ �ϼ������� ������ �� ���� ����
	wstring strScene = L"�̰� 4�� �� / ����Ű: �̵� / �̵� �� shift: ���";
	TextOut(hdc, 500, 78, strScene.c_str(), (int)strScene.length());
	strScene = L"c: ���� / space: ������Ʈ ����, ����� ����, Ǫ�� ����(����ũ ���) / z: ����,������ �ݱ�";
	TextOut(hdc, 500, 100, strScene.c_str(), (int)strScene.length());
	strScene = L"x: ����,������ ����, ����󿡰� ���� �ֱ� / v: ������ ���� ������ / m: ����ũ ������";
	TextOut(hdc, 500, 122, strScene.c_str(), (int)strScene.length());
	strScene = L"������ L: �÷��̾� �����, Ÿ�� ���� on/off";
	TextOut(hdc, 500, 144, strScene.c_str(), (int)strScene.length());
	// �ϼ������� ������ �� ���� �� }}
}