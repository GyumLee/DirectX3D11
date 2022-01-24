#include "Framework.h"

Cube::Cube(Float3 size) : size(size)
{
	tag = "Cube";

	material = new Material();
	material->SetShader(L"NormalMapping.hlsl");

	//CreateMesh();
	CreateSmoothMesh();

	worldBuffer = new WorldBuffer();
}

Cube::~Cube()
{
	delete material;
	delete mesh;

	delete worldBuffer;
}

void Cube::Render()
{
	worldBuffer->Set(world);
	worldBuffer->SetVSBuffer(0);

	mesh->IASet();
	material->Set();

	DC->DrawIndexed(indices.size(), 0, 0);
}

void Cube::CreateMesh()
{
	Float3 halfSize;
	halfSize.x = size.x * 0.5f;
	halfSize.y = size.y * 0.5f;
	halfSize.z = size.z * 0.5f;

	vertices.resize(24);

	//front
	vertices[0].position = { -halfSize.x, -halfSize.y, -halfSize.z };
	vertices[1].position = { -halfSize.x, +halfSize.y, -halfSize.z };
	vertices[2].position = { +halfSize.x, -halfSize.y, -halfSize.z };
	vertices[3].position = { +halfSize.x, +halfSize.y, -halfSize.z };
	//back
	vertices[4].position = { -halfSize.x, -halfSize.y, +halfSize.z };
	vertices[5].position = { -halfSize.x, +halfSize.y, +halfSize.z };
	vertices[6].position = { +halfSize.x, -halfSize.y, +halfSize.z };
	vertices[7].position = { +halfSize.x, +halfSize.y, +halfSize.z };
	//up
	vertices[8].position = { -halfSize.x, +halfSize.y, -halfSize.z };
	vertices[9].position = { +halfSize.x, +halfSize.y, -halfSize.z };
	vertices[10].position = { -halfSize.x, +halfSize.y, +halfSize.z };
	vertices[11].position = { +halfSize.x, +halfSize.y, +halfSize.z };
	//down
	vertices[12].position = { -halfSize.x, -halfSize.y, -halfSize.z };
	vertices[13].position = { +halfSize.x, -halfSize.y, -halfSize.z };
	vertices[14].position = { -halfSize.x, -halfSize.y, +halfSize.z };
	vertices[15].position = { +halfSize.x, -halfSize.y, +halfSize.z };
	//right
	vertices[16].position = { +halfSize.x, -halfSize.y, -halfSize.z };
	vertices[17].position = { +halfSize.x, +halfSize.y, -halfSize.z };
	vertices[18].position = { +halfSize.x, -halfSize.y, +halfSize.z };
	vertices[19].position = { +halfSize.x, +halfSize.y, +halfSize.z };
	//left
	vertices[20].position = { -halfSize.x, -halfSize.y, -halfSize.z };
	vertices[21].position = { -halfSize.x, +halfSize.y, -halfSize.z };
	vertices[22].position = { -halfSize.x, -halfSize.y, +halfSize.z };
	vertices[23].position = { -halfSize.x, +halfSize.y, +halfSize.z };

	vertices[0].uv = { 0, 1 };
	vertices[1].uv = { 0, 0 };
	vertices[2].uv = { 1, 1 };
	vertices[3].uv = { 1, 0 };

	vertices[4].uv = { 0, 1 };
	vertices[5].uv = { 0, 0 };
	vertices[6].uv = { 1, 1 };
	vertices[7].uv = { 1, 0 };

	vertices[8].uv = { 0, 1 };
	vertices[9].uv = { 0, 0 };
	vertices[10].uv = { 1, 1 };
	vertices[11].uv = { 1, 0 };

	vertices[12].uv = { 0, 1 };
	vertices[13].uv = { 0, 0 };
	vertices[14].uv = { 1, 1 };
	vertices[15].uv = { 1, 0 };

	vertices[16].uv = { 0, 1 };
	vertices[17].uv = { 0, 0 };
	vertices[18].uv = { 1, 1 };
	vertices[19].uv = { 1, 0 };

	vertices[20].uv = { 0, 1 };
	vertices[21].uv = { 0, 0 };
	vertices[22].uv = { 1, 1 };
	vertices[23].uv = { 1, 0 };

	indices = {
		//front
		0, 1, 2, 2, 1, 3,
		//back
		4, 6, 5, 5, 6, 7,
		//up
		8, 10, 9, 9, 10, 11,
		//down
		12, 13, 14, 14, 13, 15,
		//right
		16, 17, 18, 18, 17, 19,
		//left
		20, 22, 21, 21, 22, 23
	};

	CreateNormal();

	mesh = new Mesh(vertices.data(), sizeof(VertexType), vertices.size(),
		indices.data(), indices.size());
}

void Cube::CreateSmoothMesh()
{
	Float3 halfSize;
	halfSize.x = size.x * 0.5f;
	halfSize.y = size.y * 0.5f;
	halfSize.z = size.z * 0.5f;

	vertices.resize(8);

	vertices[0].position = { -halfSize.x, -halfSize.y, -halfSize.z };
	vertices[1].position = { -halfSize.x, +halfSize.y, -halfSize.z };
	vertices[2].position = { +halfSize.x, -halfSize.y, -halfSize.z };
	vertices[3].position = { +halfSize.x, +halfSize.y, -halfSize.z };

	vertices[4].position = { -halfSize.x, -halfSize.y, +halfSize.z };
	vertices[5].position = { -halfSize.x, +halfSize.y, +halfSize.z };
	vertices[6].position = { +halfSize.x, -halfSize.y, +halfSize.z };
	vertices[7].position = { +halfSize.x, +halfSize.y, +halfSize.z };

	vertices[0].uv = { 0, 1 };
	vertices[1].uv = { 0, 0 };
	vertices[2].uv = { 1, 1 };
	vertices[3].uv = { 1, 0 };

	vertices[4].uv = { 0, 1 };
	vertices[5].uv = { 0, 0 };
	vertices[6].uv = { 1, 1 };
	vertices[7].uv = { 1, 0 };

	indices = {
		//front
		0, 1, 2, 2, 1, 3,
		//right
		2, 3, 6, 6, 3, 7,
		//up
		1, 5, 3, 3, 5, 7,
		//back
		4, 6, 5, 5, 6, 7,
		//left
		0, 4, 1, 1, 4, 5,
		//down
		0, 2, 4, 4, 2, 6
	};


	CreateNormal();

	mesh = new Mesh(vertices.data(), sizeof(VertexType), vertices.size(),
		indices.data(), indices.size());
}

void Cube::CreateNormal()
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
