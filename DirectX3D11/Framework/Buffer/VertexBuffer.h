#pragma once

class VertexBuffer
{
private:
	ID3D11Buffer* buffer;

	UINT stride;
	UINT offset;

public:
	VertexBuffer(void* data, UINT stride, UINT count);
	~VertexBuffer();

	void IASet(D3D_PRIMITIVE_TOPOLOGY type, UINT slot = 0);

	void Update(void* data, UINT count);
};