#include "Framework.h"

ModelAnimator::ModelAnimator(string name)
	: name(name)
{
	tag = name;

	reader = new ModelReader(name);
	reader->SetShader(L"NormalMapping.hlsl");
	//reader->SetShader(L"ModelAnimator.hlsl");
}

ModelAnimator::~ModelAnimator()
{
	delete reader;

	for (ModelClip* clip : clips)
		delete clip;
}

void ModelAnimator::Update()
{
	UpdateWorld();
}

void ModelAnimator::Render()
{
	reader->Render();
}

void ModelAnimator::ReadClip(string clipName, UINT clipNum)
{
	string path = "ModelData/Clips/" + name + "/" + clipName + to_string(clipNum) + ".clip";

	BinaryReader r(path);
	string name = r.String();
	UINT frameCount = r.UInt();
	float tickPerSecond = r.Float();

	ModelClip* clip = new ModelClip(name, frameCount, tickPerSecond);

	UINT boneCount = r.UInt();
	for (UINT i = 0; i < boneCount; i++)
	{
		KeyFrame* keyFrame = new KeyFrame();
		keyFrame->boneName = r.String();

		UINT size = r.UInt();
		if (size > 0)
		{
			keyFrame->transforms.resize(size);

			void* ptr = (void*)keyFrame->transforms.data();
			r.Byte(&ptr, sizeof(KeyTransform) * size);
		}
		clip->SetKeyFrame(keyFrame->boneName, keyFrame);
	}

	clips.push_back(clip);
}
