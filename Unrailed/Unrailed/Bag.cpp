#include "pch.h"
#include "Bag.h"
#include "BagItem.h"

void Bag::Release()
{
	for (int i = 0; i < mBagItemList.size(); ++i)
	{
		mBagItemList.erase(mBagItemList.begin() + i);
		--i;
	}
}

void Bag::Update(int ownerX, int ownerY)
{
	for (int i = 0; i < mBagItemList.size(); ++i)
	{
		int x = ownerX - 24 + 8 + 16 * (i % 2);
		int y = ownerY - 24 - 16 - 16 * (i / 2);

		mBagItemList[i]->SetLocation(x, y);
	}
}

void Bag::Render(HDC hdc)
{
	for (int i = 0; i < mBagItemList.size(); ++i)
		mBagItemList[i]->Render(hdc);
}