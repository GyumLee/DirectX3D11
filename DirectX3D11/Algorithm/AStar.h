#pragma once

class AStar
{
private:
    UINT width, height;

    vector<Node*> nodes;
    Heap* heap;

    vector<Collider*> obstacles;

public:
    AStar(UINT width = 20, UINT height = 20);
    ~AStar();

    void Update();
    void Render();

    void SetNode(class Terrain* terrain);

    void GetPath(IN int start, IN int end, OUT vector<Vector3>& path);
    void MakeDirectPath(IN Vector3 start, IN Vector3 end, OUT vector<Vector3>& path);

    int FindCloseNode(Vector3 pos);

    bool CollisionObstacle(Ray ray, float distance);

    void SetObstacleNode();
    void AddObstacle(Collider* collider) { obstacles.push_back(collider); }

private:
    void Reset();

    float GetDistance(int start, int end);

    void Extend(int center, int end);

    void SetEdge();
};