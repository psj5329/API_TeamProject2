#pragma once
#include "Enemy.h"

class Sableye : public Enemy
{
	vector<GameObject*> mVecItem;
	//GameObject* mItem;
	GameObject* mTarget;
	bool mIsExistTarget;
	bool mIsRunAway;
	int mItemCount;
	float mDistance;
	Image* mDustImage;
	int mDustFrameX;
	float mDustFrameTime;
	float mAlpha;

public:
	void Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	void MoveToOre();
	void ReadyAnimation();
	void SetAnimation();
	void SetAnimation(EnemyState state);
	void StealOre();
	void NextDir();

public:
	bool GetExistTarget() { return mIsExistTarget; }

	void SetExistTarget(bool b) { mIsExistTarget = b; }
};

