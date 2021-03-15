#pragma once
#include "GameObject.h"

enum Direction { Left, Right, Up, Down, DirEnd };

class Image;
class Unit : public GameObject
{
protected:
	Image* mImage;
	Direction mDirection;

public:
	virtual void Init() override;
	virtual void Release()override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

public:
	// Get
	inline Direction GetDirection() { return mDirection; }
	
	//Set
	inline void SetDirection(Direction dir) { mDirection = dir; }
};

