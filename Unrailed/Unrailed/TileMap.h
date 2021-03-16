#pragma once
#include "Tile.h"
#include "MapObject.h"
class Tile;
class TileMap
{
	vector <vector <Tile*>> mTileList;
	vector <vector <MapObject*>> mMapObjectList;
	int mTileCountX;
	int mTileCountY;

public:
	void Init(int tileX, int tileY, int tileSize);
	void Release();
	void Update();
	void Render(HDC hdc);

	void SetTileListPtr(vector <vector <Tile*>>* tileListPtr) { mTileList = *tileListPtr; }
	void LoadMap();
	RECT GetTileRect(int indexX, int indexY) { return mTileList[indexY][indexX]->GetRect(); }
	TileType GetTileType(int indexX, int indexY) { return mTileList[indexY][indexX]->GetTileType(); }

	vector <vector <Tile*>> GetTileList() { return mTileList; }
	vector <vector <MapObject*>> GetMapObjectList() { return mMapObjectList; }
};

