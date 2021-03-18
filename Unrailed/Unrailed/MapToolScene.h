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
	Image image;
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

	//���콺���� ǥ����ģ��
	MouseRect mMouse;

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

