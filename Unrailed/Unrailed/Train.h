#pragma once
#include "GameObject.h"
#include "Trail.h"

enum class Direction : int
{
	Down,
	Up,
	Left,
	Right
};

enum class State : int
{
	Move,
	Shock,
	Sleep,
	Intercept,
	Synthesis,
	Explode,
	Exploding
};

class Train : public GameObject
{
protected:
	vector <vector <Trail*>> mTrailList;

	State mState;
	Direction mDirection;
	float mSpeed;
	float mSpeedX;
	float mSpeedY;
	int mTargetX;
	int mTargetY;
	int mCurrentX;
	int mCurrentY;
	float mSynthesisCoolTime;
	bool mStop;
	bool mIsExplode;
	bool mIsSynthesis;
	int mTrailCount;
	int mOreCount;
	bool mOreBroken;
	float mExplosionTimer;

	class Image* mCurrentImage;
	class Image* mImage;
	class Image* mExplodeImage;
	class Animation* mCurrentAnimation;
	class Animation* mExplode;

	//뒤의 포인터
	Train* mNextTrain;
	
public:
	virtual void Init()override = 0;
	virtual void Release()override = 0;
	virtual void Update()override = 0;
	virtual void Render(HDC hdc)override = 0;

	virtual void ReadyAnimation() = 0;
	virtual void SetAnimation() = 0;
	virtual void EndExplode() = 0;

	void SetState(State state) { mState = state; }
	void SetTrail(vector <vector <Trail*>>* trail) { mTrailList = *trail; }
	void SetNextTrain(Train* next) { mNextTrain = next; }
	void SetTarget();
	bool CheckTile();
	bool CheckTileEdge();
	void SetSpeed();
	bool CheckNextIsPassed();
	bool CheckNextTrailType();
	
	bool GetIsExplode() { return mIsExplode; }
	void SetIsExplode(bool explode) { mIsExplode = explode; }

};