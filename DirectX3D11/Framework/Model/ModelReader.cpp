#include "Framework.h"

ModelReader::ModelReader(string name)
	: name(name)
{
	ReadMaterial();
	ReadMesh();
}

ModelReader::~ModelReader()
{
	for (Material* material : materials)
		delete material;

	for (ModelMesh* mesh : meshes)
		delete mesh;
}

void ModelReader::Render()
{
	for (ModelMesh* mesh : meshes)
		mesh->Render();
}

void ModelReader::GUIRender()
{
	for (Material* material : materials)
		material->GUIRender();
}

void ModelReader::SetShader(wstring file)
{
	for (Material* material : materials)
		material->SetShader(file);
}

void ModelReader::ReadMaterial()
{
	string path = "ModelData/Materials/" + name + ".mats";

	BinaryReader r(path);

	UINT size = r.UInt();

	materials.resize(size);

	for (Material*& material : materials)
	{
		material = new Material();
		string path = "TextData/Materials/" + name + "/" + r.String() + ".mat";

		material->Load(path);
	}
}

void ModelReader::ReadMesh()
{
	string path = "ModelData/Meshes/" + name + ".mesh";

	BinaryReader r(path);

	UINT size = r.UInt();

	meshes.resize(size);

	for (ModelMesh*& mesh : meshes)
	{
		mesh = new ModelMesh(r.String());
		UINT materialIndex = r.UInt();
		mesh->SetMaterial(materials[materialIndex]);

		//vertices
		UINT vertexCount = r.UInt();

		ModelVertex* vertices = new ModelVertex[vertexCount];
		r.Byte((void**)&vertices, sizeof(ModelVertex) * vertexCount);

		//indices
		UINT indexCount = r.UInt();

		UINT* indices = new UINT[indexCount];
		r.Byte((void**)&indices, sizeof(UINT) * indexCount);

		mesh->CreateMesh(vertices, vertexCount, indices, indexCount);

		delete[] vertices;
		delete[] indices;
	}
}
