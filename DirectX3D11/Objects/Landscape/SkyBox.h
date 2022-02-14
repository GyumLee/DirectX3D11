#pragma once

class SkyBox
{
private:
	Sphere* sphere;
	Texture* cubeMap;

	RasterizerState* rasterizerState[2];
	DepthStencilState* depthStencilState[2];

public:
	SkyBox(string file);
	~SkyBox();

	void Render();
};