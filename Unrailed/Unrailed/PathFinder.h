#pragma once


class Tile;
class PathFinder
{
	Singleton(PathFinder)

public:
	struct Load {
		int x;				// 타일 인덱스
		int y;
		bool isPass;		// 지나간 길인지 확인... 그냥 벡터하지말까
	};

private:
	vector<vector<Tile*>> mTileList;
	vector<Load*> mFindPath;	// 이건 길 찾는 용도

	int mStartIndexX;
	int mStartIndexY;
	int mArrivalIndexX;
	int mArrivalIndexY;

public:
	void Init();
	void FindPath();

public:
	vector<Load*> GetPath() { return mFindPath; }
	int GetStartIndexX() { return mStartIndexX; }
	int GetStartIndexY() { return mStartIndexY; }
	int GetArrivalIndexX() { return mArrivalIndexX; }
	int GetArrivalIndexY() { return mArrivalIndexY; }

	void SetTileList(vector<vector<Tile*>> tile) { mTileList = tile; }
	void SetStartIndexX(int x) { mStartIndexX = x; }
	void SetStartIndexY(int y) { mStartIndexY = y; }
	void SetArrivalIndexX(int x) { mArrivalIndexX = x; }
	void SetArrivalIndexY(int y) { mArrivalIndexY = y; }
};
