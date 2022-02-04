#include "Framework.h"

int GameMath::Random(int min, int max)
{
	return rand() % (max - min) + min;
}

float GameMath::Random(float min, float max)
{
	float normal = rand() / (float)RAND_MAX;

	return min + (max - min) * normal;
}

float GameMath::Distance(Vector3& v1, Vector3& v2)
{
	return (v1 - v2).Length();
}
