#pragma once

class ParticleSytem
{
private:
	const UINT MAX_COUNT = 1000;

	struct ParticleData
	{
		bool isLoop = true;
		bool isAdditive = true;
		bool isBillboard = true;
		float duration = 1.0f;
		UINT count = 500;
		Vector3 minVelocity = { -1, -1, -1 };
		Vector3 maxVelocity = { +1, +1, +1 };
		Vector3 minAccelation;
		Vector3 maxAccelation;
		Vector3 minScale = { 1, 1, 1 };
		Vector3 maxScale = { 1, 1, 1 };
		float minStartTime = 0.0f;
		float maxStartTime = 0.0f;
		float minSpeed = 1.0f;
		float maxSpeed = 1.0f;
		float minAngularVelocity = 0.0f;
		float maxAngularVelocity = 0.0f;
		float worldRotationY;
		Float4 startColor = { 1, 1, 1, 1 };
		Float4 endColor = { 1, 1, 1, 1 };
	}particleData;
};