#include "Framework.h"

Trail::Trail(string imageFile, Vector3* startPos, Vector3* endPos, UINT width)
    : startPos(startPos), endPos(endPos), width(width)
{
    material = new Material();
    material->SetShader(L"Basic/Texture.hlsl");
    material->SetDiffuseMap(imageFile);

    CreateMesh();

    rsState[0] = new RasterizerState();
    rsState[1] = new RasterizerState();
    rsState[1]->CullMode(D3D11_CULL_NONE);
    //rsState[1]->FillMode(D3D11_FILL_WIREFRAME);

    blendState[0] = new BlendState();
    blendState[1] = new BlendState();
    //blendState[1]->Alpha(true);

    worldBuffer = new WorldBuffer();
}

Trail::~Trail()
{
    delete material;
    delete mesh;

    delete rsState[0];
    delete rsState[1];

    delete blendState[0];
    delete blendState[1];

    delete worldBuffer;
}

void Trail::Update()
{
    if (!isActive) return;

    for (UINT i = 0; i <= width; i++)
    {
        Vector3 start = vertices[i * 2].position;
        Vector3 end = vertices[(i * 2) + 1].position;

        Vector3 startDestPos;
        Vector3 endDestPos;
        if (i == 0)
        {
            startDestPos = *startPos;
            endDestPos = *endPos;
        }
        else
        {
            startDestPos = vertices[(i - 1) * 2].position;
            endDestPos = vertices[(i - 1) * 2 + 1].position;
        }

        start = LERP(start, startDestPos, speed * DELTA);
        end = LERP(end, endDestPos, speed * DELTA);

        vertices[i * 2].position = start;
        vertices[(i * 2) + 1].position = end;
    }

    mesh->UpdateVertex(vertices.data(), vertices.size());
}

void Trail::Render()
{
    if (!isActive) return;

    worldBuffer->SetVSBuffer(0);

    mesh->IASet();
    material->Set();

    rsState[1]->SetState();
    blendState[1]->SetState();

    DC->DrawIndexed(indices.size(), 0, 0);

    rsState[0]->SetState();
    blendState[0]->SetState();
}

void Trail::GUIRender()
{
    ImGui::Text("TrailOption");
    ImGui::SliderFloat("Speed", &speed, 0, 10);
}

void Trail::CreateMesh()
{
    for (UINT i = 0; i <= width; i++)
    {
        VertexUV vertex;
        vertex.position = (*startPos) + Vector3(i, 0, 0);
        vertex.uv = { (float)i / width, 0.0f };
        vertices.push_back(vertex);
        vertex.position = (*endPos) + Vector3(i, 0, 0);
        vertex.uv = { (float)i / width, 1.0f };
        vertices.push_back(vertex);
    }

    for (UINT i = 0; i < width; i++)
    {
        indices.push_back(i * 2);
        indices.push_back(i * 2 + 2);
        indices.push_back(i * 2 + 1);

        indices.push_back(i * 2 + 1);
        indices.push_back(i * 2 + 2);
        indices.push_back(i * 2 + 3);
    }

    mesh = new Mesh(vertices.data(), sizeof(VertexUV), vertices.size(),
        indices.data(), indices.size());
}
