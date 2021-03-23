#include "pch.h"
#include "CollisionManager.h"
#include "Player.h"
#include "TileMap.h"

Singleton_NULL(CollisionManager)

void CollisionManager::Update()
{

}

bool CollisionManager::IsCollision(RECT * rc1, RECT * rc2)
{
	RECT temp;

	if (IntersectRect(&temp, rc1, rc2))		// �̰� �� �������� �浹 �ƴ϶��ؼ� �������
		return true;

	return false;

	/*if (rc1->right < rc2->left || rc1->left > rc2->right)return false;
	if (rc1->bottom < rc2->top || rc1->top > rc2->bottom)return false;

	return true;*/
}

void CollisionManager::TileMapObjectCollision(Player* player, RECT* rc, vector<vector<Tile*>>* tileList, vector<vector<MapObject*>>* mapObjectList, int indexX, int indexY)
{
	bool isCorrected = false;

	float rcWidth = rc->right - rc->left;
	float rcHeight = rc->bottom - rc->top;

	for (int j = indexY - 1; j <= indexY + 1; ++j) // Ÿ�� �˻�
	{
		for (int i = indexX - 1; i <= indexX + 1; ++i)
		{
			if (i >= 0 && i < TileCountX && j >= 0 && j < TileCountY)
			{
				Form form = player->GetForm();
				DirectionEight dir = player->GetDir();
				RECT temp;
				Tile* tile = (*tileList)[j][i];
				RECT tileRc = tile->GetRect();
				TileType tileType = tile->GetTileType();

			//	if (tileType == TileType::Normal)
			//		continue;
			//	else if (tileType == TileType::Water && form == Form::Totodile)
			//		continue;
			//	else if (tileType == TileType::Lava && form == Form::Charmander)
			//		continue;

				//if ((dir == DirectionEight::Down && j == indexY + 1) || (dir == DirectionEight::Up && j == indexY - 1) ||
				//	(dir == DirectionEight::Left && i == indexX - 1) || (dir == DirectionEight::Right && i == indexX + 1))
				//{
					if (IntersectRect(&temp, rc, &tileRc))
					{
						float width = temp.right - temp.left;
						float height = temp.bottom - temp.top;

						if (width > height)
						{
							if (temp.top == tileRc.top) // rc�� / tile��
								player->SetY(tileRc.top - rcHeight / 2);
							else // tile�� / rc��
								player->SetY(tileRc.bottom + rcHeight / 2);
						}
						else
						{
							if (temp.left == tileRc.left) // rc�� / tile��
								player->SetY(tileRc.left - rcWidth / 2);
							else // tile�� / rc��
								player->SetY(tileRc.right + rcWidth / 2);
						}

						isCorrected = true;
					}
				//}
			}
		}
	}

	if (isCorrected) // Ÿ�� �˻� �� ���� �������� �����ּ���.
		return;
	/*
	for (int j = indexY - 1; j <= indexY + 1; ++j) // �� ������Ʈ �˻�
	{
		for (int i = indexX - 1; i <= indexX + 1; ++i)
		{
			if (i >= 0 && i < TileCountX && j >= 0 && j < TileCountY)
			{
				Form form = player->GetForm();
				DirectionEight dir = player->GetDir();
				RECT temp;
				//MapObject* mapObject = (*mapObjectList)[j][i];
				RECT mapObjectRc = (*mapObjectList)[j][i]->GetRect();
				//MapObjectType mapObjectType = mapObject->GetMapObjectType();

				if ((dir == DirectionEight::Down && j == indexY + 1) || (dir == DirectionEight::Up && j == indexY - 1) ||
					(dir == DirectionEight::Left && i == indexX - 1) || (dir == DirectionEight::Right && i == indexX + 1))
				{
					if (IntersectRect(&temp, rc, &mapObjectRc))
					{
						float width = temp.right - temp.left;
						float height = temp.bottom - temp.top;

						if (width > height)
						{
							if (temp.top == mapObjectRc.top) // rc�� / mapObject��
								player->SetY(mapObjectRc.top - rcHeight / 2);
							else // mapObject�� / rc��
								player->SetY(mapObjectRc.bottom + rcHeight / 2);
						}
						else
						{
							if (temp.left == mapObjectRc.left) // rc�� / mapObject��
								player->SetY(mapObjectRc.left - rcWidth / 2);
							else // mapObject�� / rc��
								player->SetY(mapObjectRc.right + rcWidth / 2);
						}

						isCorrected = true;
					}
				}
			}
		}
	}*/
}

void CollisionManager::TileCollision(Player* player, TileMap* tileMap)
{
	vector <vector <Tile*>>* tileList = tileMap->GetTileListPtr();

	// ���߿� �������� ������ �Լ� �޾Ƽ� �ϰ� ��ġ��
	int x = player->GetX() / TileSize;
	int y = player->GetY() / TileSize;

	for (int j = y - 1; j <= y + 1; ++j)
	{
		for (int i = x - 1; i <= x + 1; ++i)
		{
			// ���� ���� ��
			if (i >= 0 && i < TileCountX && j >= 0 && j < TileCountY)
			{
				RECT temp;
				RECT playerRc = player->GetRect();
				RECT tileRc = (*tileList)[j][i]->GetRect();
				if (IntersectRect(&temp, &playerRc, &tileRc))
				{
					float width = temp.right - temp.left;
					float height = temp.bottom - temp.top;

					if ((*tileList)[j][i]->GetTileType() != TileType::Normal)
					{
						if (width > height)
						{
							if (temp.top == (*tileList)[j][i]->GetRect().top)	// �ÿ��̾ ��
								player->SetY((*tileList)[j][i]->GetRect().top - player->GetSizeY() / 2);
							else if (temp.bottom == (*tileList)[j][i]->GetRect().bottom)	// �ÿ��̾ �Ʒ�
								player->SetY((*tileList)[j][i]->GetRect().bottom + player->GetSizeY() / 2);
						}
						else
						{
							if (temp.left == (*tileList)[j][i]->GetRect().left)	// �ÿ��̾ ����
								player->SetX((*tileList)[j][i]->GetRect().left - player->GetSizeX() / 2);
							else if (temp.right == (*tileList)[j][i]->GetRect().right)	// �ÿ��̾ ������
								player->SetX((*tileList)[j][i]->GetRect().right + player->GetSizeX() / 2);

						}
					}
				}
			}
			else
			{
				if (j >= 0 && j < TileCountY)
				{
					// x�� �������� �Ѿ ��
					if (player->GetX() < (*tileList)[j][0]->GetRect().left + player->GetSizeX() / 2)
						player->SetX((*tileList)[j][0]->GetRect().left + player->GetSizeX() / 2);
					else if (player->GetX() > (*tileList)[j][TileCountX - 1]->GetRect().right - player->GetSizeX() / 2)
						player->SetX((*tileList)[j][TileCountX - 1]->GetRect().right - player->GetSizeX() / 2);
				}

				if (i >= 0 && i < TileCountX)
				{
					// y�� �������� �Ѿ ��
					if (player->GetY() < (*tileList)[0][i]->GetRect().top + player->GetSizeY() / 2)
						player->SetY((*tileList)[0][i]->GetRect().top + player->GetSizeY() / 2);
					else if (player->GetY() > (*tileList)[TileCountY - 1][i]->GetRect().bottom - player->GetSizeY() / 2)
						player->SetY((*tileList)[TileCountY - 1][i]->GetRect().bottom - player->GetSizeY() / 2);
				}
			}
		}
	}
}

void CollisionManager::TileCollision(Player* player, RECT* rc, TileMap* tileMap)
{
	vector<vector<Tile*>>* tileList = tileMap->GetTileListPtr();

	int x = player->GetX() / TileSize;
	int y = player->GetY() / TileSize;

	float rcWidth = rc->right - rc->left;
	float rcHeight = rc->bottom - rc->top;

	for (int j = y - 1; j <= y + 1; ++j)
	{
		for (int i = x - 1; i <= x + 1; ++i)
		{
			// ���� ���� ��
			if (i >= 0 && i < TileCountX && j >= 0 && j < TileCountY)
			{
				RECT temp;
				RECT tileRc = (*tileList)[j][i]->GetRect();
				if (IntersectRect(&temp, rc, &tileRc))
				{
					float width = temp.right - temp.left;
					float height = temp.bottom - temp.top;

					if ((*tileList)[j][i]->GetTileType() != TileType::Normal)
					{
						if (width > height)
						{
							if (temp.top == (*tileList)[j][i]->GetRect().top) // rc�� ��
								player->SetY((*tileList)[j][i]->GetRect().top - rcHeight / 2);
							else if (temp.bottom == (*tileList)[j][i]->GetRect().bottom) // rc�� �Ʒ�
								player->SetY((*tileList)[j][i]->GetRect().bottom + rcHeight / 2);
						}
						else
						{
							if (temp.left == (*tileList)[j][i]->GetRect().left) // rc�� ����
								player->SetX((*tileList)[j][i]->GetRect().left - rcWidth / 2);
							else if (temp.right == (*tileList)[j][i]->GetRect().right) // rc�� ������
								player->SetX((*tileList)[j][i]->GetRect().right + rcWidth / 2);
						}
					}
				}
			}
			else
			{
				if (j >= 0 && j < TileCountY)
				{
					// x�� �������� �Ѿ ��
					if (player->GetX() < (*tileList)[j][0]->GetRect().left + rcWidth / 2)
						player->SetX((*tileList)[j][0]->GetRect().left + rcWidth / 2);
					else if (player->GetX() > (*tileList)[j][TileCountX - 1]->GetRect().right - rcWidth / 2)
						player->SetX((*tileList)[j][TileCountX - 1]->GetRect().right - rcWidth / 2);
				}

				if (i >= 0 && i < TileCountX)
				{
					// y�� �������� �Ѿ ��
					if (player->GetY() < (*tileList)[0][i]->GetRect().top + rcHeight / 2)
						player->SetY((*tileList)[0][i]->GetRect().top + rcHeight / 2);
					else if (player->GetY() > (*tileList)[TileCountY - 1][i]->GetRect().bottom - rcHeight / 2)
						player->SetY((*tileList)[TileCountY - 1][i]->GetRect().bottom - rcHeight / 2);
				}
			}
		}
	}
}

void CollisionManager::MapObjectCollision(Player * player, TileMap * tileMap)
{
	vector <vector <Tile*>>* tileList = tileMap->GetTileListPtr();
	vector <vector <MapObject*>>* mapObjectList = tileMap->GetObjectListPtr();

	// ���߿� �������� ������ �Լ� �޾Ƽ� �ϰ� ��ġ��
	int x = player->GetX() / TileSize;
	int y = player->GetY() / TileSize;

	for (int j = y - 1; j <= y + 1; ++j)
	{
		for (int i = x - 1; i <= x + 1; ++i)
		{
			// ���� ���� ��
			if (i >= 0 && i < TileCountX && j >= 0 && j < TileCountY)
			{
				RECT temp;
				RECT playerRc = player->GetRect();
				RECT mapObjectRc = (*tileList)[j][i]->GetRect();
				if (IntersectRect(&temp, &playerRc, &mapObjectRc))
				{
					float width = temp.right - temp.left;
					float height = temp.bottom - temp.top;

					if ((*mapObjectList)[j][i]->GetMapObjectType() != MapObjectType::None)		// None�� �ƴ϶�� ������ ����
					{
						if (width > height)
						{
							if (temp.top == (*mapObjectList)[j][i]->GetRect().top)	// �÷��̾ ��
							{
								player->SetY((*mapObjectList)[j][i]->GetRect().top - player->GetSizeY() / 2);
								(*mapObjectList)[j][i]->DeductHp();

								//if ((*mapObjectList)[j][i]->GetHp() <= 0)
								//	(*tileList)[j][i]->SetTileType(TileType::Normal);
							}
							else if (temp.bottom == (*mapObjectList)[j][i]->GetRect().bottom)	// �÷��̾ �Ʒ�
							{
								player->SetY((*mapObjectList)[j][i]->GetRect().bottom + player->GetSizeY() / 2);
								(*mapObjectList)[j][i]->DeductHp();

								//if ((*mapObjectList)[j][i]->GetHp() <= 0)
								//	(*tileList)[j][i]->SetTileType(TileType::Normal);
							}
						}
						else
						{
							if (temp.left == (*mapObjectList)[j][i]->GetRect().left)	// �÷��̾ ����
							{
								player->SetX((*mapObjectList)[j][i]->GetRect().left - player->GetSizeX() / 2);
								(*mapObjectList)[j][i]->DeductHp();

								//if ((*mapObjectList)[j][i]->GetHp() <= 0)
								//	(*tileList)[j][i]->SetTileType(TileType::Normal);
							}
							else if (temp.right == (*mapObjectList)[j][i]->GetRect().right)	// �÷��̾ ������
							{
								player->SetX((*mapObjectList)[j][i]->GetRect().right + player->GetSizeX() / 2);
								(*mapObjectList)[j][i]->DeductHp();

								//if ((*mapObjectList)[j][i]->GetHp() <= 0)
								//	(*tileList)[j][i]->SetTileType(TileType::Normal);
							}

						}
					}
				}
			}
		}
	}
}

void CollisionManager::MapObjectCollision(Player* player, RECT* rc, TileMap* tileMap)
{
	vector<vector<Tile*>>* tileList = tileMap->GetTileListPtr();
	vector<vector<MapObject*>>* mapObjectList = tileMap->GetObjectListPtr();

	int x = player->GetX() / TileSize;
	int y = player->GetY() / TileSize;

	float rcWidth = rc->right - rc->left;
	float rcHeight = rc->bottom - rc->top;

	for (int j = y - 1; j <= y + 1; ++j)
	{
		for (int i = x - 1; i <= x + 1; ++i)
		{
			// ���� ���� ��
			if (i >= 0 && i < TileCountX && j >= 0 && j < TileCountY)
			{
				RECT temp;
				RECT mapObjectRc = (*tileList)[j][i]->GetRect();
				if (IntersectRect(&temp, rc, &mapObjectRc))
				{
					float width = temp.right - temp.left;
					float height = temp.bottom - temp.top;

					if ((*mapObjectList)[j][i]->GetMapObjectType() != MapObjectType::None)		// None�� �ƴ϶�� ������ ����
					{
						if (width > height)
						{
							if (temp.top == (*mapObjectList)[j][i]->GetRect().top)	// �÷��̾ ��
							{
								player->SetY((*mapObjectList)[j][i]->GetRect().top - rcHeight / 2);
								(*mapObjectList)[j][i]->DeductHp();

								//if ((*mapObjectList)[j][i]->GetHp() <= 0)
								//	(*tileList)[j][i]->SetTileType(TileType::Normal);
							}
							else if (temp.bottom == (*mapObjectList)[j][i]->GetRect().bottom)	// �÷��̾ �Ʒ�
							{
								player->SetY((*mapObjectList)[j][i]->GetRect().bottom + rcHeight / 2);
								(*mapObjectList)[j][i]->DeductHp();

								//if ((*mapObjectList)[j][i]->GetHp() <= 0)
								//	(*tileList)[j][i]->SetTileType(TileType::Normal);
							}
						}
						else
						{
							if (temp.left == (*mapObjectList)[j][i]->GetRect().left)	// �÷��̾ ����
							{
								player->SetX((*mapObjectList)[j][i]->GetRect().left - rcWidth / 2);
								(*mapObjectList)[j][i]->DeductHp();

								//if ((*mapObjectList)[j][i]->GetHp() <= 0)
								//	(*tileList)[j][i]->SetTileType(TileType::Normal);
							}
							else if (temp.right == (*mapObjectList)[j][i]->GetRect().right)	// �÷��̾ ������
							{
								player->SetX((*mapObjectList)[j][i]->GetRect().right + rcWidth / 2);
								(*mapObjectList)[j][i]->DeductHp();

								//if ((*mapObjectList)[j][i]->GetHp() <= 0)
								//	(*tileList)[j][i]->SetTileType(TileType::Normal);
							}
						}
					}
				}
			}
		}
	}
}

void CollisionManager::MapObjectCollision(Player* player, RECT* rc, vector<vector<Tile*>>* tileList, vector<vector<MapObject*>>* mapObjectList)
{
	int x = player->GetX() / TileSize;
	int y = player->GetY() / TileSize;

	float rcWidth = rc->right - rc->left;
	float rcHeight = rc->bottom - rc->top;

	for (int j = y - 1; j <= y + 1; ++j)
	{
		for (int i = x - 1; i <= x + 1; ++i)
		{
			// ���� ���� ��
			if (i >= 0 && i < TileCountX && j >= 0 && j < TileCountY)
			{
				RECT temp;
				RECT mapObjectRc = (*tileList)[j][i]->GetRect();
				if (IntersectRect(&temp, rc, &mapObjectRc))
				{
					float width = temp.right - temp.left;
					float height = temp.bottom - temp.top;

					if ((*mapObjectList)[j][i]->GetMapObjectType() != MapObjectType::None)		// None�� �ƴ϶�� ������ ����
					{
						if (width > height)
						{
							if (temp.top == (*mapObjectList)[j][i]->GetRect().top)	// �÷��̾ ��
							{
								//player->SetY((*mapObjectList)[j][i]->GetRect().top - rcHeight / 2);
								(*mapObjectList)[j][i]->DeductHp();

								//if ((*mapObjectList)[j][i]->GetHp() <= 0)
								//	(*tileList)[j][i]->SetTileType(TileType::Normal);
							}
							else if (temp.bottom == (*mapObjectList)[j][i]->GetRect().bottom)	// �÷��̾ �Ʒ�
							{
								//player->SetY((*mapObjectList)[j][i]->GetRect().bottom + rcHeight / 2);
								(*mapObjectList)[j][i]->DeductHp();

								//if ((*mapObjectList)[j][i]->GetHp() <= 0)
								//	(*tileList)[j][i]->SetTileType(TileType::Normal);
							}
						}
						else
						{
							if (temp.left == (*mapObjectList)[j][i]->GetRect().left)	// �÷��̾ ����
							{
								//player->SetX((*mapObjectList)[j][i]->GetRect().left - rcWidth / 2);
								(*mapObjectList)[j][i]->DeductHp();

								//if ((*mapObjectList)[j][i]->GetHp() <= 0)
								//	(*tileList)[j][i]->SetTileType(TileType::Normal);
							}
							else if (temp.right == (*mapObjectList)[j][i]->GetRect().right)	// �÷��̾ ������
							{
								//player->SetX((*mapObjectList)[j][i]->GetRect().right + rcWidth / 2);
								(*mapObjectList)[j][i]->DeductHp();

								//if ((*mapObjectList)[j][i]->GetHp() <= 0)
								//	(*tileList)[j][i]->SetTileType(TileType::Normal);
							}
						}
					}
				}
			}
		}
	}
}

void CollisionManager::MapObjectCollision(RECT* rc, vector<vector<MapObject*>>* mapObjectList, int tileIndexX, int tileIndexY)
{
	RECT temp;
	MapObject* mapObject = (*mapObjectList)[tileIndexY][tileIndexX];
	RECT mapObjectRc = mapObject->GetRect();

	if (IntersectRect(&temp, rc, &mapObjectRc))
		mapObject->DeductHp();
}

GameObject * CollisionManager::ItemCollision(RECT* rc)
{
	vector<GameObject*> vecItem = OBJECTMANAGER->GetObjectList(ObjectLayer::ITEM);
	vector<GameObject*>::iterator iter = vecItem.begin();

	for (; iter != vecItem.end();)
	{
		RECT temp;

		if (IntersectRect(&temp, rc, &(*iter)->GetRect()))
		{
			return (*iter);
		}
		else
			++iter;
	}

	return nullptr;
}
