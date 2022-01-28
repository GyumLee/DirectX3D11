#include "Framework.h"

ModelAnimator::ModelAnimator(string name)
	: name(name), texture(nullptr)
{
	tag = name;

	reader = new ModelReader(name);
	reader->SetShader(L"Model/ModelAnimator.hlsl");

	frameBuffer = new FrameBuffer();
	worldBuffer = new WorldBuffer();
}

ModelAnimator::~ModelAnimator()
{
	delete reader;

	for (ModelClip* clip : clips)
		delete clip;

	delete frameBuffer;
	delete worldBuffer;

	delete[] clipTransform;
	delete[] nodeTransform;

	if (texture)
	{
		texture->Release();
		srv->Release();
	}
}

void ModelAnimator::Update()
{
	UpdateFrame();
	UpdateWorld();
}

void ModelAnimator::Render()
{
	if (texture == nullptr)
		CreateTexture();

	worldBuffer->Set(world);
	worldBuffer->SetVSBuffer(0);
	frameBuffer->SetVSBuffer(3);

	DC->VSSetShaderResources(0, 1, &srv);

	reader->Render();
}

void ModelAnimator::GUIRender()
{
	ImGui::SliderInt("Clip", &frameBuffer->data.clip, 0, clips.size() - 1);
	ImGui::SliderInt("Frame", (int*)&frameBuffer->data.curFrame, 0, clips[frameBuffer->data.clip]->GetFrameCount());
	ImGui::SliderFloat("Speed", &frameBuffer->data.speed, 0, 5.0f);
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

void ModelAnimator::UpdateFrame()
{
	FrameBuffer::Data& frameData = frameBuffer->data;

	ModelClip* clip = clips[frameData.clip];

	frameData.time += clip->GetTickPerSecond() * frameData.speed * DELTA;

	if (frameData.time >= 1.0f)
	{
		frameData.curFrame = (frameData.curFrame + 1) % clip->GetFrameCount();
		frameData.time = 0.0f;
	}
}

void ModelAnimator::CreateTexture()
{
	if (clips.empty()) return;

	UINT clipCount = clips.size();

	clipTransform = new ClipTransform[clipCount];
	nodeTransform = new ClipTransform[clipCount];

	for (UINT i = 0; i < clipCount; i++)
		CreateClipTransform(i);

	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = MAX_BONE * 4;
	desc.Height = MAX_FRAME;
	desc.MipLevels = 1;
	desc.ArraySize = clipCount;
	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	UINT pageSize = MAX_BONE * sizeof(Matrix) * MAX_FRAME;

	void* p = VirtualAlloc(nullptr, pageSize * clipCount, MEM_RESERVE, PAGE_READWRITE);

	for (UINT c = 0; c < clipCount; c++)
	{
		UINT start = c * pageSize;

		for (UINT y = 0; y < MAX_FRAME; y++)
		{
			void* temp = (BYTE*)p + MAX_BONE * y * sizeof(Matrix) + start;

			VirtualAlloc(temp, MAX_BONE * sizeof(Matrix), MEM_COMMIT, PAGE_READWRITE);
			memcpy(temp, clipTransform[c].transform[y], MAX_BONE * sizeof(Matrix));
		}
	}

	D3D11_SUBRESOURCE_DATA* subResource = new D3D11_SUBRESOURCE_DATA[clipCount];

	for (UINT c = 0; c < clipCount; c++)
	{
		void* temp = (BYTE*)p + c * pageSize;

		subResource[c].pSysMem = temp;
		subResource[c].SysMemPitch = MAX_BONE * sizeof(Matrix);
		subResource[c].SysMemSlicePitch = pageSize;
	}

	DEVICE->CreateTexture2D(&desc, subResource, &texture);

	delete[] subResource;
	VirtualFree(p, 0, MEM_RELEASE);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srvDesc.Texture2DArray.MipLevels = 1;
	srvDesc.Texture2DArray.ArraySize = clipCount;

	DEVICE->CreateShaderResourceView(texture, &srvDesc, &srv);
}

void ModelAnimator::CreateClipTransform(UINT index)
{
	ModelClip* clip = clips[index];

	for (UINT f = 0; f < clip->GetFrameCount(); f++)
	{
		UINT nodeIndex = 0;
		for (NodeData node : reader->GetNodes())
		{
			Matrix animation = XMMatrixIdentity();
			KeyFrame* frame = clip->GetKeyFrame(node.name);
			if (frame != nullptr)
			{
				KeyTransform& transform = frame->transforms[f];

				animation = XMMatrixTransformation(XMVectorZero(), XMQuaternionIdentity(),
					Vector3(transform.scale), XMVectorZero(),
					XMLoadFloat4(&transform.rotation),
					Vector3(transform.position));
			}

			Matrix parent = XMMatrixIdentity();
			int parentIndex = node.parent;

			if (parentIndex >= 0)
				parent = nodeTransform[index].transform[f][parentIndex];

			nodeTransform[index].transform[f][nodeIndex] = animation * parent;

			int boneIndex = reader->GetBone(node.name);
			if (boneIndex >= 0)
			{
				Matrix transform = reader->GetBones()[boneIndex].offset;
				transform *= nodeTransform[index].transform[f][nodeIndex];

				clipTransform[index].transform[f][boneIndex] = transform;
			}

			nodeIndex++;
		}
	}
}
