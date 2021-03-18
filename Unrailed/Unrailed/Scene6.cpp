#include "pch.h"
#include "Scene6.h"
#include "TrailManager.h"
#include "TileMap.h"

void Scene6::Init()
{
	mTileMap = new TileMap();
	mTileMap->Init(TileCountX, TileCountY, TileSize);
	mTileMap->LoadMap();
	//���� ��� �ִ� Ÿ�� ���� Ȯ��
	//�� ����Ÿ���� �߰����� Ÿ�� ������ �̵�
	//�ݺ�

	//�ε��� �������� : ���� x�� ������ Ÿ�ϻ�����
	mTrailManager = new TrailManager();
	mTrailManager->Init();
	mTrailManager->InsertTrail(0, 6, 1, 3);
	mTrailManager->InsertTrail(1, 6, 1, 3);
	mTrailManager->InsertTrail(2, 6, 1, 3);
	mTrailManager->InsertTrail(3, 6, 1, 0);
	mTrailManager->InsertTrail(3, 7, 1, 3);
	mTrailManager->InsertTrail(4, 7, 1, 3);
	mTrailManager->InsertTrail(5, 7, 1, 3);
	mTrailManager->InsertTrail(6, 7, 1, 3);
	mTrailManager->InsertTrail(7, 7, 1, 1);
	mTrailManager->InsertTrail(7, 6, 1, 1);
	mTrailManager->InsertTrail(7, 5, 1, 1);
	mTrailManager->InsertTrail(7, 4, 1, 2);
	mTrailManager->InsertTrail(6, 4, 1, 2);
	mTrailManager->InsertTrail(5, 4, 1, 2);
	mTrailManager->InsertTrail(4, 4, 1, 2);
	mTrailManager->InsertTrail(3, 4, 1, 2);




	//Train����
	for (int i = 0; i < 3; i++)
	{
		Voltorb* temp = new Voltorb;
		temp->Init();
		temp->SetX(70 - 50 * i);
		mVoltorbVec.push_back(temp);
		
		vector <vector <Trail*>>* temp2 = mTrailManager->GetTrailListPtr();
		mVoltorbVec[i]->SetTrail(temp2);

		OBJECTMANAGER->AddObject(ObjectLayer::ITEM, mVoltorbVec[i]);
	}

	mVoltorbVec[0]->SetImage(0);
	mVoltorbVec[1]->SetImage(1);
	mVoltorbVec[2]->SetImage(1);

	mVoltorbVec[0]->SetY(WINSIZEY / 2 - 50);
	mVoltorbVec[1]->SetY(WINSIZEY / 2 - 40);
	mVoltorbVec[2]->SetY(WINSIZEY / 2 - 40);
}

void Scene6::Release()
{
	mTrailManager->Release();
	mTileMap->Release();
}

void Scene6::Update()
{
	mTileMap->Update();
	mTrailManager->Update();

	OBJECTMANAGER->Update();
}

void Scene6::Render(HDC hdc)
{
	mTileMap->Render(hdc);
	mTrailManager->Render(hdc);

	OBJECTMANAGER->Render(hdc);
}