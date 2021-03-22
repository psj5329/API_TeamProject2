#include "pch.h"
#include "Scene1.h"

#include "Image.h"
#include "Camera.h"

#include "Scene.h"

#include "TileMap.h"
#include "Camera.h"
#include "Player.h"
#include "Sableye.h"

void Scene1::Init()
{
	mTileMap = new TileMap();
	mTileMap->Init(TileCountX, TileCountY, TileSize);
	mTileMap->LoadMap();

	mPlayer = new Player("Player_test", 0, 0);
	mPlayer->SetX(48 * 2);
	mPlayer->SetY(48 * 5);
	OBJECTMANAGER->AddObject(ObjectLayer::PLAYER, mPlayer);

	mEnemy = new Sableye;
	mEnemy->Init();
	mEnemy->SetX(48);
	mEnemy->SetY(48);
	OBJECTMANAGER->AddObject(ObjectLayer::ENEMY, mEnemy);

	Camera* main = CAMERAMANAGER->GetMainCamera();
	main->ChangeMode(Camera::Mode::Follow);
	main->SetTarget(mPlayer);
	main->SetX(WINSIZEX / 2);
	main->SetY(WINSIZEY / 2);
	CAMERAMANAGER->SetMainCamera(main);

	OBJECTMANAGER->Init();

	vector<GameObject*> object = OBJECTMANAGER->GetObjectList(ObjectLayer::ENEMY);
	for (int i = 0; i < object.size(); ++i)
		dynamic_cast<Enemy*>(object[i])->SetTileList(*mTileMap->GetTileListPtr());
}

void Scene1::Release()
{
	OBJECTMANAGER->Release();
}

void Scene1::Update()
{
	if (INPUT->GetKeyDown(VK_SPACE))
	{
		GameObject* item = COLLISIONMANAGER->ItemCollision(mPlayer->GetColBoxPtr());//(&mPlayer->GetRect());
		if(item != nullptr)
			item->SetIsActive(false);
	}

	OBJECTMANAGER->Update();
	mTileMap->Update();
	COLLISIONMANAGER->TileCollision(mPlayer, mTileMap);
	COLLISIONMANAGER->MapObjectCollision(mPlayer, mTileMap);
	CAMERAMANAGER->Update();
}

void Scene1::Render(HDC hdc)
{
	mTileMap->Render(hdc);
	OBJECTMANAGER->Render(hdc);

	wstring strScene = L"�̰� 1�� ��, �Ŀ� ������ž�....";
	TextOut(hdc, WINSIZEX / 2 - 150, WINSIZEY / 2, strScene.c_str(), strScene.length());
}

