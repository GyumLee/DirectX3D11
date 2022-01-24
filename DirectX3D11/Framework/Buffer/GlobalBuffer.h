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

		Float4 ambientColor = { 0.1f, 0.1f, 0.1f, 1.0f };
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