#pragma once
#include "Unit.h"

class Enemy : public Unit
{
	int mHp;		// 어차피 타격수에 따라 죽는다

public:
	virtual void Init() override;
	virtual void Release()override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

public:
	// Get
	int GetHp() { return mHp; }

	// Set
	void SetHp(int hp) { mHp = hp; }

	void DamagedHp() { mHp -= 1; }
};

