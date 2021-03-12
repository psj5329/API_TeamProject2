#pragma once
#include <bitset>

#define KEYMAX 256

class Input
{
	Singleton(Input)
private:
	//1: bitset : ����
	//2. GetAsyncKeyState : ����
	bool mKeyUpList[KEYMAX];		//� Ű���� ���� ������
	bool mKeyDownList[KEYMAX];		//� Ű�� �������� 
public:
	Input();

	bool GetKeyDown(int key);	//Ű�� �������� true��ȯ
	bool GetKeyUp(int key);		//Ű���� ���� ���� �� true��ȯ
	bool GetKey(int key);		//Ű�� ��� ������ �ִٸ� true��ȯ
	bool GetToggleKey(int key);	//Toggle
};

