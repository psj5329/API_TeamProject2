#include "pch.h"
#include "Input.h"

Singleton_NULL(Input)

Input::Input()
{
	mPrevKey.reset();
	mCurrentKey.reset();
}

void Input::Update()
{
	mPrevKey = mCurrentKey;
}

bool Input::GetKeyDown(const int& key)
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		if (!mPrevKey[key])
		{
			mCurrentKey.set(key, true);
			return true;
		}
	}
	else
		mCurrentKey.set(key, false);

	return false;
}

bool Input::GetKeyUp(const int& key)
{
	if (GetAsyncKeyState(key) & 0x8000)
		mCurrentKey.set(key, true);
	else
	{
		if (mPrevKey[key])
		{
			mCurrentKey.set(key, false);
			return true;
		}
	}

	return false;
}

bool Input::GetKey(const int& key)
{
	if (GetAsyncKeyState(key) & 0x8000)
		return true;

	return false;
}

bool Input::GetToggleKey(const int& key)
{
	if (GetKeyState(key) & 0x0001)
		return true;

	return false;
}

bool Input::GetKeyAKeyDownB(const int& keyA, const int& keyB)
{
	if (GetAsyncKeyState(keyA) & 0x8000)
	{
		if (GetAsyncKeyState(keyB) & 0x8000)
		{
			if (!mPrevKey[keyB])
			{
				mCurrentKey.set(keyB, true);
				return true;
			}
		}
		else
			mCurrentKey.set(keyB, false);

		//return false;
	}

	return false;
}

bool Input::GetKeyAKeyUpB(const int& keyA, const int& keyB)
{
	if (GetAsyncKeyState(keyA) & 0x8000)
	{
		if (GetAsyncKeyState(keyB) & 0x8000)
			mCurrentKey.set(keyB, true);
		else
		{
			if (mPrevKey[keyB])
			{
				mCurrentKey.set(keyB, false);
				return true;
			}
		}
		//return false;
	}

	return false;
}

bool Input::GetKeyAKeyB(const int& keyA, const int& keyB)
{
	if (GetAsyncKeyState(keyA) & 0x8000)
	{
		if (GetAsyncKeyState(keyB) & 0x8000)
			return true;

		//return false;
	}

	return false;
}