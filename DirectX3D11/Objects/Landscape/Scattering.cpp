#include "Framework.h"

Scattering::Scattering()
	:radius(10), slices(32), stacks(16)
{
	buffer = new ScatteringBuffer();

	targets[0] = new RenderTarget();
	targets[1] = new RenderTarget();

	depthStencil = new DepthStencil();

	rayleighTexture = new RenderTexture({ 200, 100 });
	rayleighTexture->position = { 100, 150, 0 };
	rayleighTexture->SetSRV(targets[0]->GetSRV());
	rayleighTexture->UpdateWorld();

	mieTexture = new RenderTexture({ 200, 100 });
	mieTexture->position = { 100, 50, 0 };
	mieTexture->SetSRV(targets[1]->GetSRV());
	mieTexture->UpdateWorld();

	quad = new Quad(2, 2);
	quad->GetMaterial()->SetShader(L"Scattering/ScatteringTarget.hlsl");

	CreateSphere();

	depthStencilState[0] = new DepthStencilState();
	depthStencilState[1] = new DepthStencilState();
	depthStencilState[1]->DepthEnable(false);
}

Scattering::~Scattering()
{
	delete buffer;

	delete targets[0];
	delete targets[1];
	delete depthStencil;

	delete rayleighTexture;
	delete mieTexture;

	delete quad;

	//Sphere
	delete mesh;
	delete material;
	delete depthStencilState[0];
	delete depthStencilState[1];
}

void Scattering::PreRender()
{
	RenderTarget::SetMulti(targets, 2, depthStencil);

	buffer->SetPSBuffer(10);

	quad->Render();
}

void Scattering::Render()
{
	mesh->IASet();

	DC->PSSetShaderResources(10, 1, &targets[0]->GetSRV());
	DC->PSSetShaderResources(11, 1, &targets[1]->GetSRV());

	material->Set();

	depthStencilState[1]->SetState();
	DC->DrawIndexed(indexCount, 0, 0);
	depthStencilState[0]->SetState();
}

void Scattering::PostRender()
{
	rayleighTexture->Render();
	mieTexture->Render();
}

void Scattering::GUIRender()
{
	ImGui::SliderInt("SampleCount", &buffer->data.sampleCount, 1, 50);
}

void Scattering::CreateSphere()
{
	material = new Material();
	material->SetShader(L"Scattering/Scattering.hlsl");
	material->SetDiffuseMap("Textures/Landscape/night.png");

	UINT domeCount = slices;
	UINT latitude = stacks;
	UINT longitude = domeCount;

	vertexCount = longitude * latitude * 2;
	indexCount = (longitude - 1) * (latitude - 1) * 2 * 6;

	VertexUV* vertices = new VertexUV[vertexCount];

	UINT index = 0;
	for (UINT i = 0; i < longitude; i++)
	{
		float xz = 100.0f * (i / (longitude - 1.0f)) * XM_PI / 180.0f;

		for (UINT j = 0; j < latitude; j++)
		{
			float y = XM_PI * j / (latitude - 1);

			vertices[index].position.x = sin(xz) * cos(y) * radius;
			vertices[index].position.y = cos(xz) * radius;
			vertices[index].position.z = sin(xz) * sin(y) * radius;

			vertices[index].uv.x = 0.5f / (float)longitude + i / (float)longitude;
			vertices[index].uv.y = 0.5f / (float)latitude + j / (float)latitude;

			index++;
		}
	}

	for (UINT i = 0; i < longitude; i++)
	{
		float xz = 100.0f * (i / (longitude - 1.0f)) * XM_PI / 180.0f;

		for (UINT j = 0; j < latitude; j++)
		{
			float y = (XM_PI * 2.0f) - (XM_PI * j / (latitude - 1));

			vertices[index].position.x = sin(xz) * cos(y) * radius;
			vertices[index].position.y = cos(xz) * radius;
			vertices[index].position.z = sin(xz) * sin(y) * radius;

			vertices[index].uv.x = 0.5f / (float)longitude + i / (float)longitude;
			vertices[index].uv.y = 0.5f / (float)latitude + j / (float)latitude;

			index++;
		}
	}

	index = 0;
	UINT* indices = new UINT[indexCount];
	for (UINT i = 0; i < longitude - 1; i++)
	{
		for (UINT j = 0; j < latitude - 1; j++)
		{
			indices[index++] = i * latitude + j;
			indices[index++] = (i + 1) * latitude + j;
			indices[index++] = (i + 1) * latitude + j + 1;

			indices[index++] = (i + 1) * latitude + j + 1;
			indices[index++] = i * latitude + j + 1;
			indices[index++] = i * latitude + j;
		}
	}

	UINT offset = latitude * longitude;
	for (UINT i = 0; i < longitude - 1; i++)
	{
		for (UINT j = 0; j < latitude - 1; j++)
		{
			indices[index++] = offset + i * latitude + j;
			indices[index++] = offset + (i + 1) * latitude + j + 1;
			indices[index++] = offset + (i + 1) * latitude + j;

			indices[index++] = offset + i * latitude + j + 1;
			indices[index++] = offset + (i + 1) * latitude + j + 1;
			indices[index++] = offset + i * latitude + j;
		}
	}

	mesh = new Mesh(vertices, sizeof(VertexUV), vertexCount,
		indices, indexCount);

	delete[] vertices;
	delete[] indices;
}
