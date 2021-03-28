#pragma once
#include "GameObject.h"

enum class TrailDirection : int
{
	Down,
	Up,
	Left,
	Right
};
//
//enum class ItemType
//{
//	None,
//	Green,
//	Blue,
//	Red
//};
//
class Image;
class Animation;
class Trail :
	public GameObject
{
	Image* mImage;
	TrailDirection mDirection;
	ItemType mTrailType;

	Animation* mDown;
	Animation* mUp;
	Animation* mLeft;
	Animation* mRight;
	Animation* mCurrentAnimation;

	bool mIsConnected;
	bool mIsTail;
	int mOrder;

	bool mIsPassed;

	bool mIsFinish;

public:
	void Init(int x, int y, int type, int direction);
	void Init()override {};
	void Release()override;
	void Update()override;
	void Render(HDC hdc)override;
	void AnimationInit(Animation* down, Animation* up, Animation* left, Animation* right);

	TrailDirection GetDirection() const{ return mDirection; }
	int GetDirectionInt() const{ return (int)mDirection; }
	ItemType GetTrailType() const{ return mTrailType; }
	int GetTrailTypeInt() const{ return (int)mTrailType; }
	bool GetIsConnected() const { return mIsConnected; }
	bool GetIsTail() const { return mIsTail; }
	bool GetIsPassed()const { return mIsPassed; }
	int GetOrder()const { return mOrder; }
	bool GetisFinish()const { return mIsFinish; }

	void SetTrailType(ItemType type);
	void SetDirection(int dir);
	void SetIsConnected(bool connected) { mIsConnected = connected; }
	void SetIsTail(bool tail) { mIsTail = tail; }
	void SetIsPassed(bool passed) { mIsPassed = passed; }
	void SetOrder(int a) { mOrder = a; }
	void SetisFinish(bool finish) { mIsFinish = finish; }

	void Turn();//플레이어의 돌리기
	ItemType PickUp();

private:
	bool IsInCamera();
};