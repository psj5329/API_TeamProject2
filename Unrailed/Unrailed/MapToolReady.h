#pragma once
#include "Scene.h"
class MapToolReady :
	public Scene
{

public:
	void Init()override;
	void Release()override;
	void Update()override;
	void Render(HDC hdc)override;
};

