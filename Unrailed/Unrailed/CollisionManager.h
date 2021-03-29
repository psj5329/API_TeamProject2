#pragma once

class Player;
class TileMap;
class Tile;
class MapObject;
class Train;

class CollisionManager
{
	Singleton(CollisionManager)

public:
	void Update();

public:
	bool IsCollision(RECT* rc1, RECT* rc2);

	void TileMapObjectCollision(Player* player, RECT* rc, vector<vector<Tile*>>* tileList, vector<vector<MapObject*>>* mapObjectList,
		int indexX, int indexY, int tileCountX, int tileCountY); // 유찬 생성 // 플레이어 이동 체크

	void TileCollision(Player* player, TileMap* tileMap, int tileCountX, int tileCountY);	// 플레이어 정보와 각 씬의 타일맵 정보를 받아와 갈 수 없는 땅은 못가게

	void MapObjectCollision(Player* player, TileMap* tileMap, int tileCountX, int tileCountY); // 잘못된 것 같다..
	void MapObjectCollision(Player* player, RECT* rc, TileMap* tileMap, int tileCountX, int tileCountY); // 잘못된 것 같다..
	void MapObjectCollision(RECT* rc, vector<vector<MapObject*>>* mapObjectList, int tileIndexX, int tileIndexY); // 유찬 생성 // 아직 디테일 부족해서 수정해야 함
	GameObject* ItemCollision(RECT* rc);
	Train* TrainCollision(RECT* rc, bool* isUpDownCollision); // 유찬 생성 // 가장 짧은 기차 충돌 방향 체크
};

#define COLLISIONMANAGER CollisionManager::GetInstance()