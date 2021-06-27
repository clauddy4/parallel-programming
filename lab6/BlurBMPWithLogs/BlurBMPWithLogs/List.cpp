#include "List.h"

void List::Append(int value)
{
	Node* newNode = new Node();
	newNode->value = value;
	newNode->next = m_first;
	m_first = newNode;
	m_count++;
}

int List::Pop()
{
	if (!m_first)
	{
		throw;
	}

	int result = m_first->value;

	Node* delNode = m_first;
	m_first = m_first->next;
	delete delNode;
	m_count--;

	return result;
}

void List::Clear()
{
	Node* nextNode = m_first;
	m_first = nullptr;
	while (nextNode)
	{
		Node* delNode = nextNode;
		nextNode = nextNode->next;
		delete delNode;
		m_count--;
	}
}