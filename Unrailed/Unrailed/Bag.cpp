#include "pch.h"
#include "Bag.h"
#include "BagItem.h"

void Bag::Update(int ownerX, int ownerY)
{
	for (int i = 0; i < mBagItemList.size(); ++i)
	{
		int x = ownerX - 24 + 3 + 12 * (i % 2);
		int y = ownerY - 24 - 12 - 12 * (i / 2);

		mBagItemList[i]->SetLocation(x, y);
	}
}

void Bag::Render(HDC hdc)
{
	for (int i = 0; i < mBagItemList.size(); ++i)
	{
		mBagItemList[i]->Render(hdc);
	}
}