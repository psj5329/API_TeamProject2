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

enum PlayerState
{
	Idle,
	Move,
	Attack,
	PlayerStateEnd
};

enum Form
{
	Ditto,
	Chikorita,
	Totodile,
	Charmander,
	FormEnd
};

class Image;
class Animation;
class MapObject;
class TrailManager;
class Inven;
class BagItem;

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

	Animation* mDownAttackChikoAnimation;
	Animation* mUpAttackChikoAnimation;
	Animation* mLeftAttackChikoAnimation;
	Animation* mRightAttackChikoAnimation;
	Animation* mLDAttackChikoAnimation;
	Animation* mRDAttackChikoAnimation;
	Animation* mLUAttackChikoAnimation;
	Animation* mRUAttackChikoAnimation;

	Animation* mDownAttackTotoAnimation;
	Animation* mUpAttackTotoAnimation;
	Animation* mLeftAttackTotoAnimation;
	Animation* mRightAttackTotoAnimation;
	Animation* mLDAttackTotoAnimation;
	Animation* mRDAttackTotoAnimation;
	Animation* mLUAttackTotoAnimation;
	Animation* mRUAttackTotoAnimation;

	Animation* mDownAttackCharAnimation;
	Animation* mUpAttackCharAnimation;
	Animation* mLeftAttackCharAnimation;
	Animation* mRightAttackCharAnimation;
	Animation* mLDAttackCharAnimation;
	Animation* mRDAttackCharAnimation;
	Animation* mLUAttackCharAnimation;
	Animation* mRUAttackCharAnimation;

	Animation* mCurrentAnimation;

	RECT mColBox;
	RECT mRangeBox;

	DirectionEight mDir;
	PlayerState mState;
	Form mForm;

	int mTileX;
	int mTileY;
	int mNextTileX;
	int mNextTileY;
	int mRangeX;
	int mRangeY;

	vector<vector<Tile*>>* mTileListPtr;
	vector<vector<MapObject*>>* mMapObjectListPtr;
	TrailManager* mTrailManager;
	Inven* mInven;
	vector<BagItem*>* mBagItemListPtr;

	float mSpeed;

	int mInputType;

	float mChangeT;

	float mIsDirectionKeyDown;

	bool mIsAttackingTemp;

	bool mIsGettingItemThisFrame;

	bool mIsChangable;

	bool mIsInfoOn;

	int mTileCountX;
	int mTileCountY;

public:
	Player(const string& name, float x, float y);

	void Init()override;
	void Release()override;
	void Update()override;
	void Render(HDC hdc)override;

	void InitAnimation();
	void SetIdleAnimation();

	void SafeDeleteAnimation();

	void InputDirectionKey();
	void Move();
	void InputSpaceKey(); // Space key: attack
	void CheckNextTile();
	void InputZKey(); // 'Z' key: pick up item 
	void InputXKey(); // 'X' key: throw item
	void InputCKey(); // 'C' key: change form // Loop Order: normal -> leaf -> water -> fire -> normal -> ... //
	void InputVKey(); // 'V' key: turn item
	void InputLKey(); // 'L' key: turn on/off gizmo
	void InputCheatKey(); // Cheat key for test
	bool CheckTileType(TileType tileType);
	void CheckRange();
	void ChangeCurrentAnimation();

	void RenderTestText(HDC hdc);

public:
	RECT* GetColBoxPtr(void) { return &mColBox; }
	DirectionEight GetDir(void) const { return mDir; }
	Form GetForm(void) const { return mForm; }

	void SetTileListPtr(vector<vector<Tile*>>* tileListPtr) { mTileListPtr = tileListPtr; }
	void SetMapObjectListPtr(vector<vector<MapObject*>>* mapObjectListPtr) { mMapObjectListPtr = mapObjectListPtr; }
	void SetTrailManagerPtr(TrailManager* trailManager) { mTrailManager = trailManager; }
	void SetTileCount(int tileCountX, int tileCountY) { mTileCountX = tileCountX; mTileCountY = tileCountY; }
};