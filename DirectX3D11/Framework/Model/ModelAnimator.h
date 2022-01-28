#pragma once

class ModelAnimator : public Transform
{
protected:
	string name;

	ModelReader* reader;

	vector<ModelClip*> clips;

	FrameBuffer* frameBuffer;
	WorldBuffer* worldBuffer;

	ClipTransform* clipTransform;
	ClipTransform* nodeTransform;

	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* srv;

public:
	ModelAnimator(string name);
	~ModelAnimator();

	void Update();
	void Render();
	void GUIRender();

	void ReadClip(string clipName, UINT clipNum = 0);

private:
	void UpdateFrame();

	void CreateTexture();
	void CreateClipTransform(UINT index);
};