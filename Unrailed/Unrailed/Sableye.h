#pragma once
#include "Enemy.h"

class Sableye : public Enemy
{
	vector<GameObject*> mVecItem;
	GameObject* mTarget;
	bool mIsExistTarget;

public:
	void Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	void ReadyAnimation();
	void SetAnimation();

public:
	bool GetExistTarget() { return mIsExistTarget; }

	void SetExistTarget(bool b) { mIsExistTarget = b; }
};

