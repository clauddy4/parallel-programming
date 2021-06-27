#pragma once
class List
{
	struct Node
	{
		int value;
		Node* next;
	};

public:
	void Append(int value);
	int Pop();
	void Clear();
	int GetCount() { return m_count; }

	~List() { Clear(); }

private:
	Node* m_first = nullptr;
	int m_count = 0;
};

