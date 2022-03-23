#include "Framework.h"

QuadTreeTerrain::QuadTreeTerrain(string heightFile)
	: drawCount(0)
{
	terrainData = new TerrainData(heightFile);
	width = terrainData->GetSize().x;
	height = terrainData->GetSize().y;

	material = new Material();
	material->SetShader(L"Lighting/ModelLighting.hlsl");
	material->SetDiffuseMap("Textures/Landscape/Dirt2.png");

	worldBuffer = new WorldBuffer();

	UINT vertexCount = terrainData->GetVertices().size();
	triangleCount = vertexCount / 3;

	vertices = new VertexType[vertexCount];
	memcpy(vertices, terrainData->GetVertices().data(), sizeof(VertexType) * vertexCount);

	float centerX = 0.0f;
	float centerZ = 0.0f;
	float size = 0.0f;

	CalcMeshDimensions(vertexCount, centerX, centerZ, size);

	root = new Node();
	CreateTreeNode(root, centerX, centerZ, size);
}

QuadTreeTerrain::~QuadTreeTerrain()
{
	DeleteNode(root);
	delete root;

	delete material;
	delete worldBuffer;

	delete[] vertices;
}

void QuadTreeTerrain::Render()
{
	drawCount = 0;
	material->Set();
	RenderNode(root);
}

void QuadTreeTerrain::GUIRender()
{
	ImGui::Text("DrawCount : %d", drawCount);
}

void QuadTreeTerrain::RenderNode(Node* node)
{
	Vector3 center(node->x, 0.0f, node->z);
	float radius = node->size * 0.5f;

	if (!FRUSTUM->ContainSphere(center, radius))
		return;

	UINT count = 0;
	for (UINT i = 0; i < 4; i++)
	{
		if (node->children[i] != nullptr)
		{
			count++;
			RenderNode(node->children[i]);
		}
	}

	if (count != 0)
		return;

	//Leaf Node
	node->mesh->IASet();

	worldBuffer->Set(world);
	worldBuffer->SetVSBuffer(0);

	UINT indexCount = node->triangleCount * 3;
	DC->DrawIndexed(indexCount, 0, 0);

	drawCount += node->triangleCount;
}

void QuadTreeTerrain::DeleteNode(Node* node)
{
	for (UINT i = 0; i < 4; i++)
	{
		if (node->children[i] != nullptr)
		{
			DeleteNode(node->children[i]);
			delete node->children[i];
		}
	}

	delete node->mesh;
}

void QuadTreeTerrain::CalcMeshDimensions(UINT vertexCount, float& centerX, float& centerZ, float& size)
{
	for (UINT i = 0; i < vertexCount; i++)
	{
		centerX += vertices[i].position.x;
		centerZ += vertices[i].position.z;
	}

	centerX /= (float)vertexCount;
	centerZ /= (float)vertexCount;

	float maxX = 0.0f;
	float maxZ = 0.0f;

	for (UINT i = 0; i < vertexCount; i++)
	{
		float width = abs(vertices[i].position.x - centerX);
		float depth = abs(vertices[i].position.z - centerZ);

		if (width > maxX) maxX = width;
		if (depth > maxZ) maxZ = depth;
	}

	size = max(maxX, maxZ) * 2.0f;
}

bool QuadTreeTerrain::IsTriangleContained(UINT index, float x, float z, float size)
{
	UINT vertexIndex = index * 3;
	float radius = size * 0.5f;

	float x1 = vertices[vertexIndex].position.x;
	float z1 = vertices[vertexIndex].position.z;
	vertexIndex++;

	float x2 = vertices[vertexIndex].position.x;
	float z2 = vertices[vertexIndex].position.z;
	vertexIndex++;

	float x3 = vertices[vertexIndex].position.x;
	float z3 = vertices[vertexIndex].position.z;

	float minX = min(x1, min(x2, x3));
	if (minX > (x + radius))
		return false;

	float minZ = min(z1, min(z2, z3));
	if (minZ > (z + radius))
		return false;

	float maxX = max(x1, max(x2, x3));
	if (maxX < (x - radius))
		return false;

	float maxZ = max(z1, max(z2, z3));
	if (maxZ < (z - radius))
		return false;

	return true;
}

UINT QuadTreeTerrain::ContainTriangleCount(float x, float z, float size)
{
	UINT count = 0;

	for (UINT i = 0; i < triangleCount; i++)
	{
		if (IsTriangleContained(i, x, z, size))
			count++;
	}

	return count;
}

void QuadTreeTerrain::CreateTreeNode(Node* node, float x, float z, float size)
{
	node->x = x;
	node->z = z;
	node->size = size;

	node->triangleCount = 0;
	node->mesh = nullptr;

	for (Node* child : node->children)
		child = nullptr;

	UINT triangles = ContainTriangleCount(x, z, size);

	if (triangles == 0)
		return;

	if (triangles > MIN_TRIANGLE)//쪼개야 하는 노드
	{
		for (UINT i = 0; i < 4; i++)
		{
			float offsetX = (((i % 2) == 0) ? -1.0f : 1.0f) * (size / 4.0f);
			float offsetZ = ((i < 2) ? -1.0f : 1.0f) * (size / 4.0f);

			node->children[i] = new Node();
			CreateTreeNode(node->children[i], x + offsetX, z + offsetZ, size * 0.5f);
		}

		return;
	}

	//Leaf Node
	node->triangleCount = triangles;

	UINT vertexCount = triangleCount * 3;
	VertexType* vertices = new VertexType[vertexCount];
	UINT* indices = new UINT[vertexCount];

	UINT index = 0, vertexIndex = 0;
	for (UINT i = 0; i < triangleCount; i++)
	{
		if (IsTriangleContained(i, x, z, size))
		{
			vertexIndex = i * 3;
			vertices[index] = this->vertices[vertexIndex];
			indices[index] = index;
			index++;

			vertexIndex++;
			vertices[index] = this->vertices[vertexIndex];
			indices[index] = index;
			index++;

			vertexIndex++;
			vertices[index] = this->vertices[vertexIndex];
			indices[index] = index;
			index++;
		}
	}

	node->mesh = new Mesh(vertices, sizeof(VertexType), vertexCount,
		indices, vertexCount);

	delete[] vertices;
	delete[] indices;
}
