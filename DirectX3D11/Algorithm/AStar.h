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

private:
	void SetEdge();
};