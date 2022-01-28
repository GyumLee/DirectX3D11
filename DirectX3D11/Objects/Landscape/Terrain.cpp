#include "Framework.h"

Terrain::Terrain()
	: width(10), height(10)
{
	material = new Material("Terrain");
	material->SetShader(L"Terrain/TerrainSplatting.hlsl");

	heightMap = Texture::Add("Textures/HeightMaps/TestHeightMap.png");
	alphaMap = Texture::Add("Textures/HeightMaps/TestAlphaMap2.png");
	secondMap = Texture::Add("Textures/Landscape/Dirt2.png");
	thirdMap = Texture::Add("Textures/Landscape/Stones.png");
	//Texture::Add Textures before CreateMesh
	CreateMesh();

	worldBuffer = new WorldBuffer();

	rasterizerState[0] = new RasterizerState();
	rasterizerState[1] = new RasterizerState();
	rasterizerState[1]->FillMode(D3D11_FILL_WIREFRAME);
}

Terrain::~Terrain()
{
	delete material;
	delete mesh;

	delete worldBuffer;

	delete rasterizerState[0];
	delete rasterizerState[1];
}

void Terrain::Render()
{
	alphaMap->Set(10);
	secondMap->Set(11);
	thirdMap->Set(12);

	worldBuffer->SetVSBuffer(0);

	mesh->IASet();
	material->Set();

	//rasterizerState[1]->SetState();

	DC->DrawIndexed(indices.size(), 0, 0);

	rasterizerState[0]->SetState();
}

float Terrain::GetHeight(Vector3 pos)
{
	int x = (int)pos.x;
	int z = (int)pos.z;

	if(x < 0 || x >= width - 1) return 0.0f;
	if(z < 0 || z >= height - 1) return 0.0f;

	UINT index[4];
	index[0] = width * z + x;
	index[1] = width * (z + 1) + x;
	index[2] = width * z + x + 1;
	index[3] = width * (z + 1) + x + 1;

	Vector3 p[4];
	for (UINT i = 0; i < 4; i++)
		p[i] = vertices[index[i]].position;

	float u = pos.x - p[0].x;
	float v = pos.z - p[0].z;

	Vector3 result;

	if (u + v <= 1.0f)
	{
		result = (p[2] - p[0]) * u + (p[1] - p[0]) * v + p[0];
	}
	else
	{
		u = 1.0f - u;
		v = 1.0f - v;

		result = (p[1] - p[3]) * u + (p[2] - p[3]) * v + p[3];
	}

	return result.y;
}

Vector3 Terrain::Picking()
{
	Ray ray = CAM->ScreenPointToRay(mousePos);

	for (UINT z = 0; z < height - 1; z++)
	{
		for (UINT x = 0; x < width - 1; x++)
		{
			UINT index[4];
			index[0] = width * z + x;
			index[1] = width * z + x + 1;
			index[2] = width * (z + 1) + x;
			index[3] = width * (z + 1) + x + 1;

			Vector3 p[4];
			for (UINT i = 0; i < 4; i++)
				p[i] = vertices[index[i]].position;

			float distance = 0.0f;
			if (TriangleTests::Intersects(ray.position, ray.direction,
				p[0], p[1], p[2], distance))
			{
				return ray.position + ray.direction * distance;
			}

			if (TriangleTests::Intersects(ray.position, ray.direction,
				p[3], p[1], p[2], distance))
			{
				return ray.position + ray.direction * distance;
			}
		}
	}

	return Vector3();
}

void Terrain::CreateMesh()
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

			vertices.push_back(vertex);
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

	CreateNormal();
	CreateTangent();
	//CreateNormal before SetMesh
	mesh = new Mesh(vertices.data(), sizeof(VertexType), vertices.size(),
		indices.data(), indices.size());
}

void Terrain::CreateNormal()
{
	for (UINT i = 0; i < indices.size() / 3; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		Vector3 v0 = vertices[index0].position;
		Vector3 v1 = vertices[index1].position;
		Vector3 v2 = vertices[index2].position;

		Vector3 A = v1 - v0;
		Vector3 B = v2 - v0;

		Vector3 normal = Vector3::Cross(A, B).Normalize();

		vertices[index0].normal = normal + vertices[index0].normal;
		vertices[index1].normal = normal + vertices[index1].normal;
		vertices[index2].normal = normal + vertices[index2].normal;
	}
}

void Terrain::CreateTangent()
{
	for (UINT i = 0; i < indices.size() / 3; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		VertexType vertex0 = vertices[index0];
		VertexType vertex1 = vertices[index1];
		VertexType vertex2 = vertices[index2];

		Vector3 p0 = vertex0.position;
		Vector3 p1 = vertex1.position;
		Vector3 p2 = vertex2.position;

		Float2 uv0 = vertex0.uv;
		Float2 uv1 = vertex1.uv;
		Float2 uv2 = vertex2.uv;

		Vector3 e0 = p1 - p0;
		Vector3 e1 = p2 - p0;

		float u0 = uv1.x - uv0.x;
		float u1 = uv2.x - uv0.x;
		float v0 = uv1.y - uv0.y;
		float v1 = uv2.y - uv0.y;

		float d = 1.0f / (u0 * v1 - v0 * u1);
		Vector3 tangent = (e0 * v1 - v0 * e1) * d;

		vertices[index0].tangent = vertices[index0].tangent + tangent;
		vertices[index1].tangent = vertices[index1].tangent + tangent;
		vertices[index2].tangent = vertices[index2].tangent + tangent;
	}

	for (VertexType& vertex : vertices)
	{
		Vector3 t = vertex.tangent;
		Vector3 n = vertex.normal;

		vertex.tangent = (t - n * Vector3::Dot(n.Normalize(), t)).Normalize();
	}
}
