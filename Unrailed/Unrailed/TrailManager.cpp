#include "pch.h"
#include "TrailManager.h"
#include "Trail.h"

void TrailManager::Init()
{
	//��Ʈ���� ����
	for (int y = 0; y < TileCountY; ++y)
	{
		mTrailList.push_back(vector <Trail*>());
		for (int x = 0; x < TileCountX; ++x)
		{
			//����
			Trail* trail = new Trail();
			trail->Init(TileSize * x, TileSize * y, 0, 0);
			mTrailList[y].push_back(trail);

		}
	}
}

void TrailManager::Release()
{
	//Ÿ�� ������
	for (int y = 0; y < mTrailList.size(); ++y)
	{
		for (int x = 0; x < mTrailList[y].size(); ++x)
		{
			SafeDelete(mTrailList[y][x]);
			mTrailList[y].erase(mTrailList[y].begin() + x);
			x--;
		}
		if (mTrailList[y].size() <= 0)
		{
			mTrailList.erase(mTrailList.begin() + y);
			y--;
		}
	}
}

void TrailManager::Update()
{
	for (int y = 0; y < mTrailList.size(); ++y)
	{
		for (int x = 0; x < mTrailList[y].size(); ++x)
		{
			mTrailList[y][x]->Update();
		}
	}
}

void TrailManager::Render(HDC hdc)
{
	for (int y = 0; y < mTrailList.size(); ++y)
	{
		for (int x = 0; x < mTrailList[y].size(); ++x)
		{
			mTrailList[y][x]->Render(hdc);
		}
	}
}

//���⼭ ��ġ�Ǵ�? ���������� ���ϰ� ���Ϳ� �־�����, �ְ� ������ �ѹ��� Ȯ��,
void TrailManager::InsertTrail(int x,int y, int type, int dir)
{
	//�ش��ε����� �Ӽ�
	mTrailList[y][x]->SetTrailType(type);
	mTrailList[y][x]->SetDirection(dir);

	//�ְ� ������ �ѹ��� Ȯ��, ��� Ÿ���������� Ŀ��Ƽ��ٱ��ֱ�
}

//���� �ִ� ������ ������
void TrailManager::TurnTrail()
{
	//mTrailList.back()->Turn();
}

//��Ʈ�Ӹ��� �������� �ݱ�
void TrailManager::PickUpTrail()
{
	//mTrailList.pop_back();
}