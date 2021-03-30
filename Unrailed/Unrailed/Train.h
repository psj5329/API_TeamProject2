#pragma once
#include "GameObject.h"
#include "Trail.h"
#include "Jigglypuff.h"

enum class State : int
{
	Move,
	Sleep,
	Intercept,
	Synthesis,
	Explode,
	Exploding,
	Hurt
};

class Train : public GameObject
{
protected:
	vector <vector <Trail*>> mTrailList;

	Jigglypuff* mJigglypuff;

	State mState;
	Direction mDirection;
	float mSpeedX;
	float mSpeedY;
	float mSynthesisCoolTime;
	float mExplosionTimer;
	float mStartTimer;
	float mSleepTimer;
	float mDustFrameTime;
	float mAlpha;

	int mTargetX;
	int mTargetY;
	int mCurrentX;
	int mCurrentY;
	int mTrailCount;
	int mOreCount;
	
	bool mOreBroken;
	bool mIsExplode;
	bool mIsHurt;
	bool mIsSynthesis;

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
	void SetSpeed();
	void SetIsExplode(bool explode) { mIsExplode = explode; }
	void SetIsHurt(bool hurt) { mIsHurt = hurt; }
	void SetJigglypuff(GameObject* object) { mJigglypuff = dynamic_cast<Jigglypuff*>(object); }
	
	bool CheckNextIsPassed();
	bool CheckNextTrailType();
	bool CheckTile();
	bool CheckTileEdge();
	bool CheckJigglypuff();
	
	bool GetIsHurt() { return mIsHurt; }
	bool GetIsExplode() { return mIsExplode; }
};