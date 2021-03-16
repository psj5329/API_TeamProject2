#pragma once
#include "Scene.h"
#include "Tile.h"
#include "Image.h"
#include "ICommand.h"
#include "MapObject.h"



enum class CurrentPallete
{
	Tile,
	Object,
	Type,
	Erase
};

class ICommand;



class MapToolScene : public Scene
{
	//¸¸µå´Â Å¸ÀÏµé
	vector <vector <Tile*>> mTileList;
	//¸¸µå´Â ¿ÉÁ§
	vector <vector <MapObject*>> mMapObjectList;


	//ÆÈ·¡Æ®
	vector <vector <TilePallete>> mPallete;
	//Å¸ÀÔ ÆÈ·¿
	vector <TypePallete> mTypePallete;
	//¸Ê¿ÀºêÁ§Æ® ÆÈ·¿
	vector <vector <MapObjectPallete>> mObjectPallete;

	TilePallete mCurrentTile;
	TileType mCurrentType;
	MapObjectPallete mCurrentObject;

	CurrentPallete mCurrentPallete;


	class Button* mSaveButton;
	class Button* mLoadButton;
	class Button* mUndoButton;
	class Button* mRightArrowButton;
	class Button* mRightArrowButton2;
	class Button* mEraseButton;

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


	void EraseButton();
	void SwitchObjectPallete();
	void SwitchTilePallete();

	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)override;

};

