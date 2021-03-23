#include "pch.h"
#include "PathFinder.h"

#include "Tile.h"

vector<Tile*> PathFinder::FindPath(vector<vector<Tile*>> tileList, int startX, int startY, int endX, int endY)
{
	vector<Tile*> path;
	vector<Load*> load;

	if (startX == endX && startY == endY)
		return path;

	if (startX < 0 || startX >= tileList[0].size()
		|| startY < 0 || startY >= tileList.size()
		|| endX < 0 || endX >= tileList[0].size()
		|| endY < 0 || endY >= tileList.size())
		return path;

	Load* startPos = {};
	startPos->parent = nullptr;
	startPos->isOpen = false;
	startPos->isClose = false;
	startPos->costFromStart = 0;
	startPos->costToEnd = 0;
	startPos->costTotal = 0;

	load.push_back(startPos);	// 시작 위치를 해주자

	Tile* startTile = tileList[startY][startX];
	Tile* endTile = tileList[endY][endX];
	Tile* currentTile = startTile;

	while (1)	// 길찾기를 시작해보자
	{
		int currentX = currentTile->GetX() / tileList[0].size();
		int currentY = currentTile->GetY() / tileList.size();

		// 주변 타일 검사하자
		for (int j = currentY - 1; j <= currentY + 1; ++j)
		{
			if (j < 0 || j >= tileList.size())
				continue;

			for (int i = currentX - 1; i <= currentX + 1; ++i)
			{
				if (i < 0 || i >= tileList[0].size())
					continue;

				if (j == currentY && i == currentX)
					continue;

				if (tileList[j][i]->GetTileType() != TileType::Normal)
				{
					continue;
				}

			}
		}
	}
	return path;
}

float CalcMinDistance(int x1, int y1, int x2, int y2)
{
	int minPathX = abs(x2 - x1);
	int minPathY = abs(y2 - y1);
	//int minimum = min(minPathX, minPathY);
	float distance = sqrtf(pow(minPathX, 2) + pow(minPathY, 2));
	return distance;
}
