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
	mTrailManager->InsertTrail(6, 0, 1, 3);
	mTrailManager->InsertTrail(6, 1, 1, 3);
	mTrailManager->InsertTrail(6, 2, 1, 3);
	mTrailManager->InsertTrail(6, 3, 1, 3);
	mTrailManager->InsertTrail(6, 4, 1, 1);
	mTrailManager->InsertTrail(5, 4, 1, 1);
	mTrailManager->InsertTrail(4, 4, 1, 3);
	mTrailManager->InsertTrail(4, 5, 1, 3);
	mTrailManager->InsertTrail(4, 6, 1, 3);
	mTrailManager->InsertTrail(4, 7, 1, 0);
	mTrailManager->InsertTrail(5, 7, 1, 0);
	mTrailManager->InsertTrail(6, 7, 1, 0);
	mTrailManager->InsertTrail(7, 7, 1, 2);
	mTrailManager->InsertTrail(7, 6, 1, 2);
	mTrailManager->InsertTrail(7, 5, 1, 2);
	mTrailManager->InsertTrail(7, 4, 1, 1);


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

	//mMachop = new Machop;
	//mMachop->Init();
	//mMachop->SetX(60);
	//mMachop->SetY(WINSIZEY / 2 - 40);
	//OBJECTMANAGER->AddObject(ObjectLayer::ITEM, mMachop);
	//vector <vector <Trail*>>* temp = mTrailManager->GetTrailListPtr();
	//mMachop->SetTrail(temp);
	//
	//mAbra = new Abra;
	//mAbra->Init();
	//mAbra->SetX(50);
	//mAbra->SetY(WINSIZEY / 2 - 40);
	//OBJECTMANAGER->AddObject(ObjectLayer::ITEM, mAbra);
	//vector <vector <Trail*>>* temp2 = mTrailManager->GetTrailListPtr();
	//mAbra->SetTrail(temp2);
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