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

	bool isPlay = true;
	int rootBoneIndex = 1;

public:
	ModelAnimator(string name);
	~ModelAnimator();

	void Update();
	void Render();
	void GUIRender();

	void PlayClip(UINT clip, float speed = 1.0f, float takeTime = 0.2f);
	void StopClip() { isPlay = false; }

	void ReadClip(string clipName, UINT clipNum = 0, bool isRootMotion = false);

	Matrix GetTransformByNode(int nodeIndex);

private:
	void UpdateFrame();

	void CreateTexture();
	void CreateClipTransform(UINT index);
};