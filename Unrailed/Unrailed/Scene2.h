#pragma once
#include "Scene.h"
#include "Voltorb.h"
#include "Machop.h"
#include "Abra.h"

class Scene2 : public Scene
{
	vector <Voltorb*> mVoltorbVec;
	Abra* mAbra;
	Machop* mMachop;

public:
	void Init()override;
	void Release()override;
	void Update()override;
	void Render(HDC hdc)override;
};

