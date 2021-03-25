#include "pch.h"
#include "Train.h"

//Train::Train()
//	: GameObject()
//{
//}

void Train::SetTarget()
{
	int indexX = mX / TileSize;
	int indexY = mY / TileSize;

	Direction dir = (Direction)mTrailList[indexY][indexX]->GetDirection();
	mDirection = dir;

	if (dir == Direction::Down)
	{
		mTargetX = indexX;
		mTargetY = indexY + 1;
	}
	if (dir == Direction::Up)
	{
		mTargetX = indexX;
		mTargetY = indexY - 1;
	}
	if (dir == Direction::Left)
	{
		mTargetX = indexX - 1;
		mTargetY = indexY;
	}
	if (dir == Direction::Right)
	{
		mTargetX = indexX + 1;
		mTargetY = indexY;
	}
}

bool Train::CheckTile()
{
	float centerX = (mTrailList[mTargetY][mTargetX]->GetRect().left + mTrailList[mTargetY][mTargetX]->GetRect().right) / 2;
	float centerY = (mTrailList[mTargetY][mTargetX]->GetRect().top + mTrailList[mTargetY][mTargetX]->GetRect().bottom) / 2;

	if (mDirection == Direction::Down)
	{
		if (mY <= centerY)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	if (mDirection == Direction::Up)
	{
		if (mY >= centerY)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	if (mDirection == Direction::Left)
	{
		if (mX >= centerX)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	if (mDirection == Direction::Right)
	{
		if (mX <= centerX)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}

void Train::SetSpeed()
{
	if (mDirection == Direction::Down)
	{
		mSpeedX = 0;
		mSpeedY = 100.f;
	}
	if (mDirection == Direction::Up)
	{
		mSpeedX = 0;
		mSpeedY = -100.f;
	}
	if (mDirection == Direction::Left)
	{
		mSpeedX = -100.f;
		mSpeedY = 0;
	}
	if (mDirection == Direction::Right)
	{
		mSpeedX = 100.f;
		mSpeedY = 0;
	}
}