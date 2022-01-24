#include "Framework.h"

PixelShader::PixelShader(wstring file)
{
	wstring path = L"Shaders/" + file;

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;

	V(D3DCompileFromFile(path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"PS", "ps_5_0", flags, 0, &blob, nullptr));

	DEVICE->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(),
		nullptr, &shader);
}

PixelShader::~PixelShader()
{
	shader->Release();
}

void PixelShader::Set()
{
	DC->PSSetShader(shader, nullptr, 0);
}
