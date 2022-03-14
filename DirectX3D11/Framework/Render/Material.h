#pragma once

class Material
{
public:
	string name;

private:
	string editName;

	string file;

	VertexShader* vertexShader;
	PixelShader* pixelShader;

	Texture* diffuseMap;
	Texture* specularMap;
	Texture* normalMap;

	MaterialBuffer* buffer;

public:
	Material(string name = "default");
	~Material();

	void Set();

	void GUIRender();

	void SetShader(wstring file);

	void SetDiffuseMap(string file);
	void SetSpecularMap(string file);
	void SetNormalMap(string file);

	string GetDiffuseMap() { return diffuseMap->GetFile(); }

	void Save(string file);
	void Load(string file);

	MaterialBuffer::Data& GetData() { return buffer->data; }

private:
	void SelectMap(int type);

	void SaveDialog();
	void LoadDialog();
};