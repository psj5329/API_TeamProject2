#pragma once
#include "GameObject.h"

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
	Charmander, // 파이리
	Chikorita, // 치코리타
	Totodile, // 리아코
	FormEnd
};

class Image;
class Animation;
class Tile;
class MapObject;

class Player : public GameObject
{
	Image* mImage;
	//Animation* mIdleAnimation;
	//Animation* mRunAnimation;

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

	Animation* mCurrentAnimation;	//현재 애니메이션

	DirectionEight mDir;
	State mState;

	float mSpeed;

	int mInputType;

	Form mForm;

	float mChangeT;

	float mIsDirectionKeyDown;

	int mTileX;
	int mTileY;

	int mNextTileX; // 반드시 다음 것은 아님(순서 때문에 값이 변함)
	int mNextTileY; // 반드시 다음 것은 아님(순서 때문에 값이 변함)

	vector<vector<Tile*>>* mTileListPtr;
	vector<vector<MapObject*>>* mMapObjectListPtr;

	bool mIsAttackingTemp;

public:
	Player(const string& name, float x, float y);

	void Init()override;
	void Release()override;
	void Update()override;
	void Render(HDC hdc)override;

	void InitAnimation();

	void InputDirectionKey();
	void InputDiggingKey();

	void ChangeForm(); // 임시

	void SafeDeleteAnimation();
  
public:
	void SetTileListPtr(vector<vector<Tile*>>* tileListPtr) { mTileListPtr = tileListPtr; }
	void SetMapObjectListPtr(vector<vector<MapObject*>>* mapObjectListPtr) { mMapObjectListPtr = mapObjectListPtr; }
};