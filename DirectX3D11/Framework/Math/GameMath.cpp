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

float GameMath::Saturate(float value)
{
	return max(0.0f, min(1.0f, value));
}

Vector3 GameMath::ClosestPointOnLine(Vector3& start, Vector3& end, Vector3& point)
{
	Vector3 line = end - start;

	float x = Vector3::Dot(line, point - start);
	float y = Vector3::Dot(line, line);

	float t = Saturate(x / y);

	return start + line * t;
}
