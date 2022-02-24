#include "Framework.h"

AStar::AStar(UINT width, UINT height)
	: width(width), height(height)
{
	heap = new Heap();
}

AStar::~AStar()
{
	for (Node* node : nodes)
		delete node;

	delete heap;
}

void AStar::Update()
{
	if (MOUSE_CLICK(1))
	{
		Ray ray = CAM->ScreenPointToRay(mousePos);

		for (Node* node : nodes)
		{
			if (node->RayCollision(ray))
			{
				node->SetState(Node::OBSTACLE);
				break;
			}
		}
	}
}

void AStar::Render()
{
	for (Node* node : nodes)
		node->Render();
}

void AStar::SetNode(Terrain* terrain)
{
	Float2 size = terrain->GetSize();

	Float2 interval;
	interval.x = size.x / width;
	interval.y = size.y / height;

	for (UINT z = 0; z <= height; z++)
	{
		for (UINT x = 0; x <= width; x++)
		{
			Vector3 pos = Vector3(x * interval.x, 0, z * interval.y);
			pos.y = terrain->GetHeight(pos);

			nodes.push_back(new Node(pos, nodes.size()));
		}
	}

	SetEdge();
}

void AStar::SetEdge()
{
	for (UINT i = 0; i < nodes.size(); i++)
	{
		if (i % (width + 1) != width)
		{
			nodes[i]->AddEdge(nodes[i + 1]);
			nodes[i + 1]->AddEdge(nodes[i]);
		}

		if (i < nodes.size() - width - 1)
		{
			nodes[i]->AddEdge(nodes[i + width + 1]);
			nodes[i + width + 1]->AddEdge(nodes[i]);
		}

		if (i < nodes.size() - width - 1 && i % (width + 1) != width)
		{
			nodes[i]->AddEdge(nodes[i + width + 2]);
			nodes[i + width + 2]->AddEdge(nodes[i]);
		}

		if (i < nodes.size() - width - 1 && i % (width + 1) != 0)
		{
			nodes[i]->AddEdge(nodes[i + width]);
			nodes[i + width]->AddEdge(nodes[i]);
		}
	}
}
