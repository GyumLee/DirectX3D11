#include "Framework.h"

ModelMesh::ModelMesh(string name)
	:name(name), indexCount(0)
{
}

ModelMesh::~ModelMesh()
{
	delete mesh;
}

void ModelMesh::Render()
{
	mesh->IASet();
	material->Set();

	DC->DrawIndexed(indexCount, 0, 0);
}

void ModelMesh::CreateMesh(void* vertexData, UINT vertexCount, void* indexData, UINT indexCount)
{
	mesh = new Mesh(vertexData, sizeof(ModelVertex), vertexCount,
		indexData, indexCount);

	this->indexCount = indexCount;
}
