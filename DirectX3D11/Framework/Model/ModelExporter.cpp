#include "Framework.h"

ModelExporter::ModelExporter(string name, string file)
	: name(name)
{
	importer = new Assimp::Importer();

	scene = importer->ReadFile(file,
		aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_MaxQuality);
	assert(scene != nullptr);
}

ModelExporter::~ModelExporter()
{
	delete importer;
}

void ModelExporter::ExportMaterial()
{
	ReadMaterial();
	WriteMaterial();
}

void ModelExporter::ExportMesh()
{
	ReadNode(scene->mRootNode, -1, -1);
	ReadMesh(scene->mRootNode);
	WriteMesh();
}

void ModelExporter::ReadMaterial()
{
	for (UINT i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* srcMaterial = scene->mMaterials[i];
		Material* material = new Material();
		material->name = srcMaterial->GetName().C_Str();

		aiColor3D color;
		MaterialBuffer::Data& data = material->GetData();

		srcMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		data.diffuse = Float4(color.r, color.g, color.b, 1.0f);

		srcMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
		data.specular = Float4(color.r, color.g, color.b, 1.0f);

		srcMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
		data.ambient = Float4(color.r, color.g, color.b, 1.0f);

		srcMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, color);
		data.emissive = Float4(color.r, color.g, color.b, 1.0f);

		srcMaterial->Get(AI_MATKEY_SHININESS, data.shininess);

		aiString file;
		srcMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &file);
		material->SetDiffuseMap(CreateTexture(file.C_Str()));
		file.Clear();

		srcMaterial->GetTexture(aiTextureType_SPECULAR, 0, &file);
		material->SetSpecularMap(CreateTexture(file.C_Str()));
		file.Clear();

		srcMaterial->GetTexture(aiTextureType_NORMALS, 0, &file);
		material->SetNormalMap(CreateTexture(file.C_Str()));
		file.Clear();

		string savePath = "TextData/Materials/" + name + "/" + material->name + ".mat";

		CreateFolders(savePath);
		material->Save(savePath);

		materials.push_back(material);
	}
}

void ModelExporter::WriteMaterial()
{
	string savePath = "ModelData/Materials/" + name + ".mats";

	CreateFolders(savePath);

	BinaryWriter w(savePath);

	w.UInt(materials.size());

	for (Material* material : materials)
	{
		w.String(material->name);

		delete material;
	}

	materials.clear();
}

string ModelExporter::CreateTexture(string file)
{
	if (file.length() == 0)
		return "";

	string fileName = GetFileNameWithoutExtension(file) + ".png";
	const aiTexture* texture = scene->GetEmbeddedTexture(file.c_str());

	string path = "Textures/Model/" + name + "/" + fileName;

	CreateFolders(path);

	if (ExistFile(path))
		return path;

	if (texture == nullptr)
		return "";

	if (texture->mHeight < 1)
	{
		BinaryWriter w(path);
		w.Byte(texture->pcData, texture->mWidth);
	}
	else
	{
		Image image;
		image.width = texture->mWidth;
		image.height = texture->mHeight;
		image.pixels = (uint8_t*)(texture->pcData);
		image.rowPitch = image.width * 4;
		image.slicePitch = image.width * image.height * 4;

		SaveToWICFile(image, WIC_FLAGS_NONE,
			GetWICCodec(WIC_CODEC_PNG), ToWString(path).c_str());
	}

	return path;
}

void ModelExporter::ReadMesh(aiNode* node)
{
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		MeshData* mesh = new MeshData();
		mesh->name = node->mName.C_Str();

		UINT index = node->mMeshes[i];
		aiMesh* srcMesh = scene->mMeshes[index];

		mesh->materialIndex = srcMesh->mMaterialIndex;

		UINT startVertex = mesh->vertices.size();

		vector<VertexWeights> vertexWeights(srcMesh->mNumVertices);

		ReadBone(srcMesh, vertexWeights);

		mesh->vertices.resize(srcMesh->mNumVertices);
		for (UINT v = 0; v < srcMesh->mNumVertices; v++)
		{
			ModelVertex vertex;
			memcpy(&vertex.position, &srcMesh->mVertices[v], sizeof(Float3));

			if(srcMesh->HasTextureCoords(0))
				memcpy(&vertex.uv, &srcMesh->mTextureCoords[0][v], sizeof(Float2));

			if(srcMesh->HasNormals())
				memcpy(&vertex.normal, &srcMesh->mNormals[v], sizeof(Float3));

			if (srcMesh->HasTangentsAndBitangents())
				memcpy(&vertex.tangent, &srcMesh->mTangents[v], sizeof(Float3));

			if (!vertexWeights.empty())
			{
				vertexWeights[v].Normalize();

				vertex.indices.x = (float)vertexWeights[v].indices[0];
				vertex.indices.y = (float)vertexWeights[v].indices[1];
				vertex.indices.z = (float)vertexWeights[v].indices[2];
				vertex.indices.w = (float)vertexWeights[v].indices[3];

				vertex.weights.x = vertexWeights[v].weights[0];
				vertex.weights.y = vertexWeights[v].weights[1];
				vertex.weights.z = vertexWeights[v].weights[2];
				vertex.weights.w = vertexWeights[v].weights[3];
			}

			mesh->vertices[v] = vertex;
		}

		mesh->indices.resize(srcMesh->mNumFaces * 3);
		for (UINT f = 0; f < srcMesh->mNumFaces; f++)
		{
			aiFace& face = srcMesh->mFaces[f];

			for (UINT k = 0; k < face.mNumIndices; k++)
			{
				mesh->indices[f * 3 + k] = face.mIndices[k] + startVertex;
			}
		}

		meshes.push_back(mesh);
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
		ReadMesh(node->mChildren[i]);
}

void ModelExporter::ReadNode(aiNode* node, int index, int parent)
{
	NodeData* nodeData = new NodeData();
	nodeData->index = index;
	nodeData->parent = parent;
	nodeData->name = node->mName.C_Str();

	Matrix matrix(node->mTransformation[0]);
	nodeData->transform = XMMatrixTranspose(matrix);

	nodes.push_back(nodeData);

	for (UINT i = 0; i < node->mNumChildren; i++)
		ReadNode(node->mChildren[i], nodes.size(), index);
}

void ModelExporter::ReadBone(aiMesh* mesh, vector<VertexWeights>& vertexWeights)
{
	for (UINT i = 0; i < mesh->mNumBones; i++)
	{
		UINT boneIndex = 0;
		string name = mesh->mBones[i]->mName.C_Str();

		if (boneMap.count(name) == 0)
		{
			boneIndex = boneCount++;
			boneMap[name] = boneIndex;

			BoneData* boneData = new BoneData;
			boneData->name = name;
			boneData->index = boneIndex;

			Matrix matrix(mesh->mBones[i]->mOffsetMatrix[0]);
			boneData->offset = XMMatrixTranspose(matrix);

			bones.push_back(boneData);
		}
		else
		{
			boneIndex = boneMap[name];
		}

		for (UINT j = 0; j < mesh->mBones[i]->mNumWeights; j++)
		{
			UINT index = mesh->mBones[i]->mWeights[j].mVertexId;
			vertexWeights[index].Add(boneIndex, mesh->mBones[i]->mWeights[j].mWeight);
		}
	}
}

void ModelExporter::WriteMesh()
{
	string path = "ModelData/Meshes/" + name + ".mesh";

	CreateFolders(path);

	BinaryWriter w(path);

	w.UInt(meshes.size());
	for (MeshData* mesh : meshes)
	{
		w.String(mesh->name);
		w.UInt(mesh->materialIndex);

		w.UInt(mesh->vertices.size());
		w.Byte(mesh->vertices.data(), sizeof(ModelVertex) * mesh->vertices.size());

		w.UInt(mesh->indices.size());
		w.Byte(mesh->indices.data(), sizeof(UINT) * mesh->indices.size());

		delete mesh;
	}
	meshes.clear();

	w.UInt(nodes.size());
	for (NodeData* node : nodes)
	{
		w.UInt(node->index);
		w.String(node->name);

		w.UInt(node->parent);
		w.Matrix(node->transform);

		delete node;
	}
	nodes.clear();

	w.UInt(bones.size());
	for (BoneData* bone : bones)
	{
		w.String(bone->name);
		w.UInt(bone->index);
		w.Matrix(bone->offset);

		delete bone;
	}
	bones.clear();
}
