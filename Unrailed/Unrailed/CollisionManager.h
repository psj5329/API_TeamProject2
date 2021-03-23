#pragma once

class Player;
class TileMap;
class Tile;
class MapObject;

class CollisionManager
{
	Singleton(CollisionManager)

public:
	void Update();

public:
	bool IsCollision(RECT* rc1, RECT* rc2);

	void TileMapObjectCollision(Player* player, RECT* rc, vector<vector<Tile*>>* tileList, vector<vector<MapObject*>>* mapObjectList, int indexX, int indexY);

	void TileCollision(Player* player, TileMap* tileMap);	// 플레이어 정보와 각 씬의 타일맵 정보를 받아와 갈 수 없는 땅은 못가게
	void TileCollision(Player* player, RECT* rc, TileMap* tileMap); // 잘못된 것 같다..

	


	void MapObjectCollision(Player* player, TileMap* tileMap); // 잘못된 것 같다..
	void MapObjectCollision(Player* player, RECT* rc, TileMap* tileMap); // 잘못된 것 같다..
	void MapObjectCollision(Player* player, RECT* rc, vector<vector<Tile*>>* tileList, vector<vector<MapObject*>>* mapObjectList); // 추가 테스트 중
	void MapObjectCollision(RECT* rc, vector<vector<MapObject*>>* mapObjectList, int tileIndexX, int tileIndexY);
	GameObject* ItemCollision(RECT* rc);
};

#define COLLISIONMANAGER CollisionManager::GetInstance()

