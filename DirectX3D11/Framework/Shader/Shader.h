#pragma once

class VertexShader;
class PixelShader;
class ComputeShader;
class GeometryShader;
class HullShader;
class DomainShader;

class Shader
{
private:
	static unordered_map<wstring, Shader*> shaders;

protected:
	ID3DBlob* blob;

	virtual ~Shader();

public:
	static VertexShader* AddVS(wstring file);
	static PixelShader* AddPS(wstring file);
	static ComputeShader* AddCS(wstring file);
	static GeometryShader* AddGS(wstring file);
	static HullShader* AddHS(wstring file);
	static DomainShader* AddDS(wstring file);

	static void Delete();

	virtual void Set() = 0;
};