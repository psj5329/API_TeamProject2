#pragma once
#include "Scene.h"
#include "Tile.h"
#include "Image.h"
#include "ICommand.h"
#include "MapObject.h"


#define TileCountX 10
#define TileCountY 10
#define TileSize 48
#define PalleteCountX 12
#define PalleteCountY 6
#define PalleteSize 27
#define TypePalleteCount 3
#define ObjectPalletCountX 6
#define ObjectPalletCountY 3

enum class CurrentPallete
{
	Tile,
	Object,
	Type
};

class ICommand;

class MapToolScene : public Scene
{
	//����� Ÿ�ϵ�
	vector <vector <Tile*>> mTileList;
	//����� ����
	vector <vector <MapObject*>> mMapObjectList;


	//�ȷ�Ʈ
	vector <vector <TilePallete>> mPallete;
	//Ÿ�� �ȷ�
	vector <TypePallete> mTypePallete;
	//�ʿ�����Ʈ �ȷ�
	vector <vector <MapObjectPallete>> mObjectPallete;

	TilePallete mCurrentTile;
	TileType mCurrentType;
	MapObjectPallete mCurrentObject;

	CurrentPallete mCurrentPallete;


	class Button* mSaveButton;
	class Button* mLoadButton;
	class Button* mUndoButton;
	class Button* mRightArrowButton;

	stack<ICommand*> mCommandList;
public:
	void Init()override;
	void Release()override;
	void Update()override;
	void Render(HDC hdc)override;
	void ImageLoad();

	vector<vector <Tile*>>* GetTileListPtr() { return &mTileList; }
	vector<vector <MapObject*>>* GetObjectListPtr() { return &mMapObjectList; }

private:
	void Save();
	void Load();

	void PushCommand(ICommand* command);
	void Undo();
	void Redo();
};

