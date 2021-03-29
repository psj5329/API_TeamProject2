#pragma once
#include "GameObject.h"

enum UnitDirection { LEFT, RIGHT, UP, DOWN, DIREND };

class Image;
class Unit : public GameObject
{
protected:
	Image* mImage;
	UnitDirection mDirection;

	int mHp;

public:
	virtual void Init() override;
	virtual void Release()override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

public:
	// Get
	inline UnitDirection GetDirection() { return mDirection; }
	inline int GetHp() { return mHp; }
	
	//Set
	inline void SetDirection(UnitDirection dir) { mDirection = dir; }
	inline void SetHp(int hp) { mHp = hp; }

	inline void DamagedHp() { mHp -= 1; }
};