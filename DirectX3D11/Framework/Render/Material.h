#pragma once

class Material
{
private:
	string name;
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

	void Save(string file);
	void Load(string file);

private:
	void SelectMap(int type);

	void SaveDialog();
	void LoadDialog();
};