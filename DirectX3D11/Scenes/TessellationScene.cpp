#include "Framework.h"
#include "TessellationScene.h"

TessellationScene::TessellationScene()
{
	material = new Material();
	//material->SetShader(L"TS/Triangle.hlsl");
	//hullShader = Shader::AddHS(L"TS/Triangle.hlsl");
	//domainShader = Shader::AddDS(L"TS/Triangle.hlsl");

	material->SetShader(L"TS/rectangle.hlsl");
	hullShader = Shader::AddHS(L"TS/rectangle.hlsl");
	domainShader = Shader::AddDS(L"TS/rectangle.hlsl");

	Vertex vertices[VERTEX_COUNT];
	//vertices[0].position = { +0.0f, +0.9f, 0.0f };
	//vertices[1].position = { +0.9f, -0.9f, 0.0f };
	//vertices[2].position = { -0.9f, -0.9f, 0.0f };

	vertices[0].position = { -0.9f, -0.9f, 0.0f };
	vertices[1].position = { -0.9f, +0.9f, 0.0f };
	vertices[2].position = { +0.9f, -0.9f, 0.0f };
	vertices[3].position = { +0.9f, +0.9f, 0.0f };

	vertexBuffer = new VertexBuffer(vertices, sizeof(Vertex), VERTEX_COUNT);

	rsState = new RasterizerState();
	rsState->FillMode(D3D11_FILL_WIREFRAME);
	rsState->SetState();

	edgeBuffer = new ValueBuffer();
	edgeBuffer->values[0] = 1;
	edgeBuffer->values[1] = 1;
	edgeBuffer->values[2] = 1;
	edgeBuffer->values[3] = 1;

	insideBuffer = new ValueBuffer();
	insideBuffer->values[0] = 1;
	insideBuffer->values[1] = 1;
}

TessellationScene::~TessellationScene()
{
	delete material;
	delete vertexBuffer;
	delete rsState;
	delete edgeBuffer;
	delete insideBuffer;
}

void TessellationScene::Update()
{
}

void TessellationScene::PreRender()
{
}

void TessellationScene::Render()
{
	//vertexBuffer->IASet(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
	vertexBuffer->IASet(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);

	//edgeBuffer->SetHSBuffer(10);

	edgeBuffer->SetHSBuffer(10);
	insideBuffer->SetHSBuffer(11);

	material->Set();
	hullShader->Set();
	domainShader->Set();

	DC->Draw(VERTEX_COUNT, 0);
}

void TessellationScene::PostRender()
{
}

void TessellationScene::GUIRender()
{
	ImGui::Text("Edge Option");
	//ImGui::DragFloat("Edge0", &edgeBuffer->values[0]);
	//ImGui::DragFloat("Edge1", &edgeBuffer->values[1]);
	//ImGui::DragFloat("Edge2", &edgeBuffer->values[2]);
	//ImGui::DragFloat("Inside", &edgeBuffer->values[3]);

	ImGui::DragFloat("Edge0", &edgeBuffer->values[0]);
	ImGui::DragFloat("Edge1", &edgeBuffer->values[1]);
	ImGui::DragFloat("Edge2", &edgeBuffer->values[2]);
	ImGui::DragFloat("Edge3", &edgeBuffer->values[3]);
	
	ImGui::DragFloat("Inside0", &insideBuffer->values[0]);
	ImGui::DragFloat("Inside1", &insideBuffer->values[1]);
}
