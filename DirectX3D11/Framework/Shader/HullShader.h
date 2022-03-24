#pragma once

class HullShader : public Shader
{
private:
	friend class Shader;

	ID3D11HullShader* shader;

	HullShader(wstring file);
	~HullShader();

public:
	virtual void Set() override;
};