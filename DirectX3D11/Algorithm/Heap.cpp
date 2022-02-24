#include "Framework.h"

Heap::Heap()
{
}

Heap::~Heap()
{
}

void Heap::Insert(Node* node)
{
	heap.push_back(node);
	UpdateUpper();
}

void Heap::UpdateUpper()
{
	int curIndex = heap.size() - 1;
	int parent = (curIndex - 1) / 2;

	while (curIndex != 0)
	{
		if (heap[parent]->GetCost() <= heap[curIndex]->GetCost())
			break;

		Swap(curIndex, parent);
		curIndex = parent;
		parent = (curIndex - 1) / 2;
	}
}

Node* Heap::DeleteRoot()
{
    Node* root = heap.front();
    Swap(0, heap.size() - 1);
    heap.pop_back();
    UpdateLower();

    return root;
}

void Heap::UpdateLower()
{
    int curIndex = 0;
    int lChild = 1;
    int rChild = 2;
    int minNode = curIndex;

    while (true)
    {
        if (lChild < heap.size() && heap[lChild]->GetCost() < heap[minNode]->GetCost())
            minNode = lChild;

        if (rChild < heap.size() && heap[rChild]->GetCost() < heap[minNode]->GetCost())
            minNode = rChild;

        if (minNode == curIndex)
            break;

        Swap(curIndex, minNode);
        curIndex = minNode;
        lChild = curIndex * 2 + 1;
        rChild = lChild + 1;
    }
}

void Heap::Swap(int n1, int n2)
{
    swap(heap[n1], heap[n2]);
}
