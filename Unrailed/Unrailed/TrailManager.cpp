#include "pch.h"
#include "TrailManager.h"
#include "Trail.h"

void TrailManager::Init()
{
	//빈트레일 인잇
	for (int y = 0; y < TileCountY; ++y)
	{
		mTrailList.push_back(vector <Trail*>());
		for (int x = 0; x < TileCountX; ++x)
		{
			//벡터
			Trail* trail = new Trail();
			trail->Init(TileSize * x, TileSize * y, 0, 0);
			mTrailList[y].push_back(trail);

		}
	}
}

void TrailManager::Release()
{
	//타일 릴리즈
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

//여기서 위치판단? 넣을지말지 정하고 백터에 넣어주자, 넣고 주위를 한번더 확인,
void TrailManager::InsertTrail(int x,int y, int type, int dir)
{
	//해당인덱스의 속성
	mTrailList[y][x]->SetTrailType(type);
	mTrailList[y][x]->SetDirection(dir);

	//넣고 주위를 한번더 확인, 닿는 타일이있으면 커넥티드바까주기
}

//끝에 있는 기차길 돌리기
void TrailManager::TurnTrail()
{
	//mTrailList.back()->Turn();
}

//끄트머리의 기차길을 줍기
void TrailManager::PickUpTrail()
{
	//mTrailList.pop_back();
}