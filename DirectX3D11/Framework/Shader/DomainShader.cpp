#include "Framework.h"

DomainShader::DomainShader(wstring file)
{
	wstring path = L"Shaders/" + file;

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;

	V(D3DCompileFromFile(path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"DS", "ds_5_0", flags, 0, &blob, nullptr));

	DEVICE->CreateDomainShader(blob->GetBufferPointer(), blob->GetBufferSize(),
		nullptr, &shader);
}

DomainShader::~DomainShader()
{
	shader->Release();
}

void DomainShader::Set()
{
	DC->DSSetShader(shader, nullptr, 0);
}
