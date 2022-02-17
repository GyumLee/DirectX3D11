#pragma once

namespace GameMath
{
	int Random(int min, int max);
	float Random(float min, float max);

	float Distance(Vector3& v1, Vector3& v2);
	float Saturate(float value);

	Vector3 ClosestPointOnLine(Vector3& start, Vector3& end, Vector3& point);
}