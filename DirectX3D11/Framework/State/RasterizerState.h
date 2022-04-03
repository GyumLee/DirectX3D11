#pragma once

class RasterizerState
{
private:
	D3D11_RASTERIZER_DESC desc;
	ID3D11RasterizerState* state;

public:
	RasterizerState();
	~RasterizerState();

	void SetState();

	void CullMode(D3D11_CULL_MODE value);
	void FillMode(D3D11_FILL_MODE value);
	void FrontCounterClockwise(bool value);

	void Changed();
};