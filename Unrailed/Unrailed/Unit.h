#pragma once
#include "GameObject.h"

class Image;
class Unit : public GameObject
{
protected:
	Image* mImage;
	Direction mDirection;

	int mHp;

public:
	virtual void Init() override;
	virtual void Release()override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

public:
	// Get
	inline Direction GetDirection() { return mDirection; }
	inline int GetHp() { return mHp; }
	
	//Set
	inline void SetDirection(Direction dir) { mDirection = dir; }
	inline void SetHp(int hp) { mHp = hp; }

	inline void DamagedHp() { mHp -= 1; }
};