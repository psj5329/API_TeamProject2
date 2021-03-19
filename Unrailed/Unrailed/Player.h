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
	Ditto, // ��Ÿ��
	Charmander, // ���̸�
	Chikorita, // ġ�ڸ�Ÿ
	Totodile, // ������
	FormEnd
};

class Image;
class Animation;
class Tile;
class MapObject;

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

	int mNextTileX; // �ݵ�� ���� ���� �ƴ�(���� ������ ���� ����)
	int mNextTileY; // �ݵ�� ���� ���� �ƴ�(���� ������ ���� ����)

	vector<vector<Tile*>>* mTileListPtr;
	vector<vector<MapObject*>>* mMapObjectListPtr;

	bool mIsAttackingTemp;

	vector<GameObject*> mItemList;

public:
	Player(const string& name, float x, float y);

	void Init()override;
	void Release()override;
	void Update()override;
	void Render(HDC hdc)override;

	void InitAnimation();

	void SafeDeleteAnimation();

	void InputDirectionKey();
	void InputSpaceKey();
	void CheckNextTile();
	void ChangeForm(); // �ӽ�
	void ChangeCurrentAnimation();

	void RenderTestText(HDC hdc);

public:
	RECT* GetColBoxPtr(void) { return &mColBox; }

	void SetTileListPtr(vector<vector<Tile*>>* tileListPtr) { mTileListPtr = tileListPtr; }
	void SetMapObjectListPtr(vector<vector<MapObject*>>* mapObjectListPtr) { mMapObjectListPtr = mapObjectListPtr; }
};