#include "pch.h"
#include "TrailManager.h"
#include "Trail.h"

void TrailManager::Init()
{
	
}

void TrailManager::Release()
{

}

void TrailManager::Update()
{

}

void TrailManager::Render(HDC hdc)
{
	for (int i = 0;i < mTrailList.size();i++)
	{
		mTrailList[i].Render(hdc);
	}
}

void TrailManager::InsertTrail()
{

}

void TrailManager::TurnTrail()
{

}