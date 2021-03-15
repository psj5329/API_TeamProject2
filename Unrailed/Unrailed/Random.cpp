#include "pch.h"
#include "Random.h"
#include <ctime>

Singleton_NULL(Random)

Random::Random()
{
	srand((unsigned int)time(NULL));
}

