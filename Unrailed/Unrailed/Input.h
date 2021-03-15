#pragma once
#include <bitset>

#define KEYMAX 256

class Input
{
	Singleton(Input)

private:
	bitset<KEYMAX> mPrevKey;
	bitset<KEYMAX> mCurrentKey;

public:
	Input();

	void Update();

	bool GetKeyDown(const int& key);
	bool GetKeyUp(const int& key);
	bool GetKey(const int& key);
	bool GetToggleKey(const int& key);

	bool GetKeyAKeyDownB(const int& keyA, const int& keyB);
	bool GetKeyAKeyUpB(const int& keyA, const int& keyB);
	bool GetKeyAKeyB(const int& keyA, const int& keyB);
};

#define INPUT Input::GetInstance()