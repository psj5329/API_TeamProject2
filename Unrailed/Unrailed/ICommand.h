#pragma once
#include "Tile.h"
#include "Image.h"
#include "MapToolScene.h"
#include "MapObject.h"

class Tile;
class ICommand
{
public:
	virtual void Execute() = 0;
	virtual void Reset() = 0;
};

class IBrushTile : public ICommand
{
	Tile* mTargetTile;

	wstring mSaveImageKey;
	int mSaveFrameX;
	int mSaveFrameY;
	//TilePallete mSavePallete;

	TilePallete mExecutePalleteData;


public:
	IBrushTile(Tile* tile, TilePallete executeData)
	{
		mTargetTile = tile;
		mExecutePalleteData = executeData;


		if (tile->GetImage() != nullptr)
			mSaveImageKey = tile->GetImage()->GetKeyName();
		mSaveFrameX = tile->GetFrameIndexX();
		mSaveFrameY = tile->GetFrameIndexY();
	}
	void Execute()
	{
		mTargetTile->SetImage(ImageManager::GetInstance()->FindImage(mExecutePalleteData.image->GetKeyName()));
		mTargetTile->SetFrameIndexX(mExecutePalleteData.frameX);
		mTargetTile->SetFrameIndexY(mExecutePalleteData.frameY);
	}
	void Reset()override
	{
		mTargetTile->SetImage(ImageManager::GetInstance()->FindImage(mSaveImageKey));
		mTargetTile->SetFrameIndexX(mSaveFrameX);
		mTargetTile->SetFrameIndexY(mSaveFrameY);
	}
	
};

class ISetTileType : public ICommand
{
	Tile* mTargetTile;

	TileType mSaveType;
	//TilePallete mSavePallete;

	TileType mExecuteType;


public:
	ISetTileType(Tile* tile, TileType type)
	{
		mTargetTile = tile;
		mExecuteType = type;
		mSaveType = tile->GetTileType();
	}



	void Execute()
	{
		mTargetTile->SetTileType(mExecuteType);
	}
	void Reset()override
	{
		//mTargetTile->SetImage(ImageManager::GetInstance()->FindImage(mSaveImageKey));
		//mTargetTile->SetFrameIndexX(mSaveFrameX);
		//mTargetTile->SetFrameIndexY(mSaveFrameY);
		//mTargetTile->SetTileType(mSaveType);
	}

};


class IPlaceObject : public ICommand
{
	MapObject* mTargetObject;
	
	wstring mSaveImageKey;
	int mSaveFrameX;
	int mSaveFrameY;
	int mSaveType;
	//TilePallete mSavePallete;

	MapObjectPallete mExecutePalleteData;

public:
	IPlaceObject(MapObject* object, MapObjectPallete executeData)
	{
		mTargetObject = object;
		mExecutePalleteData = executeData;


		if (object->GetImage() != nullptr)
			mSaveImageKey = object->GetImage()->GetKeyName();
		mSaveFrameX = object->GetFrameIndexX();
		mSaveFrameY = object->GetFrameIndexY();
		mSaveType = (int)object->GetMapObjectType();

	}
	void Execute()
	{
		mTargetObject->SetImage(ImageManager::GetInstance()->FindImage(mExecutePalleteData.image->GetKeyName()));
		mTargetObject->SetFrameIndexX(mExecutePalleteData.frameX);
		mTargetObject->SetFrameIndexY(mExecutePalleteData.frameY);
		mTargetObject->SetObjectType(mExecutePalleteData.type);
	}
	void Reset()override
	{
		mTargetObject->SetImage(ImageManager::GetInstance()->FindImage(mSaveImageKey));
		mTargetObject->SetFrameIndexX(mSaveFrameX);
		mTargetObject->SetFrameIndexY(mSaveFrameY);
	}

};