#include "pch.h"
#include "Input.h"

Input::Input()
{
	//ZeroMemory : ���ڷ� ���� ���� ���� ��� �޸𸮿� �����ؼ� 0���� �ʱ�ȭ!!!
	//ù��° ���� : �ʱ�ȭ�� �޸� ù �ּ�
	//�ι�° ���� : �ʱ�ȭ�� �޸� ũ��(ù �ּҷκ���)

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
	//GetAsyncKeyState : ���� Ű�� ���ȴ��� �ȴ��ȴ��� �� Ű�� ���� ���¸� ��ȯ���ִ� �Լ�
	//�ش� Ű�� �����ִٸ�
	if (GetAsyncKeyState(key) & 0x8000)
	{
		//�ش�Ű�� ������ �ʾҴٸ�
		if (mKeyDownList[key] == false)
		{
			mKeyDownList[key] = true;
			return true;
		}
	}
	//�ش� Ű�� �������� �ʴٸ�
	else
	{
		//Ű���� ���´� false
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
