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
	void TileCollision(Player* player, TileMap* tileMap);	// �÷��̾� ������ �� ���� Ÿ�ϸ� ������ �޾ƿ� �� �� ���� ���� ������
	void TileCollision(Player* player, RECT* rc, TileMap* tileMap);
	void MapObjectCollision(Player* player, TileMap* tileMap);
	void MapObjectCollision(Player* player, RECT* rc, TileMap* tileMap);
	GameObject* ItemCollision(RECT* rc);
};

#define COLLISIONMANAGER CollisionManager::GetInstance()

