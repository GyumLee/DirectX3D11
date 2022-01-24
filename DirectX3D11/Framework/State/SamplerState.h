#pragma once

class SamplerState
{
private:
	D3D11_SAMPLER_DESC desc;
	ID3D11SamplerState* state;

public:
	SamplerState();
	~SamplerState();

	void SetState(UINT slot = 0);

	void Changed();
};