#include "pch.h"
#include "PathFinder.h"

void PathFinder::Init()
{
	mStartIndexX = 0;
	mStartIndexY = 0;
	mArrivalIndexX = 0;
	mArrivalIndexY = 0;
}

void PathFinder::FindPath()
{
	if (mStartIndexX == mArrivalIndexX && mStartIndexY == mArrivalIndexY)
		return;

	if (mStartIndexX < 0 || mStartIndexX >= mTileList[0].size()
		|| mStartIndexY < 0 || mStartIndexY >= mTileList.size()
		|| mArrivalIndexX < 0 || mArrivalIndexX >= mTileList[0].size()
		|| mArrivalIndexY < 0 || mArrivalIndexY >= mTileList.size())
		return;


}
