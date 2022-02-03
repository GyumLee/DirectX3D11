#pragma once

class Ninja : public ModelAnimator
{
private:
	Sphere* sphere;

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