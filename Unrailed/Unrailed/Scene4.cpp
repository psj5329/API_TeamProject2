#include "pch.h"
#include "Scene4.h"

#include "Player.h"
#include "Camera.h"

#include "TileMap.h"
#include "TrailManager.h"

void Scene4::Init()
{
	// �÷��̾ ��� ���ΰ������� ���� ��
	mTempPlayer = new Player("Player", TileSize * 4.5, TileSize * 4.5);
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
	mTileMap->Init(TileCountX, TileCountY, TileSize);
	mTileMap->LoadMap();

	mTrailManager = new TrailManager();
	mTrailManager->Init(mTileMap->GetYTileCount(),mTileMap->GetXTileCount());
	mTrailManager->InsertTrail(0, 5, 1, 0); // 0: down 1: up 2: left 3: right
	mTrailManager->InsertTrail(1, 5, 1, 0);
	mTrailManager->InsertTrail(2, 5, 1, 0);
	mTrailManager->InsertTrail(3, 5, 1, 0);

	mTrailManager->InsertTrail(4, 5, 1, 3);
	mTrailManager->InsertTrail(4, 6, 1, 3);

	mTrailManager->InsertTrail(4, 7, 1, 0);
	mTrailManager->InsertTrail(5, 7, 1, 0);
	mTrailManager->InsertTrail(6, 7, 1, 0);

	mTrailManager->InsertTrail(7, 7, 1, 2);
	mTrailManager->InsertTrail(7, 6, 1, 2);
	mTrailManager->InsertTrail(7, 5, 1, 2);
	mTrailManager->InsertTrail(7, 4, 1, 2);

	mTrailManager->InsertTrail(7, 3, 1, 1);
	mTrailManager->InsertTrail(6, 3, 1, 1);
	mTrailManager->InsertTrail(5, 3, 1, 1);

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
	OBJECTMANAGER->Update();

	RECT* playerColBoxPtr = mTempPlayer->GetColBoxPtr();
	COLLISIONMANAGER->TileCollision(mTempPlayer, playerColBoxPtr, mTileMap); // �÷��̾� ���η� �̵�.. �ϴ� ����.. ������ �絵��
//	COLLISIONMANAGER->MapObjectCollision(mTempPlayer, playerColBoxPtr, mTileMap);
}

void Scene4::Render(HDC hdc)
{
	mTileMap->Render(hdc);
	mTrailManager->Render(hdc);
	OBJECTMANAGER->Render(hdc);

	// {{ �ϼ������� ������ �� ���� ����
	wstring strScene = L"�̰� 4�� �� / ����Ű: �̵� / �̵� �� shift: ��� / C: ����";
	TextOut(hdc, WINSIZEX / 2 - 15, 100, strScene.c_str(), strScene.length());
	// �ϼ������� ������ �� ���� �� }}
}