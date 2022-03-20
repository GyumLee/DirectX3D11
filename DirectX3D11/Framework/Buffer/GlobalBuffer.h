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

enum class LightType
{
	DIRECTIONAL, POINT, SPOT, CAPSULE
};

class LightBuffer : public ConstBuffer
{
public:
	struct Light
	{
		Float4 color = { 1, 1, 1, 1 };

		Float3 direction = { 0, -1, 1 };
		LightType type = LightType::DIRECTIONAL;

		Float3 position = { 0, 5, 0 };
		float range = 50.0f;

		float inner = 55.0f;
		float outer = 70.0f;
		float length = 50.0f;
		int active = 1;
	};

	struct Data
	{
		Light lights[MAX_LIGHT];

		int lightCount = 1;
		Float3 ambientColor = { 0.0f, 0.0f, 0.0f };

		Float3 ambientCeil = { 0.05f, 0.05f, 0.05f };
		float padding;
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

class FrameInstancingBuffer : public ConstBuffer
{
public:
	struct Frame
	{
		int clip = 0;
		UINT curFrame = 0;
		float time = 0.0f;
		float speed = 1.0f;
	};

	struct Motion
	{
		float takeTime = 0.2f;
		float tweenTime = 0.0f;
		float runningTime = 0.0f;
		float padding;

		Frame cur;
		Frame next;

		Motion()
		{
			next.clip = -1;
		}
	};

	struct Data
	{
		Motion motions[MAX_INSTANCE];
	}data;

	FrameInstancingBuffer() : ConstBuffer(&data, sizeof(Data))
	{
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

class SpriteBuffer : public ConstBuffer
{
public:
	struct Data
	{
		Float2 maxFrame = { 1, 1 };
		Float2 curFrame = { 0, 0 };
	}data;
	
	SpriteBuffer() : ConstBuffer(&data, sizeof(Data))
	{
	}
};

class ColorBuffer : public ConstBuffer
{
public:
	Float4 color = { 1, 1, 1, 1 };

	ColorBuffer() : ConstBuffer(&color, sizeof(Float4))
	{
	}
};

class WeatherBuffer : public ConstBuffer
{
public:
	struct Data
	{
		Float3 velocity = Float3(0, -1, 0);
		float distance = 100.0f;

		Float4 color = Float4(1, 1, 1, 1);

		Float3 origin = Float3(0, 0, 0);
		float time = 0.0f;

		//Float3 size = Float3(50, 50, 50);
		Float3 size = Float3(1000, 1000, 1000);
		float turbulence = 5.0f;
	}data;

	WeatherBuffer() : ConstBuffer(&data, sizeof(Data))
	{
	}
};

class WaterBuffer : public ConstBuffer
{
public:
	struct Data
	{
		Float4 color = Float4(1, 1, 1, 1);

		float waveTime = 0.0f;
		float waveSpeed = 0.1f;
		float waveScale = 0.1;
		float waveShininess = 24.0f;

		float fresnel = 0.5f;
		float padding[3];
	}data;

	WaterBuffer() : ConstBuffer(&data, sizeof(Data))
	{
	}
};