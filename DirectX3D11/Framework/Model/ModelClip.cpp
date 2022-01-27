#include "Framework.h"

ModelClip::ModelClip(string name, UINT frameCount, float tickPerSecond)
	: name(name), frameCount(frameCount), tickPerSecond(tickPerSecond)
{
	duration = frameCount / tickPerSecond;
}

ModelClip::~ModelClip()
{
	for (pair<string, KeyFrame*> frame : keyFrames)
		delete frame.second;
}

void ModelClip::SetKeyFrame(string boneName, KeyFrame* keyFrame)
{
	if (keyFrames.count(boneName) > 0) return;

	keyFrames[boneName] = keyFrame;
}

KeyFrame* ModelClip::GetKeyFrame(string boneName)
{
	if (keyFrames.count(boneName) == 0) nullptr;

	return keyFrames[boneName];
}
