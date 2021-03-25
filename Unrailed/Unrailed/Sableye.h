#pragma once
#include "Enemy.h"

class Sableye : public Enemy
{
	vector<GameObject*> mVecItem;
	GameObject* mItem;
	GameObject* mTarget;
	bool mIsExistTarget;

public:
	void Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	void MoveToOre();
	void ReadyAnimation();
	void SetAnimation();

public:
	bool GetExistTarget() { return mIsExistTarget; }

	void SetExistTarget(bool b) { mIsExistTarget = b; }
};

