#pragma once

class Texture
{
private:
	string file;

	ScratchImage image;

	ID3D11ShaderResourceView* srv;

	static map<string, Texture*> textures;

	Texture(ID3D11ShaderResourceView* srv, ScratchImage& image);
	~Texture();

public:
	static Texture* Add(string file);
	static Texture* Add(string file, string key);
	static void Delete();

	void Set(UINT slot);

	vector<Float4> ReadPixels();

	UINT Width() { return image.GetMetadata().width; }
	UINT Height() { return image.GetMetadata().height; }

	ID3D11ShaderResourceView*& GetSRV() { return srv; }
	string GetFile() { return file; }
};