#pragma once

class Trail
{
public:
    bool isActive = true;

private:
    Material* material;
    Mesh* mesh;

    vector<VertexUV> vertices;
    vector<UINT> indices;

    Vector3* startPos;
    Vector3* endPos;

    UINT width;

    RasterizerState* rsState[2];
    BlendState* blendState[2];

    WorldBuffer* worldBuffer;

    float speed = 10.0f;
public:
    Trail(string imageFile, Vector3* startPos, Vector3* endPos, UINT width);
    ~Trail();

    void Update();
    void Render();
    void GUIRender();

private:
    void CreateMesh();
};