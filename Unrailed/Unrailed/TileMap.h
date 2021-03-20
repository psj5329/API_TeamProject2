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
	void LoadMap(int i);
	void InitEmptyMap();
	RECT GetTileRect(int indexX, int indexY) { return mTileList[indexY][indexX]->GetRect(); }
	TileType GetTileType(int indexX, int indexY) { return mTileList[indexY][indexX]->GetTileType(); }

	float GetTileCenterX(int indexX, int indexY) {
		return (mTileList[indexY][indexX]->GetRect().left + mTileList[indexY][indexX]->GetRect().right) / 2;
	}
	float GetTileCenterY(int indexX, int indexY) {
		return (mTileList[indexY][indexX]->GetRect().top + mTileList[indexY][indexX]->GetRect().bottom) / 2;
	}

	vector<vector<Tile*>>* GetTileListPtr() { return &mTileList; }
	vector<vector<MapObject*>>* GetObjectListPtr() { return &mMapObjectList; }
};

