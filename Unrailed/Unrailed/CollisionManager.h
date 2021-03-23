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

	void TileMapObjectCollision(Player* player, RECT* rc, vector<vector<Tile*>>* tileList, vector<vector<MapObject*>>* mapObjectList, int indexX, int indexY); // ���� ���� // �÷��̾� �̵� üũ

	void TileCollision(Player* player, TileMap* tileMap);	// �÷��̾� ������ �� ���� Ÿ�ϸ� ������ �޾ƿ� �� �� ���� ���� ������
	//void TileCollision(Player* player, RECT* rc, TileMap* tileMap); // �߸��� �� ����..
	//void TileCollision(Player* player, RECT* rc, vector<vector<Tile*>>* tileListPtr); // ���� ���� // �׽�Ʈ���̶� ���߿� ����� ��

	void MapObjectCollision(Player* player, TileMap* tileMap); // �߸��� �� ����..
	void MapObjectCollision(Player* player, RECT* rc, TileMap* tileMap); // �߸��� �� ����..
	//void MapObjectCollision(Player* player, RECT* rc, vector<vector<Tile*>>* tileList, vector<vector<MapObject*>>* mapObjectList); // ���� ���� �߰� �׽�Ʈ �� �Ƹ��� ���� ����
	void MapObjectCollision(RECT* rc, vector<vector<MapObject*>>* mapObjectList, int tileIndexX, int tileIndexY); // ���� ���� // ���� ������ �����ؼ� �����ؾ� ��
	GameObject* ItemCollision(RECT* rc);
};

#define COLLISIONMANAGER CollisionManager::GetInstance()