#pragma once

class AStar
{
private:
    UINT width, height;

    vector<Node*> nodes;
    Heap* heap;

public:
    AStar(UINT width = 20, UINT height = 20);
    ~AStar();

    void Update();
    void Render();

    void SetNode(class Terrain* terrain);

    void GetPath(IN int start, IN int end, OUT vector<Vector3>& path);

    int FindCloseNode(Vector3 pos);

private:
    void Reset();

    float GetDistance(int start, int end);

    void Extend(int center, int end);

    void SetEdge();
};