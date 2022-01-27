#pragma once

class ModelClip
{
private:
	string name;

	UINT frameCount;
	float tickPerSecond;
	float duration;

	unordered_map<string, KeyFrame*> keyFrames;

public:
	ModelClip(string name, UINT frameCount, float tickPerSecond);
	~ModelClip();

	void SetKeyFrame(string boneName, KeyFrame* keyFrame);

	KeyFrame* GetKeyFrame(string boneName);
};