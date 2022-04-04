#include "Framework.h"

TerrainEditor::TerrainEditor(UINT width, UINT height)
	: width(width), height(height), heightMap(nullptr),
	adjustValue(1.0f), editType(1), selectMap(0)
{
	material = new Material("TerrainEditor");
	material->SetShader(L"Terrain/TerrainEditor.hlsl");
	material->Load("TextData/Materials/Dirt2.mat");

	secondMap = Texture::Add("Textures/Landscape/Dirt.png");
	thirdMap = Texture::Add("Textures/Landscape/Stones.png");

	CreateMesh();
	CreateComputeData();

	worldBuffer = new WorldBuffer();
	brushBuffer = new BrushBuffer();

	comboList.push_back("HIEGHT");
	comboList.push_back("ALPHA");
}

TerrainEditor::~TerrainEditor()
{
	delete material;
	delete mesh;

	delete worldBuffer;
	delete brushBuffer;
}

void TerrainEditor::Update()
{
	brushBuffer->data.pickingPos = ComputePicking();

	if (MOUSE_PRESS(0) && !ImGui::GetIO().WantCaptureMouse)
	{
		switch (editType)
		{
		case 0:
			AdjustHeight();
			break;
		case 1:
			AdjustAlpha();
			break;
		}
	}

	if (MOUSE_UP(0) && !ImGui::GetIO().WantCaptureMouse)
	{
		InitNormalTangent();
		CreateNormal();
		CreateTangent();
		mesh->UpdateVertex(vertices.data(), vertices.size());
	}
}

void TerrainEditor::Render()
{
	secondMap->Set(11);
	thirdMap->Set(12);

	worldBuffer->SetVSBuffer(0);
	brushBuffer->SetPSBuffer(10);

	mesh->IASet();
	material->Set();

	DC->DrawIndexed(indices.size(), 0, 0);
}

void TerrainEditor::GUIRender()
{
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "TERRAIN EDIT");

	const char* brushList[] = { "CIRCLE", "RECTANGLE", "TRIANGLE" };
	ImGui::Combo("BRUSH TYPE", &brushBuffer->data.type, brushList, sizeof(brushList) / sizeof(const char*));

	ImGui::ColorEdit3("BRUSH COLOR", (float*)&brushBuffer->data.color);
	ImGui::DragFloat("BRUSH RANGE", &brushBuffer->data.range, 0.1f, 0, 100);
	ImGui::DragFloat("ADJUST VALUE", &adjustValue, 1.0f);

	const char* editList[100];
	//for (int i = 0; i < 100; i++)
	//	editList[i] = new char[100];
	for (int i = 0; i < comboList.size(); i++)
		editList[i] = comboList[i].c_str();
	ImGui::Combo("EDIT TYPE", &editType, editList, comboList.size());

	if (ImGui::ImageButton(secondMap->GetSRV(), ImVec2(50, 50)))
		selectMap = 0;
	ImGui::SameLine();
	if (ImGui::ImageButton(thirdMap->GetSRV(), ImVec2(50, 50)))
		selectMap = 1;

	SaveHeightMap();
	ImGui::SameLine();
	LoadHeightMap();

	SaveAlphaMap();
	ImGui::SameLine();
	LoadAlphaMap();

	material->GUIRender();
}

Vector3 TerrainEditor::Picking()
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

Vector3 TerrainEditor::ComputePicking()
{
	Ray ray = CAM->ScreenPointToRay(mousePos);

	rayBuffer->data.position = ray.position;
	rayBuffer->data.direction = ray.direction;
	rayBuffer->data.size = size;

	rayBuffer->SetCSBuffer(0);

	DC->CSSetShaderResources(0, 1, &structuredBuffer->GetSRV());
	DC->CSSetUnorderedAccessViews(0, 1, &structuredBuffer->GetUAV(), nullptr);

	computeShader->Set();

	UINT x = ceil((float)size / 64.0f);

	DC->Dispatch(x, 1, 1);

	structuredBuffer->Copy(output, sizeof(OutputDesc) * size);

	float minDistance = FLT_MAX;
	int minIndex = -1;
	for (UINT i = 0; i < size; i++)
	{
		OutputDesc temp = output[i];
		if (temp.picked)
		{
			if (minDistance > temp.distance)
			{
				minDistance = temp.distance;
				minIndex = i;
			}
		}
	}

	if (minIndex >= 0)
	{
		return ray.position + ray.direction * minDistance;
	}

	return Vector3();
}

void TerrainEditor::AdjustHeight()
{
	Vector3 pickingPos = brushBuffer->data.pickingPos;

	switch (brushBuffer->data.type)
	{
	case 0:
		for (VertexType& vertex : vertices)
		{
			Vector3 p = Vector3(vertex.position.x, 0, vertex.position.z);
			pickingPos.y = 0.0f;

			float distance = (p - pickingPos).Length();

			float temp = adjustValue * max(0, cos(XM_PIDIV2 * distance / brushBuffer->data.range));

			if (distance <= brushBuffer->data.range)
			{
				vertex.position.y += temp * DELTA;
				vertex.position.y = max(MIN_HEIGHT, vertex.position.y);
				vertex.position.y = min(MAX_HEIGHT, vertex.position.y);
			}
		}
		break;
	case 1:
	{
		float size = brushBuffer->data.range;

		float left = max(0, pickingPos.x - size);
		float right = max(0, pickingPos.x + size);
		float top = max(0, pickingPos.z + size);
		float bottom = max(0, pickingPos.z - size);

		for (UINT z = (UINT)bottom; z <= (UINT)top; z++)
		{
			for (UINT x = (UINT)left; x <= (UINT)right; x++)
			{
				UINT index = width * z + x;

				if (index >= vertices.size()) continue;

				vertices[index].position.y += adjustValue * DELTA;
				vertices[index].position.y = max(MIN_HEIGHT, vertices[index].position.y);
				vertices[index].position.y = min(MAX_HEIGHT, vertices[index].position.y);
			}
		}
		break;
	}
	case 2:
		for (VertexType& vertex : vertices)
		{
			pickingPos.y = 0.0f;

			float range = brushBuffer->data.range;

			Vector3 A = Vector3(cos(XMConvertToRadians(90)), 0, sin(XMConvertToRadians(90))) * range + pickingPos;
			Vector3 B = Vector3(cos(XMConvertToRadians(210)), 0, sin(XMConvertToRadians(210))) * range + pickingPos;
			Vector3 C = Vector3(cos(XMConvertToRadians(330)), 0, sin(XMConvertToRadians(330))) * range + pickingPos;

			Vector3 T = Vector3(vertex.position.x, 0, vertex.position.z);

			if (Vector3::Cross(B - A, T - A).y * Vector3::Cross(C - A, T - A).y > 0)
				continue;
			if (Vector3::Cross(C - B, T - B).y * Vector3::Cross(A - B, T - B).y > 0)
				continue;

			vertex.position.y += adjustValue * DELTA;
			vertex.position.y = max(MIN_HEIGHT, vertex.position.y);
			vertex.position.y = min(MAX_HEIGHT, vertex.position.y);
		}
		break;
	}

	mesh->UpdateVertex(vertices.data(), vertices.size());
}

void TerrainEditor::AdjustAlpha()
{
	Vector3 pickingPos = brushBuffer->data.pickingPos;

	switch (brushBuffer->data.type)
	{
	case 0:
		for (VertexType& vertex : vertices)
		{
			Vector3 p = Vector3(vertex.position.x, 0, vertex.position.z);
			pickingPos.y = 0.0f;

			float distance = (p - pickingPos).Length();

			float temp = adjustValue * max(0, cos(XM_PIDIV2 * distance / brushBuffer->data.range));

			if (distance <= brushBuffer->data.range)
			{
				vertex.alpha[selectMap] += temp * DELTA;

				vertex.alpha[selectMap] = max(0.0f, vertex.alpha[selectMap]);
				vertex.alpha[selectMap] = min(1.0f, vertex.alpha[selectMap]);
			}
		}
		break;
	case 1:
	{
		float size = brushBuffer->data.range;

		float left = max(0, pickingPos.x - size);
		float right = max(0, pickingPos.x + size);
		float top = max(0, pickingPos.z + size);
		float bottom = max(0, pickingPos.z - size);

		for (UINT z = (UINT)bottom; z <= (UINT)top; z++)
		{
			for (UINT x = (UINT)left; x <= (UINT)right; x++)
			{
				UINT index = width * z + x;

				if (index >= vertices.size()) continue;

				vertices[index].alpha[selectMap] += adjustValue * DELTA;

				vertices[index].alpha[selectMap] = max(0.0f, vertices[index].alpha[selectMap]);
				vertices[index].alpha[selectMap] = min(1.0f, vertices[index].alpha[selectMap]);
			}
		}
		break;
	}
	case 2:
		for (VertexType& vertex : vertices)
		{
			pickingPos.y = 0.0f;

			float range = brushBuffer->data.range;

			Vector3 A = Vector3(cos(XMConvertToRadians(90)), 0, sin(XMConvertToRadians(90))) * range + pickingPos;
			Vector3 B = Vector3(cos(XMConvertToRadians(210)), 0, sin(XMConvertToRadians(210))) * range + pickingPos;
			Vector3 C = Vector3(cos(XMConvertToRadians(330)), 0, sin(XMConvertToRadians(330))) * range + pickingPos;

			Vector3 T = Vector3(vertex.position.x, 0, vertex.position.z);

			if (Vector3::Cross(B - A, T - A).y * Vector3::Cross(C - A, T - A).y > 0)
				continue;
			if (Vector3::Cross(C - B, T - B).y * Vector3::Cross(A - B, T - B).y > 0)
				continue;

			vertex.alpha[selectMap] += adjustValue * DELTA;

			vertex.alpha[selectMap] = max(0.0f, vertex.alpha[selectMap]);
			vertex.alpha[selectMap] = min(1.0f, vertex.alpha[selectMap]);
		}
		break;
	}

	mesh->UpdateVertex(vertices.data(), vertices.size());
}

void TerrainEditor::CreateMesh()
{
	vector<Float4> pixels;
	if (heightMap)
	{
		width = heightMap->Width();
		height = heightMap->Height();

		pixels = heightMap->ReadPixels();
	}
	else
	{
		pixels.resize(width * height);
	}

	vertices.clear();
	indices.clear();

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

void TerrainEditor::CreateNormal()
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

void TerrainEditor::CreateTangent()
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

void TerrainEditor::CreateComputeData()
{
	rayBuffer = new RayBuffer();

	computeShader = Shader::AddCS(L"Terrain/Picking.hlsl");

	size = indices.size() / 3;

	input = new InputDesc[size];
	output = new OutputDesc[size];

	for (UINT i = 0; i < size; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		input[i].v0 = vertices[index0].position;
		input[i].v1 = vertices[index1].position;
		input[i].v2 = vertices[index2].position;

		input[i].index = i;
	}

	structuredBuffer = new StructuredBuffer(input, sizeof(InputDesc), size,
		sizeof(OutputDesc), size);
}

void TerrainEditor::InitNormalTangent()
{
	for (VertexType& vertex : vertices)
	{
		vertex.normal = Vector3();
		vertex.tangent = Vector3();
	}
}

void TerrainEditor::SaveHeightMap()
{
	if (ImGui::Button("SaveHeightMap"))
	{
		ImGuiFileDialog::Instance()->OpenDialog("SaveHeightMap", "SaveHeightMap",
			".png", ".");
	}

	if (ImGuiFileDialog::Instance()->Display("SaveHeightMap"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			string file = ImGuiFileDialog::Instance()->GetFilePathName();

			char path[128];
			GetCurrentDirectoryA(sizeof(path), path);

			file = file.substr(strlen(path) + 1, file.length());

			UINT size = width * height * 4;
			uint8_t* pixels = new uint8_t[size];

			for (UINT i = 0; i < size / 4; i++)
			{
				float y = vertices[i].position.y;

				uint8_t height = (y - MIN_HEIGHT) / (MAX_HEIGHT - MIN_HEIGHT) * 255;

				pixels[i * 4 + 0] = height; // R
				pixels[i * 4 + 1] = height; // G
				pixels[i * 4 + 2] = height; // B
				pixels[i * 4 + 3] = 255; // Alpha
			}

			Image image;
			image.width = width;
			image.height = height;
			image.pixels = pixels;
			image.format = DXGI_FORMAT_R8G8B8A8_UNORM;
			image.rowPitch = width * 4; // width size
			image.slicePitch = size; // whole size

			SaveToWICFile(image, WIC_FLAGS_FORCE_RGB, GetWICCodec(WIC_CODEC_PNG),
				ToWString(file).c_str());
		}

		ImGuiFileDialog::Instance()->Close();
	}
}

void TerrainEditor::LoadHeightMap()
{
	if (ImGui::Button("LoadHeightMap"))
	{
		ImGuiFileDialog::Instance()->OpenDialog("LoadHeightMap", "LoadHeightMap",
			".png", ".");
	}

	if (ImGuiFileDialog::Instance()->Display("LoadHeightMap"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			string file = ImGuiFileDialog::Instance()->GetFilePathName();

			char path[128];
			GetCurrentDirectoryA(sizeof(path), path);

			file = file.substr(strlen(path) + 1, file.length());

			if (!ExistFile(file)) return;

			heightMap = Texture::Add(file);

			delete mesh;

			CreateMesh();

			delete structuredBuffer;
			delete rayBuffer;
			delete[] input;
			delete[] output;

			CreateComputeData();
		}

		ImGuiFileDialog::Instance()->Close();
	}
}

void TerrainEditor::SaveAlphaMap()
{
	if (ImGui::Button("SaveAlphaMap"))
	{
		ImGuiFileDialog::Instance()->OpenDialog("SaveAlphaMap", "SaveAlphaMap",
			".png", ".");
	}

	if (ImGuiFileDialog::Instance()->Display("SaveAlphaMap"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			string file = ImGuiFileDialog::Instance()->GetFilePathName();

			char path[128];
			GetCurrentDirectoryA(sizeof(path), path);

			file = file.substr(strlen(path) + 1, file.length());

			UINT size = width * height * 4;
			uint8_t* pixels = new uint8_t[size];

			for (UINT i = 0; i < size / 4; i++)
			{
				pixels[i * 4 + 0] = vertices[i].alpha[0] * 255;
				pixels[i * 4 + 1] = vertices[i].alpha[1] * 255;
				pixels[i * 4 + 2] = vertices[i].alpha[2] * 255;
				pixels[i * 4 + 3] = 255;
			}

			Image image;
			image.width = width;
			image.height = height;
			image.pixels = pixels;
			image.format = DXGI_FORMAT_R8G8B8A8_UNORM;
			image.rowPitch = width * 4; // width size
			image.slicePitch = size; // whole size

			SaveToWICFile(image, WIC_FLAGS_FORCE_RGB, GetWICCodec(WIC_CODEC_PNG),
				ToWString(file).c_str());
		}

		ImGuiFileDialog::Instance()->Close();
	}
}

void TerrainEditor::LoadAlphaMap()
{
	if (ImGui::Button("LoadAlphaMap"))
	{
		ImGuiFileDialog::Instance()->OpenDialog("LoadAlphaMap", "LoadAlphaMap",
			".png", ".");
	}

	if (ImGuiFileDialog::Instance()->Display("LoadAlphaMap"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			string file = ImGuiFileDialog::Instance()->GetFilePathName();

			char path[128];
			GetCurrentDirectoryA(sizeof(path), path);

			file = file.substr(strlen(path) + 1, file.length());

			if (!ExistFile(file)) return;

			Texture* alphaMap = Texture::Add(file);

			vector<Float4> pixels = alphaMap->ReadPixels();

			for (UINT i = 0; i < vertices.size(); i++)
			{
				vertices[i].alpha[0] = pixels[i].x;
				vertices[i].alpha[1] = pixels[i].y;
				vertices[i].alpha[2] = pixels[i].z;
				vertices[i].alpha[3] = pixels[i].w;
			}

			mesh->UpdateVertex(vertices.data(), vertices.size());
		}

		ImGuiFileDialog::Instance()->Close();
	}
}
