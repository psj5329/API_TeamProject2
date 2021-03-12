#include "pch.h"
#include "GameObject.h"

GameObject::GameObject()
	: mIsActive(true), mIsDestroy(false)
{
}

GameObject::GameObject(const string & name, float x, float y)
	: mName(name), mX(x), mY(y), mIsActive(true), mIsDestroy(false)
{
}
