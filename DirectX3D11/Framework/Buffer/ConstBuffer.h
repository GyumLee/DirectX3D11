#pragma once

class ConstBuffer
{
private:
	ID3D11Buffer* buffer;

	void* data;
	UINT dataSize;

	D3D11_MAPPED_SUBRESOURCE subResource;

protected:
	ConstBuffer(void* data, UINT dataSize);
	~ConstBuffer();

	void Update();

public:
	void SetVSBuffer(UINT slot);
	void SetPSBuffer(UINT slot);
	void SetCSBuffer(UINT slot);
	void SetGSBuffer(UINT slot);
};