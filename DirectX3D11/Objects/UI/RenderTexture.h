#pragma once

class RenderTexture : public Quad
{
private:
	ID3D11ShaderResourceView* srv;

public:
	RenderTexture(float width = 1.0f, float height = 1.0f);
	~RenderTexture();

	void Render();

	void SetSRV(ID3D11ShaderResourceView* srv) { this->srv = srv; }
};