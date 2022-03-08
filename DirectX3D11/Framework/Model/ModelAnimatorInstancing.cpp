#include "Framework.h"

ModelAnimatorInstancing::ModelAnimatorInstancing(string name)
	: ModelAnimator(name)
{
	reader->SetShader(L"Model/ModelAnimatorInstancing.hlsl");

	instanceBuffer = new VertexBuffer(instanceDatas, sizeof(InstanceData), MAX_INSTANCE);
	frameInstancingBuffer = new FrameInstancingBuffer();
}

ModelAnimatorInstancing::~ModelAnimatorInstancing()
{
	delete instanceBuffer;
	delete frameInstancingBuffer;
}

void ModelAnimatorInstancing::Update()
{
	UpdateFrames();
	UpdateTransforms();
}

void ModelAnimatorInstancing::Render()
{
	if (texture == nullptr)
		CreateTexture();

	frameInstancingBuffer->SetVSBuffer(4);
	DC->VSSetShaderResources(0, 1, &srv);

	instanceBuffer->IASet(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 1);

	reader->Render(drawCount);
}

void ModelAnimatorInstancing::GUIRender()
{
}

Transform* ModelAnimatorInstancing::Add()
{
	Transform* transform = new Transform();
	transform->tag = reader->GetName() + "_" + to_string(transforms.size());
	transforms.push_back(transform);

	for (ModelClip* clip : clips)
	{
		clip->ResizeInstancing(transforms.size());
	}

	return transform;
}

void ModelAnimatorInstancing::PlayClip(UINT instanceID, UINT clip, float speed, float takeTime)
{
	isPlay = true;

	frameInstancingBuffer->data.motions[instanceID].next.clip = clip;
	frameInstancingBuffer->data.motions[instanceID].next.speed = speed;
	frameInstancingBuffer->data.motions[instanceID].next.curFrame = 0;
	frameInstancingBuffer->data.motions[instanceID].next.time = 0.0f;

	frameInstancingBuffer->data.motions[instanceID].takeTime = takeTime;
	frameInstancingBuffer->data.motions[instanceID].tweenTime = 0.0f;
	frameInstancingBuffer->data.motions[instanceID].runningTime = 0.0f;

	clips[clip]->InstancingInit(instanceID);
}

void ModelAnimatorInstancing::AddEvent(UINT instanceID, UINT clip, float time, Event event)
{
	clips[clip]->SetInstancingEvent(instanceID, time, event);
}

void ModelAnimatorInstancing::UpdateFrames()
{
	for (UINT i = 0; i < transforms.size(); i++)
	{
		UpdateFrame(frameInstancingBuffer->data.motions[i], i);
	}
}

void ModelAnimatorInstancing::UpdateFrame(FrameInstancingBuffer::Motion& motion, UINT instanceID)
{
	{//CurAnim
		ModelClip* clip = clips[motion.cur.clip];

		motion.cur.time += clip->GetTickPerSecond() * motion.cur.speed * DELTA;
		motion.runningTime += motion.cur.speed * DELTA;

		if (motion.cur.time >= 1.0f)
		{
			motion.cur.curFrame = (motion.cur.curFrame + 1) % (clip->GetFrameCount() - 1);
			motion.cur.time = 0.0f;
		}

		clip->InstancingExcute(instanceID, motion.runningTime);
	}

	{//NextAnim
		if (motion.next.clip < 0) return;

		ModelClip* clip = clips[motion.next.clip];

		motion.tweenTime += DELTA / motion.takeTime;

		if (motion.tweenTime < 1.0f)
		{
			motion.next.time += clip->GetTickPerSecond() * motion.next.speed * DELTA;

			if (motion.next.time >= 1.0f)
			{
				motion.next.curFrame = (motion.next.curFrame + 1) % (clip->GetFrameCount() - 1);
				motion.next.time = 0.0f;
			}
		}
		else
		{
			motion.cur = motion.next;
			motion.tweenTime = 0.0f;

			motion.next.clip = -1;
			motion.next.curFrame = 0;
			motion.next.time = 0.0f;
		}
	}
}

void ModelAnimatorInstancing::UpdateTransforms()
{
	drawCount = 0;

	for (UINT i = 0; i < transforms.size(); i++)
	{
		if (!transforms[i]->isActive) continue;

		transforms[i]->UpdateWorld();

		instanceDatas[drawCount].transform = XMMatrixTranspose(transforms[i]->GetWorld());
		instanceDatas[drawCount].index = i;
		drawCount++;
	}

	instanceBuffer->Update(instanceDatas, drawCount);
}

Matrix ModelAnimatorInstancing::GetTransformByNode(UINT instanceID, int nodeIndex)
{
	if (texture == nullptr) return XMMatrixIdentity();

	FrameInstancingBuffer::Frame& curFrame =
		frameInstancingBuffer->data.motions[instanceID].cur;

	Matrix cur = nodeTransform[curFrame.clip].transform[curFrame.curFrame][nodeIndex];
	Matrix next = nodeTransform[curFrame.clip].transform[curFrame.curFrame + 1][nodeIndex];

	Matrix curAnim = LERP(cur, next, curFrame.time);

	FrameInstancingBuffer::Frame& nextFrame =
		frameInstancingBuffer->data.motions[instanceID].next;

	if (nextFrame.clip == -1) return curAnim;

	cur = nodeTransform[nextFrame.clip].transform[nextFrame.curFrame][nodeIndex];
	next = nodeTransform[nextFrame.clip].transform[nextFrame.curFrame + 1][nodeIndex];

	Matrix nextAnim = LERP(cur, next, nextFrame.time);

	return LERP(curAnim, nextAnim, frameInstancingBuffer->data.motions[instanceID].tweenTime);
}
