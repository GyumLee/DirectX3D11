#include "Framework.h"
#include "TutorialScene.h"

TutorialScene::TutorialScene()
{
	vertices.resize(8);

	vertices[0].position = { -0.5f, -0.5f, -0.5f };
	vertices[1].position = { -0.5f, +0.5f, -0.5f };
	vertices[2].position = { +0.5f, -0.5f, -0.5f };
	vertices[3].position = { +0.5f, +0.5f, -0.5f };

	vertices[4].position = { -0.5f, -0.5f, +0.5f };
	vertices[5].position = { -0.5f, +0.5f, +0.5f };
	vertices[6].position = { +0.5f, -0.5f, +0.5f };
	vertices[7].position = { +0.5f, +0.5f, +0.5f };

	vertices[0].color = { 1, 0, 0, 1 };
	vertices[1].color = { 0, 1, 0, 1 };
	vertices[2].color = { 0, 0, 1, 1 };
	vertices[3].color = { 1, 1, 0, 1 };
	vertices[4].color = { 1, 0, 1, 1 };
	vertices[5].color = { 0, 1, 1, 1 };
	vertices[6].color = { 0, 0, 0, 1 };
	vertices[7].color = { 1, 1, 1, 1 };
	
	/*
		5		7
	1		3

		4		6
	0		2
	*/

	indices = {
		//front
		0, 1, 2,
		2, 1, 3,
		//right
		2, 3, 6,
		6, 3, 7,
		//top
		1, 5, 3,
		3, 5, 7,
		//back
		/*7, 5, 6,
		6, 5, 4,*/
		4, 6, 5,
		5, 6, 7,
		//left
		/*5, 1, 4,
		4, 1, 0,*/
		0, 4, 1,
		1, 4, 5,
		//bottom
		/*6, 4, 2,
		2, 4, 0*/
		0, 2, 4,
		4, 2, 6
	};

	vertexBuffer = new VertexBuffer(vertices.data(), sizeof(VertexColor), vertices.size());
	indexBuffer = new IndexBuffer(indices.data(), indices.size());

	vertexShader = Shader::AddVS(L"Tutorial.hlsl");
	pixelShader = Shader::AddPS(L"Tutorial.hlsl");

	worldBuffer = new WorldBuffer();

	Texture* texture = Texture::Add("Landscape/Box.png");
}

TutorialScene::~TutorialScene()
{
	delete vertexBuffer;
	delete indexBuffer;

	delete worldBuffer;
}

void TutorialScene::Update()
{
	static float angle = 0.0f;
	angle += DELTA;
	Matrix world = XMMatrixRotationRollPitchYaw(angle, angle, 0.0f);
	worldBuffer->Set(world);
}

void TutorialScene::PreRender()
{
}

void TutorialScene::Render()
{
	worldBuffer->SetVSBuffer(0);

	vertexBuffer->IASet(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	indexBuffer->IASet();

	vertexShader->Set();
	pixelShader->Set();

	DC->DrawIndexed(indices.size(), 0, 0);
}

void TutorialScene::PostRender()
{
}

void TutorialScene::GUIRender()
{
	if (ImGui::Button("test"))
		ImGuiFileDialog::Instance()->OpenDialog("Test", "test", ".*", ".");

	if (ImGuiFileDialog::Instance()->Display("Test"))
		ImGuiFileDialog::Instance()->Close();
}
