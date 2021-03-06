#pragma once

class RawBuffer
{
private:
	ID3D11Resource* output;
	ID3D11UnorderedAccessView* uav;

	ID3D11Resource* result;

	UINT bytewidth;

public:
	RawBuffer(UINT bytewidth);
	~RawBuffer();

	void Copy(void* data, UINT size);

	ID3D11UnorderedAccessView*& GetUAV() { return uav; }

private:
	void CreateOutput();
	void CreateUAV();
	void CreateResult();
};