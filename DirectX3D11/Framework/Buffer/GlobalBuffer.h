#pragma once

class WorldBuffer : public ConstBuffer
{
private:
	struct Data
	{
		Matrix matrix;
	}data;

public:
	WorldBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		data.matrix = XMMatrixIdentity();
	}

	void Set(Matrix value)
	{
		data.matrix = XMMatrixTranspose(value);
	}
};

class ViewBuffer : public ConstBuffer
{
private:
	struct Data
	{
		Matrix view;
		Matrix invView;
	}data;

public:
	ViewBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		data.view = XMMatrixIdentity();
		data.invView = XMMatrixIdentity();
	}

	void Set(Matrix view, Matrix invView)
	{
		data.view = XMMatrixTranspose(view);
		data.invView = XMMatrixTranspose(invView);
	}
};

class ProjectionBuffer : public ConstBuffer
{
private:
	struct Data
	{
		Matrix matrix;
	}data;

public:
	ProjectionBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		data.matrix = XMMatrixIdentity();
	}

	void Set(Matrix value)
	{
		data.matrix = XMMatrixTranspose(value);
	}
};

class LightBuffer : public ConstBuffer
{
public:
	struct Data
	{
		Float4 color = { 1, 1, 1, 1 };
		// it requires 16byte
		Float3 direction = { 0, -1, 1 }; // 12byte
		float padding; // 4byte

		float padding2;
		Float3 ambientColor = { 0.1f, 0.1f, 0.1f };

		Float3 ambientCeil = { 0.1f, 0.1f, 0.1f };
		float padding3;
	}data;

	LightBuffer() : ConstBuffer(&data, sizeof(Data))
	{
	}
};

class MaterialBuffer : public ConstBuffer
{
public:
	struct Data
	{
		Float4 diffuse = { 1, 1, 1, 1 };
		Float4 specular = { 1, 1, 1, 1 };
		Float4 ambient = { 1, 1, 1, 1 };
		Float4 emissive = { 0, 0, 0, 0 };

		float shininess = 24.0f;

		int hasDiffuseMap = 0;
		int hasSpecularMap = 0;
		int hasNormalMap = 0;
	}data;

	MaterialBuffer() : ConstBuffer(&data, sizeof(Data))
	{
	}
};

class BrushBuffer : public ConstBuffer
{
public:
	struct Data
	{
		int type = 0;
		Float3 pickingPos = { 0, 0, 0 };

		Float3 color = { 0, 0.5f, 0 };
		float range = 10.0f;
	}data;

	BrushBuffer() : ConstBuffer(&data, sizeof(Data))
	{
	}
};

class RayBuffer : public ConstBuffer
{
public:
	struct Data
	{
		Float3 position = { 0, 0, 0 };
		float size = 0.0f;

		Float3 direction = { 0, 0, 1 };
		float padding;
	}data;

	RayBuffer() : ConstBuffer(&data, sizeof(Data))
	{
	}
};

class BoneBuffer : public ConstBuffer
{
public:
	struct Data
	{
		Matrix bones[MAX_BONE];
	}data;

	BoneBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		for (UINT i = 0; i < MAX_BONE; i++)
			data.bones[i] = XMMatrixIdentity();
	}
};

class BoneIndexBuffer : public ConstBuffer
{
public:
	struct Data
	{
		int index = 0;

		float padding[3];
	}data;

	BoneIndexBuffer() : ConstBuffer(&data, sizeof(Data))
	{
	}
};

class FrameBuffer : public ConstBuffer
{
public:
	struct Frame
	{
		int clip = 0;
		UINT curFrame = 0;
		float time = 0.0f;
		float speed = 1.0f;
	};

	struct Data
	{
		float takeTime = 0.2f;
		float tweenTime = 0.0f;
		float runningTime = 0.0f;
		float padding;

		Frame cur;
		Frame next;
	}data;

	FrameBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		data.next.clip = -1;
	}
};

class ValueBuffer : public ConstBuffer
{
public:
	float values[4];

	ValueBuffer() : ConstBuffer(&values, sizeof(float) * 4)
	{
	}
};