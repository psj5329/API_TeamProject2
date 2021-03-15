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

public:
	Player(const string& name, float x, float y);

	void Init()override;
	void Release()override;
	void Update()override;
	void Render(HDC hdc)override;

	void InitAnimation();

	void InputDirectionKey();

	void ChangeForm(); // 임시

	void SafeDeleteAnimation();
};