#pragma once

class ModelClip
{
private:
	string name;

	UINT frameCount;
	float tickPerSecond;
	float duration;

	bool isRootMotion = false;

	unordered_map<string, KeyFrame*> keyFrames;

	map<float, Event> Events;
	map<float, Event>::iterator eventIter;

	vector<map<float, Event>> InstancingEvents;
	vector<map<float, Event>::iterator> instancingEventIter;

public:
	ModelClip(string name, UINT frameCount, float tickPerSecond);
	~ModelClip();

	void Init();
	void Excute(float playTime);

	void InstancingInit(UINT instanceID);
	void InstancingExcute(UINT instanceID, float playTime);

	void SetEvent(float timeRatio, Event event);
	void SetInstancingEvent(UINT instanceID, float timeRatio, Event event);

	void SetKeyFrame(string boneName, KeyFrame* keyFrame);

	KeyFrame* GetKeyFrame(string boneName);
	UINT GetFrameCount() { return frameCount; }
	float GetTickPerSecond() { return tickPerSecond; }

	void SetRootMotion(bool value) { isRootMotion = value; }
	bool IsRootMotion() { return isRootMotion; }

	void ResizeInstancing(UINT size);
};