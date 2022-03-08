#pragma once

class ModelAnimatorInstancing : public ModelAnimator
{
private:
	vector<Transform*> transforms;
	InstanceData instanceDatas[MAX_INSTANCE];

	VertexBuffer* instanceBuffer;
	FrameInstancingBuffer* frameInstancingBuffer;

	UINT drawCount;

public:
	ModelAnimatorInstancing(string name);
	~ModelAnimatorInstancing();

	void Update();
	void Render();
	void GUIRender();

	Transform* Add();

	void PlayClip(UINT instanceID, UINT clip, float speed = 1.0f, float takeTime = 0.2f);

	void AddEvent(UINT instanceID, UINT clip, float time, Event event);

	void UpdateFrames();
	void UpdateFrame(FrameInstancingBuffer::Motion& motion, UINT instanceID);
	void UpdateTransforms();

	Matrix GetTransformByNode(UINT instanceID, int nodeIndex);
};