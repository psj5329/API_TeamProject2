#pragma once
#include "Enemy.h"

class Sableye : public Enemy
{

public:
	void Init();
	void Release();
	void Update();
	void Render(HDC hdc);
};

