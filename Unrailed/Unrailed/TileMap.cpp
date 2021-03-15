#include "pch.h"
#include "TileMap.h"
#include "Tile.h"
#include "MapToolScene.h"


void TileMap::Init(int tileX, int tileY, int tileSize)
{
	mTileCountX = tileX;
	mTileCountY = tileY;

	//∫Û∏  ¿Œ¿’
	for (int y = 0; y < mTileCountY; ++y)
	{
		mTileList.push_back(vector <Tile*>());
		for (int x = 0; x < mTileCountX; ++x)
		{
			//∫§≈Õ
			Tile* tempTile = new Tile
			(
				nullptr,
				tileSize * x,
				tileSize * y,
				tileSize,
				tileSize,
				Random::GetInstance()->RandomInt(2),
				0,
				0
			);

			mTileList[y].push_back(tempTile);

		}
	}
	//∫Û∏ ø…¡ß ¿Œ¿’
	for (int y = 0; y < mTileCountY; ++y)
	{
		mMapObjectList.push_back(vector <MapObject*>());
		for (int x = 0; x < mTileCountX; ++x)
		{

			//∫§≈Õ
			MapObject* tempObject = new MapObject
			(
				nullptr,
				tileSize * x,
				tileSize * y,
				tileSize,
				tileSize,
				0,
				0,
				0
			);
			mMapObjectList[y].push_back(tempObject);

		}
	}
}

void TileMap::Release()
{

}

void TileMap::Update()
{
	
}

void TileMap::Render(HDC hdc)
{
	//≈∏¿œµÈ
	for (int y = 0; y < mTileCountY; ++y)
	{
		for (int x = 0; x < mTileCountX; ++x)
		{
			mTileList[y][x]->Render(hdc);
		}
	}

	//ø…¡ß
	for (int y = 0; y < mMapObjectList.size(); ++y)
	{
		for (int x = 0; x < mMapObjectList[y].size(); ++x)
		{
			mMapObjectList[y][x]->Render(hdc);
		}
	}

}

void TileMap::LoadMap()
{
	ifstream loadStream(L"../Data/Test.txt");
	if (loadStream.is_open())
	{
		for (int y = 0; y < mTileCountY; ++y)
		{
			for (int x = 0; x < mTileCountX; ++x)
			{
				string key;
				int frameX;
				int frameY;
				int type;
				string objectKey;
				int objectFrameX;
				int objectFrameY;
				string buffer;


				getline(loadStream, buffer, ',');
				key = buffer;
				getline(loadStream, buffer, ',');
				frameX = stoi(buffer);
				getline(loadStream, buffer, ',');
				frameY = stoi(buffer);
				getline(loadStream, buffer, ',');
				type = stoi(buffer);

				//ø…¡ß
				getline(loadStream, buffer, ',');
				objectKey = buffer;
				getline(loadStream, buffer, ',');
				objectFrameX = stoi(buffer);
				getline(loadStream, buffer);
				objectFrameY = stoi(buffer);


				wstring wstr;
				wstr.assign(key.begin(), key.end());
				mTileList[y][x]->SetImage(ImageManager::GetInstance()->FindImage(wstr));
				mTileList[y][x]->SetFrameIndexX(frameX);
				mTileList[y][x]->SetFrameIndexY(frameY);
				mTileList[y][x]->SetTileType((TileType)type);

				if (objectKey == "none")
					mMapObjectList[y][x]->SetImage(nullptr);
				else
				{
					wstr.assign(objectKey.begin(), objectKey.end());
					mMapObjectList[y][x]->SetImage(ImageManager::GetInstance()->FindImage(wstr));
				}
				mMapObjectList[y][x]->SetFrameIndexX(objectFrameX);
				mMapObjectList[y][x]->SetFrameIndexY(objectFrameY);
			}
		}
	}
}