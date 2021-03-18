#pragma once
#include "GameObject.h"

enum class TrailDirection : int
{
	Down,
	Up,
	Left,
	Right
};

enum class TrailType
{
	None,
	Green,
	Blue,
	Red
};

class Image;
class Animation;
class Trail :
	public GameObject
{
	Image* mImage;
	TrailDirection mDirection;
	TrailType mTrailType;

	Animation* mDown;
	Animation* mUp;
	Animation* mLeft;
	Animation* mRight;
	Animation* mCurrentAnimation;

	bool mIsConnected;
	bool mIsTail;

public:
	void Init(int x, int y, int type, int direction);
	void Init()override {};
	void Release()override;
	void Update()override;
	void Render(HDC hdc)override;
	void AnimationInit();

	TrailDirection GetDirection() const{ return mDirection; }
	int GetDirectionInt() const{ return (int)mDirection; }
	TrailType GetTrailType() const{ return mTrailType; }
	int GetTrailTypeInt() const{ return (int)mTrailType; }
	bool GetIsConnected() const { return mIsConnected; }
	bool GetIsTail() const { return mIsTail; }

	void SetTrailType(int type);
	void SetDirection(int dir);
	void SetIsConnected(bool connected) { mIsConnected = connected; }
	void SetIsTail(bool tail) { mIsTail = tail; }

	void Turn();//플레이어의 돌리기
	int PickUp();
};