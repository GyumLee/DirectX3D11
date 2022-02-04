#pragma once

class Ninja : public ModelAnimator
{
private:
	Model* kunai;

	Matrix rightHand;

public:
	Ninja();
	~Ninja();

	void Update();
	void Render();
	void GUIRender();

private:
	void SetRightHand();
};