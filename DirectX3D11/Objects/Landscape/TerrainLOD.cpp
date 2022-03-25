#include "Framework.h"

TerrainLOD::TerrainLOD(string heightFile)
	: cellsPerPatch(32), cellSpacing(5)
{
	material = new Material();
	material->SetShader(L"TS/TerrainLOD.hlsl");
	material->SetDiffuseMap("Textures/Landscape/Dirt2.png");

	hullShader = Shader::AddHS(L"TS/TerrainLOD.hlsl");
	domainShader = Shader::AddDS(L"TS/TerrainLOD.hlsl");

	heightMap = Texture::Add(heightFile);
	width = heightMap->Width();
	height = heightMap->Height();

	cellUV.x = 1.0f / width;
	cellUV.y = 1.0f / height;

	patchWidth = ((width - 1) / cellsPerPatch) + 1;
	patchHeight = ((height - 1) / cellsPerPatch) + 1;

	vertices.resize(patchWidth * patchHeight);
	indices.resize((patchWidth - 1) * (patchHeight - 1) * 4);

	terrainBuffer = new ValueBuffer();
	heightBuffer = new ValueBuffer();
	worldBuffer = new WorldBuffer();

	terrainBuffer->values[0] = 1.0f;
	terrainBuffer->values[1] = 1000.0f;
	terrainBuffer->values[2] = 1.0f;
	terrainBuffer->values[3] = 64.0f;

	heightBuffer->values[0] = 20.0f;

	CreatePatchVertex();
	CreatePatchIndex();
}

TerrainLOD::~TerrainLOD()
{
	delete material;
	delete mesh;

	delete terrainBuffer;
	delete heightBuffer;
	delete worldBuffer;
}

void TerrainLOD::Update()
{
	UpdateWorld();
}

void TerrainLOD::Render()
{
	mesh->IASet(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);

	worldBuffer->Set(world);
	worldBuffer->SetHSBuffer(0);

	terrainBuffer->SetHSBuffer(10);
	heightBuffer->SetDSBuffer(10);

	heightMap->SetDS(0);

	material->Set();
	hullShader->Set();
	domainShader->Set();

	DC->DrawIndexed(indices.size(), 0, 0);

	DC->HSSetShader(nullptr, nullptr, 0);
	DC->DSSetShader(nullptr, nullptr, 0);
}

void TerrainLOD::GUIRender()
{
	ImGui::Text("TerrainLOD Option");
	ImGui::SliderFloat("MinDistance", &terrainBuffer->values[0], 1.0f, 2000.0f);
	ImGui::SliderFloat("MaxDistance", &terrainBuffer->values[1], 1.0f, 2000.0f);
	ImGui::SliderFloat("MinQuality", &terrainBuffer->values[2], 1.0f, 100.0f);
	ImGui::SliderFloat("MaxQuality", &terrainBuffer->values[3], 1.0f, 100.0f);
	ImGui::SliderFloat("HeightScale", &heightBuffer->values[0], 1.0f, 100.0f);
}

void TerrainLOD::CreatePatchVertex()
{
	float halfWidth = width * cellSpacing * 0.5f;
	float halfHeight = height * cellSpacing * 0.5f;

	float tempWidth = width * cellSpacing / patchWidth;
	float tempHeight = height * cellSpacing / patchHeight;

	float du = 1.0f / patchWidth;
	float dv = 1.0f / patchHeight;

	for (UINT z = 0; z < patchHeight; z++)
	{
		float tempZ = halfHeight - z * tempHeight;

		for (UINT x = 0; x < patchWidth; x++)
		{
			float tempX = -halfHeight + x * tempWidth;

			UINT index = patchWidth * z + x;
			vertices[index].position = { tempX, 0.0f, tempZ };
			vertices[index].uv = { x * du, z * dv };
		}
	}
}

void TerrainLOD::CreatePatchIndex()
{
	UINT index = 0;
	for (UINT z = 0; z < patchHeight - 1; z++)
	{
		for (UINT x = 0; x < patchWidth - 1; x++)
		{
			indices[index++] = patchWidth * z + x;
			indices[index++] = patchWidth * z + x + 1;
			indices[index++] = patchWidth * (z + 1) + x;
			indices[index++] = patchWidth * (z + 1) + x + 1;
		}
	}

	mesh = new Mesh(vertices.data(), sizeof(VertexType), vertices.size(),
		indices.data(), indices.size());
}
