#pragma once


class Tile;
class PathFinder
{
	Singleton(PathFinder)

public:
	struct Load {
		Tile* parent;
		bool isOpen;
		bool isClose;
		int costFromStart;
		int costToEnd;
		int costTotal;
	};

public:
	vector<Tile*> FindPath(vector<vector<Tile*>> tileList
		, int startX, int startY, int endX, int endY);
	float CalcMinDistance(int x1, int y1, int x2, int y2);
};
