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
	Explode
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
	int mLevel;
	bool mStop;
	bool mIsExplode;
	bool mIsSynthesis;
	int mTrailCount;
	int mOreCount;
	bool mOreBroken;

	class Image* mCurrentImage;
	class Image* mImage;
	class Image* mExplodeImage;
	class Animation* mCurrentAnimation;
	class Animation* mExplode;

public:
	//Train();

	virtual void Init()override = 0;
	virtual void Release()override = 0;
	virtual void Update()override = 0;
	virtual void Render(HDC hdc)override = 0;

	virtual void ReadyAnimation() = 0;
	virtual void SetAnimation() = 0;
	virtual void EndExplode() = 0;

	void SetTrail(vector <vector <Trail*>>* trail) { mTrailList = *trail; }
	void SetTarget();
	bool CheckTile();
	bool CheckTileEdge();
	void SetSpeed();
	bool CheckNextIsPassed();
	bool CheckNextTrailType();
};