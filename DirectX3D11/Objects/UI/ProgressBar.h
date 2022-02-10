#pragma once

class ProgressBar : public Transform
{
private:
	ValueBuffer* valueBuffer;

	Quad* frontBar;
	Quad* backBar;

public:
	ProgressBar(string frontImage, string backImage, float maxValue = 100.0f);
	~ProgressBar();

	void Update();
	void Render();

	void SetValue(float value) { valueBuffer->values[1] = value; }
	void SetLerpValue(float value) { valueBuffer->values[2] = value; }
};