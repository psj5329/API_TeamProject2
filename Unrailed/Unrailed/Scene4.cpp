#include "pch.h"
#include "Scene4.h"

#include "Player.h"
#include "Camera.h"

#include "TileMap.h"

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

	// {{ �ϼ������� ������ �� ���� ����
	wstring strScene = L"�̰� 4�� �� / ����Ű: �̵� / �̵� �� shift: ��� / 8, 9, 0, 7: ����";
	TextOut(hdc, WINSIZEX / 2 - 15, 100, strScene.c_str(), strScene.length());
	// �ϼ������� ������ �� ���� �� }}
}