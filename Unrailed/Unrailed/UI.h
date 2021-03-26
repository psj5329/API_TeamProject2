#pragma once
#include "GameObject.h"

class Image;

class UI : public GameObject
{
protected:
	Image* mImage;

public:
	virtual void Init() = 0;
	virtual void Release() = 0;
	virtual void Update() = 0;
	virtual void Render(HDC hdc) = 0;
};