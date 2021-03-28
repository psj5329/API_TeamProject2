#pragma once
#include "Scene.h"
#include "Tile.h"
#include "Image.h"
#include "ICommand.h"
#include "MapObject.h"
#include "Hut.h"


enum class CurrentPallete
{
	Tile,
	Object,
	Type,
	Erase,
	Hut
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

struct AreaSelect
{
	vector <Tile*> selectedTileList;
	bool isSelecting;
	POINT startIndex;	//마우스로 고른
	POINT endIndex;		//마우스로 고른
	RECT selectedArea;
	POINT leftTopIndex;	//서순
	POINT rightBottomIndex;		//서순
	int YTileCount;	//정해진영역의 y수
	int XTileCount; //정해진영역의 X수
};

struct HutPallete
{
	Image* mImage;
	int positionX;
	int positionY;
	RECT rect;
	int width;
	int height;
	HutType type;
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
	//hut
	HutPallete mHutPallete;
	HutPallete mCurrentHut;
	class Hut mHut;

	TilePallete mCurrentTile;
	TileType mCurrentType;
	MapObjectPallete mCurrentObject;
	CurrentPallete mCurrentPallete;

	//마우스끝에 표시할친구
	MouseRect mMouse;

	//팔레트배경
	PalleteBackground mPalleteBackground;

	//범위선택용 친구
	AreaSelect mAreaSelect;

	Button* mSaveButton;
	Button* mLoadButton;
	Button* mUndoButton;
	Button* mRightArrowButton;
	Button* mRightArrowButton2;
	Button* mRightArrowButton3;
	Button* mEraseButton;
	Button* mFoldButton;
	Button* mUnFoldButton;
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

	void UnFoldButton();
	void FoldButton();
	void EraseButton();
	void SwitchObjectPallete();
	void SwitchTilePallete();
	void SwitchHutPallete();

	//그리기
	void PaintTilesL();
	//많이그리기
	void PaintTilesR();
	void SetLeftTopRightBottom();
	void InitSelectVecter();
	void EmptySelectedVecter();
	void RenderSelectedRect(HDC hdc);

	//마우스
	void InitMouseRect();
	void SetMouseRect();
	void UpdateMouseRect();
	void RenderMouseRect(HDC hdc);
	
	//팔레트
	void InitPalletes();
	void RenderPalletes(HDC hdc);
	
	//팔레트배경
	void InitPalleteBackground();
	void UpdatePalleteBackground();
	void RenderPalleteBackground(HDC hdc);

	//버튼들
	void InitButtons();
	void UpdateButtons();
	void RenderButtons(HDC hdc);
	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)override;



};

