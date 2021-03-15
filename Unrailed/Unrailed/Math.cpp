#include "pch.h"
#include "Math.h"

float Math::GetDistance(float aX, float aY, float bX, float bY)
{
	float width = aX - bX;
	float height = aY - bY;

	float distance = sqrtf(width * width + height * height);

	return distance;
}

float Math::GetAngle(float aX, float aY, float bX, float bY)
{
	float width = bX - aX;
	float height = bY - aY;

	float distance = sqrtf(width * width + height * height);

	float angle = acos(width / distance);

	if (bY > aY)
	{
		angle = PI2 - angle;
	}

	return angle;
}

float Math::Lerp(float start, float end, float ratio)
{
	return start + (end - start) * ratio;
}

float Math::DiagonalLength(float a, float b)
{
	return sqrtf(pow(a, 2) + pow(b, 2));
}
