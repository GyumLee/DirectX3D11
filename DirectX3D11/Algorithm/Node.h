#pragma once

class Node : public BoxCollider
{
public:
	enum State
	{
		NONE, OPEN, CLOSED, USING, OBSTACLE
	};

private:
	friend class AStar;

	struct Edge
	{
		int index;
		float cost;
	};

	int index;
	int via;

	float f, g, h;

	State state;

	vector<Edge*> edges;

public:
	Node(Vector3 pos, int index);
	~Node();

	void Render();

	void AddEdge(Node* node);

	void SetState(State state) { this->state = state; }

	float GetCost() { return f; }
};