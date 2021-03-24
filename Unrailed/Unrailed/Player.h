#pragma once
#include "GameObject.h"
#include "Tile.h"

enum DirectionEight
{
	Down,
	Up,
	Left,
	Right,
	LeftDown,
	RightDown,
	LeftUp,
	RightUp,
	DirectionEightEnd
};

enum State
{
	Idle,
	Move,
	StateEnd
};

enum Form
{
	Ditto, // 메타몽
	Chikorita, // 치코리타
	Totodile, // 리아코
	Charmander, // 파이리
	FormEnd
};

enum ItemName
{
	ItemOre,
	ItemTrail,
	ItemNameEnd
};

class InvenItem
{
	ItemName itemName;
	ItemType itemType;

public:
	ItemName GetName() const { return itemName; }
	ItemType GetType() const { return itemType; }
	void SetName(ItemName name) { itemName = name; }
	void SetType(ItemType type) { itemType = type; }
};

class Image;
class Animation;
class MapObject;
class TrailManager;

class Player : public GameObject
{
	Image* mImage;

	Animation* mDownIdleAnimation;
	Animation* mUpIdleAnimation;
	Animation* mLeftIdleAnimation;
	Animation* mRightIdleAnimation;
	Animation* mLDIdleAnimation;
	Animation* mRDIdleAnimation;
	Animation* mLUIdleAnimation;
	Animation* mRUIdleAnimation;

	Animation* mDownMoveAnimation;
	Animation* mUpMoveAnimation;
	Animation* mLeftMoveAnimation;
	Animation* mRightMoveAnimation;
	Animation* mLDMoveAnimation;
	Animation* mRDMoveAnimation;
	Animation* mLUMoveAnimation;
	Animation* mRUMoveAnimation;

	Animation* mCurrentAnimation;

	RECT mColBox;

	DirectionEight mDir;
	State mState;

	float mSpeed;

	int mInputType;

	Form mForm;

	float mChangeT;

	float mIsDirectionKeyDown;

	int mTileX;
	int mTileY;

	int mNextTileX;
	int mNextTileY;

	vector<vector<Tile*>>* mTileListPtr;
	vector<vector<MapObject*>>* mMapObjectListPtr;

	bool mIsAttackingTemp;

	bool mIsGettingItemThisFrame;

	TrailManager* mTrailManager;

	bool mIsChangable;

	vector<InvenItem*> mInvenItemList;

public:
	Player(const string& name, float x, float y);

	void Init()override;
	void Release()override;
	void Update()override;
	void Render(HDC hdc)override;

	void InitAnimation();

	void SafeDeleteAnimation();

	void InputDirectionKey();
	void Move();
	void InputSpaceKey(); // Space key: attack gameobject
	void CheckNextTile();
	void InputZKey(); // 'Z' key: pick up item 
	void InputXKey(); // 'X' key: throw item
	void InputCKey(); // 'C' key: change form // Loop Order: normal -> leaf -> water -> fire -> normal -> ... //
	bool CheckTileType(TileType tileType);
	void ChangeCurrentAnimation();

	void RenderTestText(HDC hdc);

public:
	RECT* GetColBoxPtr(void) { return &mColBox; }
	DirectionEight GetDir(void) const { return mDir; }
	Form GetForm(void) const { return mForm; }

	void SetTileListPtr(vector<vector<Tile*>>* tileListPtr) { mTileListPtr = tileListPtr; }
	void SetMapObjectListPtr(vector<vector<MapObject*>>* mapObjectListPtr) { mMapObjectListPtr = mapObjectListPtr; }
	void SetTrailManagerPtr(TrailManager* trailManager) { mTrailManager = trailManager; }
};