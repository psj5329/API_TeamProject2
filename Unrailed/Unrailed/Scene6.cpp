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
	mTrailManager->Init(mTileMap->GetYTileCount(), mTileMap->GetXTileCount());
	mTrailManager->InsertTrail(6, 0, ItemType::Green, 3);
	mTrailManager->InsertTrail(6, 1, ItemType::Green, 3);
	mTrailManager->InsertTrail(6, 2, ItemType::Green, 3);
	mTrailManager->InsertTrail(6, 3, ItemType::Green, 3);
	mTrailManager->InsertTrail(6, 4, ItemType::Green, 1);
	mTrailManager->InsertTrail(5, 4, ItemType::Green, 1);
	mTrailManager->InsertTrail(4, 4, ItemType::Green, 3);
	mTrailManager->InsertTrail(4, 5, ItemType::Green, 3);
	mTrailManager->InsertTrail(4, 6, ItemType::Green, 3);
	mTrailManager->InsertTrail(4, 7, ItemType::Green, 0);
	mTrailManager->InsertTrail(5, 7, ItemType::Green, 0);
	mTrailManager->InsertTrail(6, 7, ItemType::Green, 0);
	mTrailManager->InsertTrail(7, 7, ItemType::Green, 2);
	mTrailManager->InsertTrail(7, 6, ItemType::Green, 2);
	mTrailManager->InsertTrail(7, 5, ItemType::Green, 2);
	mTrailManager->InsertTrail(7, 4, ItemType::Green, 1);


	//Train����
	for (int i = 0; i < 3; i++)
	{
		Voltorb* temp = new Voltorb;
		temp->Init();
		temp->SetX(120 - 50 * i);
		mVoltorbVec.push_back(temp);
		
		vector <vector <Trail*>>* temp2 = mTrailManager->GetTrailListPtr();
		mVoltorbVec[i]->SetTrail(temp2);

		OBJECTMANAGER->AddObject(ObjectLayer::TRAIN, mVoltorbVec[i]);
	}

	mVoltorbVec[0]->SetImage(0);
	mVoltorbVec[1]->SetImage(1);
	mVoltorbVec[2]->SetImage(1);

	for (int i = 0; i < 3; i++)
	{
		mVoltorbVec[i]->SetY(WINSIZEY / 2 - 40);
	}

	mMachop = new Machop;
	mMachop->Init();
	mMachop->SetX(-35);
	mMachop->SetY(WINSIZEY / 2 - 40);
	OBJECTMANAGER->AddObject(ObjectLayer::TRAIN, mMachop);
	vector <vector <Trail*>>* temp = mTrailManager->GetTrailListPtr();
	mMachop->SetTrail(temp);
	
	mAbra = new Abra;
	mAbra->Init();
	mAbra->SetX(-85);
	mAbra->SetY(WINSIZEY / 2 - 40);
	OBJECTMANAGER->AddObject(ObjectLayer::TRAIN, mAbra);
	vector <vector <Trail*>>* temp2 = mTrailManager->GetTrailListPtr();
	mAbra->SetTrail(temp2);
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