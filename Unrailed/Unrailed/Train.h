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
	float mSpeed;
	float mSpeedX;
	float mSpeedY;
	int mTargetX;
	int mTargetY;
	int mCurrentX;
	int mCurrentY;
	float mSynthesisCoolTime;
	bool mIsExplode;
	bool mIsSynthesis;
	int mTrailCount;
	int mOreCount;
	bool mOreBroken;
	float mExplosionTimer;
	float mStartTimer;
	float mSleepTimer;

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
	bool CheckJigglypuff();
	
	bool GetIsExplode() { return mIsExplode; }
	void SetIsExplode(bool explode) { mIsExplode = explode; }
	void SetJigglypuff(GameObject* object) { mJigglypuff = dynamic_cast<Jigglypuff*>(object); }
};