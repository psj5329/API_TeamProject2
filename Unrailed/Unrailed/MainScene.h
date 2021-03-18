#pragma once
#include "Scene.h"

class MainScene : public Scene
{
public:
	void Init()override;
	void Release()override;
	void Update()override;
	void Render(HDC hdc)override;
};

