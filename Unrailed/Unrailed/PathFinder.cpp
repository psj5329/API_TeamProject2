#include "pch.h"
#include "PathFinder.h"

#include "Tile.h"
#include "MapObject.h"

Singleton_NULL(PathFinder)

vector<Tile*> PathFinder::FindPath(vector<vector<Tile*>> tileList, vector<vector<MapObject*>> objectList, int startX, int startY, int endX, int endY)
{
	vector<Tile*> path;		// �ݳ��� ��
	vector<Tile*> openLoad;		// ��ã�� �� �˻�??

	if (startX == endX && startY == endY)
		return path;

	if (startX < 0 || startX >= tileList[0].size()
		|| startY < 0 || startY >= tileList.size()
		|| endX < 0 || endX >= tileList[0].size()
		|| endY < 0 || endY >= tileList.size())
		return path;

	vector<vector<Load>> loadList;
	loadList.assign(tileList.size(), vector<Load>());
	for (int i = 0; i < loadList.size(); ++i)
		loadList[i].assign(tileList[0].size(), Load());

	loadList[startY][startX].isOpen = true;
	loadList[startY][startX].isClose = true;
	loadList[startY][startX].costFromStart = 0;
	loadList[startY][startX].costToEnd = CalcMinDistance(startX, startY, endX, endY);
	loadList[startY][startX].costTotal = loadList[startY][startX].costToEnd;

	Tile* startTile = tileList[startY][startX];
	Tile* endTile = tileList[endY][endX];
	Tile* currentTile = startTile;

	while (1)	// ��ã�⸦ �����غ���
	{
		if (currentTile == nullptr)
			return path;

		int currentX = currentTile->GetX() / tileList[0].size();
		int currentY = currentTile->GetY() / tileList.size();

		// �ֺ� Ÿ�� �˻�����
		for (int j = currentY - 1; j <= currentY + 1; ++j)
		{
			if (j < 0 || j >= tileList.size())
				continue;

			for (int i = currentX - 1; i <= currentX + 1; ++i)
			{
				if (i < 0 || i >= tileList[0].size())
					continue;

				if (abs(i - currentX) == abs(j - currentY))
					continue;

				if (loadList[j][i].isClose)
					continue;

				if (tileList[j][i]->GetTileType() == TileType::Wall
					|| tileList[j][i]->GetTileType() == TileType::Lava
					|| tileList[j][i]->GetTileType() == TileType::ice
					|| tileList[j][i]->GetTileType() == TileType::Water)
				{
					loadList[j][i].isClose = true;
					loadList[j][i].isOpen = true;
					continue;
				}

				// Ÿ���� �븻������ ������Ʈ�� ���� ���
				if (objectList[j][i]->GetMapObjectType() != MapObjectType::None)
				{
					loadList[j][i].isClose = true;
					loadList[j][i].isOpen = true;
					continue;
				}

				// ó�� �˻��ϴ� Ÿ���̸�?
				if (!loadList[j][i].isOpen)
				{
					loadList[j][i].isOpen = true;
					loadList[j][i].parent = currentTile;
					loadList[j][i].costFromStart = loadList[currentY][currentX].costFromStart + 1;
					loadList[j][i].costToEnd = CalcMinDistance(i, j, endX, endY);
					loadList[j][i].costTotal = loadList[j][i].costFromStart + loadList[j][i].costToEnd;

					openLoad.push_back(tileList[j][i]);
				}
				else
				{			
					// �ͺô� Ÿ���̸�?
					int cost = loadList[currentY][currentX].costFromStart + 1;
					if (cost < loadList[j][i].costFromStart)
					{
						loadList[j][i].costFromStart = cost;
						loadList[j][i].costTotal = loadList[j][i].costFromStart + loadList[j][i].costToEnd;
						loadList[j][i].parent = currentTile;
					}
				}
			}
		}

		// openLoad���� ���� ���� ��� ã��
		Tile* tileMin = nullptr;

		for (int i = 0; i < openLoad.size(); ++i)
		{
			if (openLoad[i] == currentTile)
			{
				openLoad.erase(openLoad.begin() + i);
				--i;
				continue;
			}

			if (tileMin == nullptr)
			{
				tileMin = openLoad[i];
				continue;
			}

			if (loadList[openLoad[i]->GetY() / TileSize][openLoad[i]->GetX() / TileSize].costTotal
				< loadList[tileMin->GetY() / TileSize][tileMin->GetX() / TileSize].costTotal)
				tileMin = openLoad[i];
		}

		if (tileMin != nullptr)
		{
			loadList[tileMin->GetY() / TileSize][tileMin->GetX() / TileSize].isClose = true;
			currentTile = tileMin;
		}
		else
		{
			Tile* temp = tileMin;
			path.push_back(temp);
			while (loadList[temp->GetY() / TileSize][temp->GetX() / TileSize].parent != nullptr)
			{
				temp = loadList[temp->GetY() / TileSize][temp->GetX() / TileSize].parent;
				path.push_back(temp);
			}

			reverse(path.begin(), path.end());

			return path;
		}
	}
	return path;
}

float PathFinder::CalcMinDistance(int x1, int y1, int x2, int y2)
{
	int minPathX = abs(x2 - x1);
	int minPathY = abs(y2 - y1);
	//int minimum = min(minPathX, minPathY);
	float distance = sqrtf(pow(minPathX, 2) + pow(minPathY, 2));
	return distance;
}
