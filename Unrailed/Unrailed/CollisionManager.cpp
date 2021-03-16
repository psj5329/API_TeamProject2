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

	if (IntersectRect(&temp, rc1, rc2))		// 이거 딱 맞을때는 충돌 아니라해서 불편헌디
		return true;

	return false;

	/*if (rc1->right < rc2->left || rc1->left > rc2->right)return false;
	if (rc1->bottom < rc2->top || rc1->top > rc2->bottom)return false;

	return true;*/
}

void CollisionManager::TileCollision(Player* player, TileMap* tileMap)
{
	vector <vector <Tile*>> tileList = tileMap->GetTileList();

	// 나중에 유찬형꺼 받으면 함수 받아서 하게 고치기
	int x = player->GetX() / TileSize;
	int y = player->GetY() / TileSize;

	for (int j = y - 1; j <= y + 1; ++j)
	{
		for (int i = x - 1; i <= x + 1; ++i)
		{
			// 범위 안일 때
			if (i >= 0 && i < TileCountX && j >= 0 && j < TileCountY)
			{
				RECT temp;
				if (IntersectRect(&temp, &player->GetRect(), &tileList[j][i]->GetRect()))
				{
					float width = temp.right - temp.left;
					float height = temp.bottom - temp.top;

					if (tileList[j][i]->GetTileType() != TileType::Normal)
					{
						if (width > height)
						{
							if (temp.top == tileList[j][i]->GetRect().top)	// 플에이어가 위
								player->SetY(tileList[j][i]->GetRect().top - player->GetSizeY() / 2);
							else if (temp.bottom == tileList[j][i]->GetRect().bottom)	// 플에이어가 아래
								player->SetY(tileList[j][i]->GetRect().bottom + player->GetSizeY() / 2);
						}
						else
						{
							if (temp.left == tileList[j][i]->GetRect().left)	// 플에이어가 왼쪽
								player->SetX(tileList[j][i]->GetRect().left - player->GetSizeX() / 2);
							else if (temp.right == tileList[j][i]->GetRect().right)	// 플에이어가 오른쪽
								player->SetX(tileList[j][i]->GetRect().right + player->GetSizeX() / 2);

						}
					}
				}
			}
			else
			{
				if (j >= 0 && j < TileCountY)
				{
					// x축 방향으로 넘어갈 때
					if (player->GetX() < tileList[j][0]->GetRect().left + player->GetSizeX() / 2)
						player->SetX(tileList[j][0]->GetRect().left + player->GetSizeX() / 2);
					else if (player->GetX() > tileList[j][TileCountX - 1]->GetRect().right - player->GetSizeX() / 2)
						player->SetX(tileList[j][TileCountX - 1]->GetRect().right - player->GetSizeX() / 2);
				}

				if (i >= 0 && i < TileCountX)
				{
					// y축 방향으로 넘어갈 때
					if (player->GetY() < tileList[0][i]->GetRect().top + player->GetSizeY() / 2)
						player->SetY(tileList[0][i]->GetRect().top + player->GetSizeY() / 2);
					else if (player->GetY() > tileList[TileCountY - 1][i]->GetRect().bottom - player->GetSizeY() / 2)
						player->SetY(tileList[TileCountY - 1][i]->GetRect().bottom - player->GetSizeY() / 2);
				}
			}
		}
	}
}

void CollisionManager::MapObjectCollision(Player * player, TileMap * tileMap)
{
	vector <vector <Tile*>> tileList = tileMap->GetTileList();
	vector <vector <MapObject*>> mapObjectList = tileMap->GetMapObjectList();

	// 나중에 유찬형꺼 받으면 함수 받아서 하게 고치기
	int x = player->GetX() / TileSize;
	int y = player->GetY() / TileSize;

	for (int j = y - 1; j <= y + 1; ++j)
	{
		for (int i = x - 1; i <= x + 1; ++i)
		{
			// 범위 안일 때
			if (i >= 0 && i < TileCountX && j >= 0 && j < TileCountY)
			{
				RECT temp;
				if (IntersectRect(&temp, &player->GetRect(), &mapObjectList[j][i]->GetRect()))
				{
					float width = temp.right - temp.left;
					float height = temp.bottom - temp.top;

					if (mapObjectList[j][i]->GetMapObjectType() != MapObjectType::None)		// None이 아니라면 광물이 있음
					{
						if (width > height)
						{
							if (temp.top == mapObjectList[j][i]->GetRect().top)	// 플레이어가 위
							{
								player->SetY(mapObjectList[j][i]->GetRect().top - player->GetSizeY() / 2);
								mapObjectList[j][i]->DamagedHp();

								if (mapObjectList[j][i]->GetHp() <= 0)
								{
									//SafeDelete(mapObjectList[j][i]);
									mapObjectList[j][i]->SetHp(3);
									mapObjectList[j][i]->SetObjectType(0);
									mapObjectList[j][i]->SetImage(nullptr);
									mapObjectList[j][i]->SetActive(false);
									tileList[j][i]->SetTileType(TileType::Normal);
								}
							}
							else if (temp.bottom == mapObjectList[j][i]->GetRect().bottom)	// 플레이어가 아래
							{
								player->SetY(mapObjectList[j][i]->GetRect().bottom + player->GetSizeY() / 2);
								mapObjectList[j][i]->DamagedHp();

								if (mapObjectList[j][i]->GetHp() <= 0)
								{
									//SafeDelete(mapObjectList[j][i]);
									mapObjectList[j][i]->SetHp(3);
									mapObjectList[j][i]->SetObjectType(0);
									mapObjectList[j][i]->SetImage(nullptr);
									mapObjectList[j][i]->SetActive(false);
									tileList[j][i]->SetTileType(TileType::Normal);
								}
							}
						}
						else
						{
							if (temp.left == mapObjectList[j][i]->GetRect().left)	// 플레이어가 왼쪽
							{
								player->SetX(mapObjectList[j][i]->GetRect().left - player->GetSizeX() / 2);
								mapObjectList[j][i]->DamagedHp();

								if (mapObjectList[j][i]->GetHp() <= 0)
								{
									//SafeDelete(mapObjectList[j][i]);
									mapObjectList[j][i]->SetHp(3);
									mapObjectList[j][i]->SetObjectType(0);
									mapObjectList[j][i]->SetImage(nullptr);
									mapObjectList[j][i]->SetActive(false);
									tileList[j][i]->SetTileType(TileType::Normal);
								}
							}
							else if (temp.right == mapObjectList[j][i]->GetRect().right)	// 플레이어가 오른쪽
							{
								player->SetX(mapObjectList[j][i]->GetRect().right + player->GetSizeX() / 2);
								mapObjectList[j][i]->DamagedHp();

								if (mapObjectList[j][i]->GetHp() <= 0)
								{
									//SafeDelete(mapObjectList[j][i]);
									mapObjectList[j][i]->SetHp(3);
									mapObjectList[j][i]->SetObjectType(0);
									mapObjectList[j][i]->SetImage(nullptr);
									mapObjectList[j][i]->SetActive(false);
									tileList[j][i]->SetTileType(TileType::Normal);
								}
							}

						}
					}
				}
			}
			//else
			//{
			//	if (j >= 0 && j < TileCountY)
			//	{
			//		// x축 방향으로 넘어갈 때
			//		if (player->GetX() < tileList[j][0]->GetRect().left + player->GetSizeX() / 2)
			//			player->SetX(tileList[j][0]->GetRect().left + player->GetSizeX() / 2);
			//		else if (player->GetX() > tileList[j][TileCountX - 1]->GetRect().right - player->GetSizeX() / 2)
			//			player->SetX(tileList[j][TileCountX - 1]->GetRect().right - player->GetSizeX() / 2);
			//	}

			//	if (i >= 0 && i < TileCountX)
			//	{
			//		// y축 방향으로 넘어갈 때
			//		if (player->GetY() < tileList[0][i]->GetRect().top + player->GetSizeY() / 2)
			//			player->SetY(tileList[0][i]->GetRect().top + player->GetSizeY() / 2);
			//		else if (player->GetY() > tileList[TileCountY - 1][i]->GetRect().bottom - player->GetSizeY() / 2)
			//			player->SetY(tileList[TileCountY - 1][i]->GetRect().bottom - player->GetSizeY() / 2);
			//	}
			//}
		}
	}
}
