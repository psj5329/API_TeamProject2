#pragma once
#include "Unit.h"

class Enemy : public Unit
{

public:
	virtual void Init() override;
	virtual void Release()override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
};

