#pragma once

class ModelAnimator : public Transform
{
protected:
	string name;

	ModelReader* reader;

	vector<ModelClip*> clips;

public:
	ModelAnimator(string name);
	~ModelAnimator();

	void Update();
	void Render();

	void ReadClip(string clipName, UINT clipNum = 0);
};