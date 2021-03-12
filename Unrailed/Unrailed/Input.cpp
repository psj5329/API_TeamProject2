#include "pch.h"
#include "Input.h"

Input::Input()
{
	//ZeroMemory : 인자로 들어온 범위 내의 모든 메모리에 접근해서 0으로 초기화!!!
	//첫번째 인자 : 초기화할 메모리 첫 주소
	//두번째 인자 : 초기화할 메모리 크기(첫 주소로부터)

	ZeroMemory(mKeyDownList, sizeof(bool) * KEYMAX);
	ZeroMemory(mKeyUpList, sizeof(bool) * KEYMAX);

	//for (int i = 0; i < KEYMAX; ++i)
	//{
	//	mKeyDownList[i] = false;
	//	mKeyUpList[i] = false;
	//}
}

bool Input::GetKeyDown(int key)
{
	//GetAsyncKeyState : 현재 키가 눌렸는지 안눌렸는지 등 키에 대한 상태를 반환해주는 함수
	//해당 키가 눌려있다면
	if (GetAsyncKeyState(key) & 0x8000)
	{
		//해당키가 눌리지 않았다면
		if (mKeyDownList[key] == false)
		{
			mKeyDownList[key] = true;
			return true;
		}
	}
	//해당 키가 눌려있지 않다면
	else
	{
		//키눌림 상태는 false
		mKeyDownList[key] = false;
	}

	return false;
}

bool Input::GetKeyUp(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		mKeyUpList[key] = true;
	}
	else
	{
		if (mKeyUpList[key] == true)
		{
			mKeyUpList[key] = false;
			return true;
		}
	}

	return false;
}

bool Input::GetKey(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
		return true;
	return false;
}

bool Input::GetToggleKey(int key)
{
	if (GetAsyncKeyState(key) & 0x0001)
		return true;

	return false;
}
