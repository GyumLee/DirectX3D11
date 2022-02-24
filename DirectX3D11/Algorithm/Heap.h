#pragma once

class Heap
{
private:
	vector<Node*> heap;

public:
	Heap();
	~Heap();

	void Insert(Node* node);
	void UpdateUpper();

	Node* DeleteRoot();
	void UpdateLower();

	void Swap(int n1, int n2);

	void Clear() { heap.clear(); }
	bool Empty() { return heap.empty(); }
};