#pragma once
#include <bitset>

#define KEYMAX 256

class Input
{
	Singleton(Input)
private:
	//1: bitset : 조사
	//2. GetAsyncKeyState : 조사
	bool mKeyUpList[KEYMAX];		//어떤 키에서 손을 땠는지
	bool mKeyDownList[KEYMAX];		//어떤 키를 눌렀는지 
public:
	Input();

	bool GetKeyDown(int key);	//키가 눌렸을때 true반환
	bool GetKeyUp(int key);		//키에서 손을 땠을 때 true반환
	bool GetKey(int key);		//키를 계속 누르고 있다면 true반환
	bool GetToggleKey(int key);	//Toggle
};

