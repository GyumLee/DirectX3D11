#pragma once

typedef VertexUVNormalTangentBlend ModelVertex;

struct MeshData
{
	string name;
	UINT materialIndex;

	vector<ModelVertex> vertices;
	vector<UINT> indices;
};

struct NodeData
{
	int index;
	string name;
	int parent;
	Matrix transform;
};

struct BoneData
{
	string name;
	int index;
	Matrix offset;
};

struct VertexWeights
{
	UINT indices[4] = {};//������ �ε��� ��
	float weights[4] = {};//���볢���� ���� ���� ��

	void Add(const UINT& index, const float& weight)
	{
		for (UINT i = 0; i < 4; i++)
		{
			if (weights[i] == 0.0f)
			{
				indices[i] = index;
				weights[i] = weight;
				return;
			}
		}
	}

	void Normalize()
	{
		float sum = 0.0f;

		for (UINT i = 0; i < 4; i++)
			sum += weights[i];

		for (UINT i = 0; i < 4; i++)
			weights[i] /= sum;
	}
};