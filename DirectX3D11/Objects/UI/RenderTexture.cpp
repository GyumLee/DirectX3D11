#include "Framework.h"

RenderTexture::RenderTexture(float width, float height)
	: Quad(width, height), srv(nullptr)
{
}

RenderTexture::~RenderTexture()
{
}

void RenderTexture::Render()
{
	worldBuffer->Set(world);
	worldBuffer->SetVSBuffer(0);

	mesh->IASet();
	material->Set();

	if (srv)
		DC->PSSetShaderResources(0, 1, &srv);

	DC->DrawIndexed(indices.size(), 0, 0);
}
