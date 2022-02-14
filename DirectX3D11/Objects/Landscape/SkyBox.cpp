#include "Framework.h"

SkyBox::SkyBox(string file)
{
	sphere = new Sphere();
	sphere->GetMaterial()->SetShader(L"Basic/SkyBox.hlsl");

	cubeMap = Texture::Add(file);

	rasterizerState[0] = new RasterizerState();
	rasterizerState[1] = new RasterizerState();
	rasterizerState[1]->FrontCounterClockwise(true);

	depthStencilState[0] = new DepthStencilState();
	depthStencilState[1] = new DepthStencilState();
	depthStencilState[1]->DepthEnable(false);
}

SkyBox::~SkyBox()
{
	delete sphere;

	delete rasterizerState[0];
	delete rasterizerState[1];
	delete depthStencilState[0];
	delete depthStencilState[1];
}

void SkyBox::Render()
{
	cubeMap->Set(10);

	rasterizerState[1]->SetState();
	depthStencilState[1]->SetState();

	sphere->Render();

	rasterizerState[0]->SetState();
	depthStencilState[0]->SetState();
}
