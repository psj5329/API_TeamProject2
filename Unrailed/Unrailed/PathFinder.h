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

		Load()
			: parent(nullptr), isOpen(false), isClose(false),
			costFromStart(MAXINT), costToEnd(MAXINT), costTotal(MAXINT) {}
	};

public:
	vector<Tile*> FindPath(vector<vector<Tile*>> tileList, vector<vector<MapObject*>> objectList
		, int startX, int startY, int endX, int endY);
	float CalcMinDistance(int x1, int y1, int x2, int y2);
};

#define PATHFINDER PathFinder::GetInstance()