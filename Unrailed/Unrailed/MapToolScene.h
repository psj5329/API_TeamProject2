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

struct MouseRect
{
	Image* image;
	float positionX;
	float positionY;
	float sizeX;
	float sizeY;
	int indexX;
	int indexY;
	RECT rect;
	int frameX;
	int frameY;
};

class ICommand;
class Button;


class MapToolScene : public Scene
{
	int mYTileCount;
	int mXTileCount;

	//만드는 타일들
	vector <vector <Tile*>> mTileList;
	//만드는 옵젝
	vector <vector <MapObject*>> mMapObjectList;


	//팔래트
	vector <vector <TilePallete>> mPallete;
	//타입 팔렛
	vector <TypePallete> mTypePallete;
	//맵오브젝트 팔렛
	vector <vector <MapObjectPallete>> mObjectPallete;

	TilePallete mCurrentTile;
	TileType mCurrentType;
	MapObjectPallete mCurrentObject;
	CurrentPallete mCurrentPallete;

	//마우스끝에 표시할친구
	MouseRect mMouse;

	Button* mSaveButton;
	Button* mLoadButton;
	Button* mUndoButton;
	Button* mRightArrowButton;
	Button* mRightArrowButton2;
	Button* mEraseButton;
	vector <Button*> mSaveButtons;


	stack<ICommand*> mCommandList;
public:
	void Init()override;
	void Release()override;
	void Update()override;
	void Render(HDC hdc)override;
	void ImageLoad();
	void SetMapSize(int y, int x) override {
		mYTileCount = y; mXTileCount = x;
	};
	void OpenLoadWindow()override;

	vector<vector <Tile*>>* GetTileListPtr() { return &mTileList; }
	vector<vector <MapObject*>>* GetObjectListPtr() { return &mMapObjectList; }


private:
	void InitEmptyMap();
	void ReleaseMap();

	void Save(int i);
	void Load(wstring fileName);

	void PushCommand(ICommand* command);
	void Undo();
	void Redo();


	void EraseButton();
	void SwitchObjectPallete();
	void SwitchTilePallete();

	//마우스
	void InitMouseRect();
	void SetMouseRect();
	void UpdateMouseRect();
	void RenderMouseRect(HDC hdc);
	
	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)override;

};

