#include "Framework.h"

GBuffer::GBuffer()
{
	diffuseRender = new RenderTarget(WIN_WIDTH, WIN_HEIGHT, DXGI_FORMAT_R8G8B8A8_UNORM);
	specularRender = new RenderTarget(WIN_WIDTH, WIN_HEIGHT, DXGI_FORMAT_R8G8B8A8_UNORM);
	normalRender = new RenderTarget(WIN_WIDTH, WIN_HEIGHT, DXGI_FORMAT_R8G8B8A8_UNORM);

	depthStencil = new DepthStencil(WIN_WIDTH, WIN_HEIGHT, true);

	rtvs[0] = diffuseRender;
	rtvs[1] = specularRender;
	rtvs[2] = normalRender;

	srvs[0] = depthStencil->GetSRV();
	srvs[1] = diffuseRender->GetSRV();
	srvs[2] = specularRender->GetSRV();
	srvs[3] = normalRender->GetSRV();

	for (UINT i = 0; i < 4; i++)
	{
		targetTextures[i] = new RenderTexture(200, 200);
		targetTextures[i]->position = { 100 +(float)i * 200, 100, 0 };
		targetTextures[i]->SetSRV(srvs[i]);
		targetTextures[i]->UpdateWorld();
	}
}

GBuffer::~GBuffer()
{
	delete diffuseRender;
	delete specularRender;
	delete normalRender;

	delete depthStencil;

	for (RenderTexture* targetTexture :  targetTextures)
		delete targetTexture;
}

void GBuffer::PreRender()
{
	RenderTarget::SetMulti(rtvs, 3, depthStencil);
}

void GBuffer::Render()
{
	for (UINT i = 0; i < 4; i++)
	{
		DC->PSSetShaderResources(10 + i, 1, &srvs[i]);
	}
}

void GBuffer::PostRender()
{
	for (RenderTexture* targetTexture : targetTextures)
		targetTexture->Render();
}
