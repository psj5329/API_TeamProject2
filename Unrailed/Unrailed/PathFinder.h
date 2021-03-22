#pragma once


class Tile;
class PathFinder
{
	Singleton(PathFinder)

public:
	struct Load {
		int x;				// Ÿ�� �ε���
		int y;
		bool isPass;		// ������ ������ Ȯ��... �׳� ������������
	};

private:
	vector<vector<Tile*>> mTileList;
	vector<Load*> mFindPath;	// �̰� �� ã�� �뵵

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
