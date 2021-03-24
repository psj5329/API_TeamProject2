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
	bool isOn;
};

struct PalleteBackground
{
	Image* image;
	float positionX;
	float positionY;
	float sizeX;
	float sizeY;
	RECT rect;
	bool isOpen;
};
class ICommand;
class Button;


class MapToolScene : public Scene
{
	int mYTileCount;
	int mXTileCount;

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

	//�ȷ�Ʈ���
	PalleteBackground mPalleteBackground;

	Button* mSaveButton;
	Button* mLoadButton;
	Button* mUndoButton;
	Button* mRightArrowButton;
	Button* mRightArrowButton2;
	Button* mEraseButton;
	Button* mFoldButton;
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

	void FoldButton();
	void EraseButton();
	void SwitchObjectPallete();
	void SwitchTilePallete();

	//���콺
	void InitMouseRect();
	void SetMouseRect();
	void UpdateMouseRect();
	void RenderMouseRect(HDC hdc);
	
	//�ȷ�Ʈ���
	void InitPalleteBackground();
	void UpdatePalleteBackground();
	void RenderPalleteBackground(HDC hdc);

	//��ư��
	void InitButtons();
	void UpdateButtons();
	void RenderButtons(HDC hdc);
	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)override;

};

