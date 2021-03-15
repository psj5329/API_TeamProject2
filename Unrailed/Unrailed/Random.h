#pragma once

class Random
{
	Singleton(Random)

public:
	Random();

	inline int RandomInt(int num) { return rand() % num; }	
	inline int RandomInt(int startNum, int endNum) { return rand() % (endNum - startNum + 1) + startNum; }
};

#define RANDOM Random::GetInstance()