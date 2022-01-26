#pragma once

class ModelBone : public Transform
{
private:
	ModelReader* reader;

	WorldBuffer* worldBuffer;

	BoneBuffer* boneBuffer;

	map<int, Matrix> boneTransform;
	Matrix* nodeTransforms;

public:
	ModelBone(string name);
	~ModelBone();

	void Render();

	ModelReader* GetReader() { return reader; }

private:
	void MakeTransform();
};