#pragma once

class Player;
class TileMap;
class CollisionManager
{
	Singleton(CollisionManager)

public:
	void Update();

public:
	bool IsCollision(RECT* rc1, RECT* rc2);
	void TileCollision(Player* player, TileMap* tileMap);	// 플레이어 정보와 각 씬의 타일맵 정보를 받아와 갈 수 없는 땅은 못가게
	void MapObjectCollision(Player* player, TileMap* tileMap);
};

#define COLLISIONMANAGER CollisionManager::GetInstance()

