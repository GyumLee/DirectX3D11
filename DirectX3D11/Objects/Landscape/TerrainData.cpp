#include "Framework.h"

TerrainData::TerrainData(string heightFile)
{
	heightMap = Texture::Add(heightFile);

	CreateMesh();
	CreateNormal();
	AlignVertexData();
}

TerrainData::~TerrainData()
{
}

void TerrainData::CreateMesh()
{
	width = heightMap->Width();
	height = heightMap->Height();

	vector<Float4> pixels = heightMap->ReadPixels();

	//Vertices
	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{
			VertexType vertex;
			vertex.position = { (float)x, 0.0f, (float)z };
			vertex.uv.x = x / (float)(width - 1);
			vertex.uv.y = z / (float)(height - 1);

			UINT index = width * z + x;
			vertex.position.y = pixels[index].x * MAX_HEIGHT;

			prevData.push_back(vertex);
		}
	}

	//Indices
	for (UINT z = 0; z < height - 1; z++)
	{
		for (UINT x = 0; x < width - 1; x++)
		{
			indices.push_back(width * z + x);//0
			indices.push_back(width * (z + 1) + x);//1
			indices.push_back(width * (z + 1) + x + 1);//2

			indices.push_back(width * z + x);//0
			indices.push_back(width * (z + 1) + x + 1);//2
			indices.push_back(width * z + x + 1);//3
		}
	}
}

void TerrainData::CreateNormal()
{
	for (UINT i = 0; i < indices.size() / 3; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		Vector3 v0 = prevData[index0].position;
		Vector3 v1 = prevData[index1].position;
		Vector3 v2 = prevData[index2].position;

		Vector3 A = v1 - v0;
		Vector3 B = v2 - v0;

		Vector3 normal = Vector3::Cross(A, B).Normalize();

		prevData[index0].normal = normal + prevData[index0].normal;
		prevData[index1].normal = normal + prevData[index1].normal;
		prevData[index2].normal = normal + prevData[index2].normal;
	}
}

void TerrainData::AlignVertexData()
{
	vertices.resize(indices.size());

	UINT index = 0;
	for (UINT z = 0; z < height - 1; z++)
	{
		for (UINT x = 0; x < width - 1; x++)
		{
			UINT index0 = width * z + x;
			UINT index1 = width * (z + 1) + x;
			UINT index2 = width * (z + 1) + x + 1;
			UINT index3 = width * z + x + 1;

			vertices[index++] = prevData[index0];
			vertices[index++] = prevData[index1];
			vertices[index++] = prevData[index2];

			vertices[index++] = prevData[index0];
			vertices[index++] = prevData[index2];
			vertices[index++] = prevData[index3];
		}
	}
}
