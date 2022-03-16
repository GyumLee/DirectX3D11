#include "Framework.h"

Spark::Spark(string imageFile, bool isAdditive)
{
	material->SetShader(L"Geometry/Spark.hlsl");
	material->SetDiffuseMap(imageFile);

	geometryShader = Shader::AddGS(L"Geometry/Spark.hlsl");

	sparkBuffer = new ValueBuffer();
	startColorBuffer = new ColorBuffer();
	endColorBuffer = new ColorBuffer();

	if (isAdditive)
		blendState[1]->Additive();

	Create();
}

Spark::~Spark()
{
	delete sparkBuffer;
	delete[] vertices;

	delete startColorBuffer;
	delete endColorBuffer;
}

void Spark::Update()
{
	if (!isActive) return;

	sparkBuffer->values[0] += DELTA;

	if (sparkBuffer->values[0] > sparkBuffer->values[1])
		Stop();
}

void Spark::Render()
{
	sparkBuffer->SetVSBuffer(10);
	startColorBuffer->SetPSBuffer(10);
	endColorBuffer->SetPSBuffer(11);

	Particle::Render();
}

void Spark::GUIRender()
{
	ImGui::Text("Spark Option");
	ImGui::SliderInt("ParticleCount", (int*)&particleCount, 0, MAX_COUNT);
	ImGui::SliderFloat("Duration", &sparkBuffer->values[1], 0.0f, 10.0f);
	ImGui::SliderFloat("MinRadius", &minRadius, 1.0f, maxRadius);
	ImGui::SliderFloat("MaxRadius", &maxRadius, minRadius, 50.0f);
	ImGui::SliderFloat("MinSize", &minSize, 0.1f, maxSize);
	ImGui::SliderFloat("MaxSize", &maxSize, minSize, 10.0f);

	ImGui::ColorEdit4("StartColor", (float*)&startColorBuffer->color);
	ImGui::ColorEdit4("EndColor", (float*)&endColorBuffer->color);
}

void Spark::Play(Vector3 position)
{
	sparkBuffer->values[0] = 0.0f;
	Particle::Play(position);

	UpdateParticle();
	vertexBuffer->Update(vertices, particleCount);
}

void Spark::Create()
{
	vertices = new VertexParticle[MAX_COUNT];
	vertexBuffer = new VertexBuffer(vertices, sizeof(VertexParticle), MAX_COUNT);

	particleCount = 500;
	sparkBuffer->values[1] = 1.0f;
}

void Spark::UpdateParticle()
{
	for (UINT i = 0; i < particleCount; i++)
	{
		vertices[i].position = position;

		float size = Random(minSize, maxSize);
		vertices[i].size = Float2(size, size);

		float radius = Random(minRadius, maxRadius);

		Vector3 velocity = Vector3(0, 0, radius);

		Float3 rot;
		rot.x = Random(0.0f, XM_2PI);
		rot.y = Random(0.0f, XM_2PI);
		rot.z = Random(0.0f, XM_2PI);

		Matrix matRot = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);
		XMStoreFloat3(&vertices[i].velocity, XMVector3TransformCoord(velocity, matRot));
	}
}
