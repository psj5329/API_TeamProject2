#pragma once
#include "GameObject.h"

enum class HutType : int
{
	None,
	ChikoritaHut,
	TotodileHut,
	CharmanderHut,
	MachopHut,
	NormalHut
};

class Hut :
	public GameObject
{
	class Image* mImage;
	HutType mType;

public:
	void Init()override {};
	void Init(float x, float y, HutType type);
	void Release() override;
	void Update()override;
	void Render(HDC hdc)override;

	void SetType(HutType type);
	int GetHutTypeInt() { return (int)mType; }
private:
};

