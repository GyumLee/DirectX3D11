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

void ModelReader::Render(UINT instanceCount)
{
	for (ModelMesh* mesh : meshes)
		mesh->Render(instanceCount);
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

int ModelReader::GetBone(string name)
{
	if (boneMap.count(name) == 0) return -1;

	return boneMap[name];
}

Vector3 ModelReader::GetSize()
{
	Vector3 minBox, maxBox;
	meshes[0]->SetBox(&minBox, &maxBox);

	for (UINT i = 1; i < meshes.size(); i++)
	{
		Vector3 minPos, maxPos;

		meshes[i]->SetBox(&minPos, &maxPos);

		minBox.x = min(minBox.x, minPos.x);
		minBox.y = min(minBox.y, minPos.y);
		minBox.z = min(minBox.z, minPos.z);

		maxBox.x = max(maxBox.x, maxPos.x);
		maxBox.y = max(maxBox.y, maxPos.y);
		maxBox.z = max(maxBox.z, maxPos.z);
	}

	return maxBox - minBox;
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

	size = r.UInt();
	nodes.resize(size);
	for (NodeData& node : nodes)// 2)Debugging nodes
	{
		node.index = r.Int();
		node.name = r.String();
		node.parent = r.Int();
		node.transform = r.Matrix();

		SetMeshIndex(node.name, node.index);
	}
	
	size = r.UInt();// 1)Breakpoint here
	bones.resize(size);
	for (BoneData& bone : bones)
	{
		bone.name = r.String();
		bone.index = r.Int();
		bone.offset = r.Matrix();

		boneMap[bone.name] = bone.index;
	}
}

void ModelReader::SetMeshIndex(string name, int index)
{
	for (ModelMesh* mesh : meshes)
	{
		if (mesh->GetName() == name)
		{
			mesh->SetBoneIndex(index);
		}
	}
}
