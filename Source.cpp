#include <iostream>

using namespace std;

template<typename T>
class Node
{
public:
	T value;
	Node* pNext = nullptr;
	Node* pPrevious = nullptr;
	unsigned int priority = 0;
};

template<typename T>
class Queue
{
private:
	int count = 0;
	Node<T> *pTop = nullptr;
	Node<T> *pFront = nullptr;
	void push(T value, int priority = 0);
	virtual Node<T>* nodeDequeue();
public:
	Queue(void);
	~Queue(void);
	virtual void enqueue(T value, int priority = 0);
	virtual T dequeue(void);
	int size(void);
};
template<typename T>
Queue<T>::Queue()
{
	pFront = pTop = nullptr;
}
template<typename T>
Queue<T>::~Queue()
{
	while (pTop != nullptr){ delete nodeDequeue(); }
}
template<typename T>
void Queue<T>::push(T value, int priority)
{
	Node<T>* pTmp = new Node<T>;
	pTmp->value = value;
	pTmp->pNext = pTop;
	pTmp->priority = priority;
	pTop = pTmp;
	count++;
}
template<typename T>
Node<T>* Queue<T>::nodeDequeue(void)
{
	Node<T> *pTmp = pFront;
	if (pFront != nullptr)
	{
		pFront = pFront->pPrevious;
		if (pFront != nullptr) pFront->pNext = nullptr;
		else pTop = pFront; //last node has been removed
	}

	return pTmp;
}
template<typename T>
T Queue<T>::dequeue(void)
{
	if (count == 0) throw "Queue Empty!";

	Node<T> *pTmp = nodeDequeue();
	T ret;

	if (pTmp != nullptr)
	{
		ret = pTmp->value;
	}
	else
	{
		throw "Error!";
	}
	delete pTmp;
	count--;
	return ret;
}
template<typename T>
void Queue<T>::enqueue(T value, int priority)
{
	this->push(value, priority);

	if (pFront == nullptr) pFront = pTop;
	else(pTop->pNext)->pPrevious = pTop;
}
template<typename T>
int Queue<T>::size()
{
	return count;
}
template<typename T>
class BasicScheduler : public Queue<T>
{
public:
	virtual T dequeue(void);

private:
	virtual Node<T>* nodeDequeue(void);
};
template<typename T>
Node<T>* BasicScheduler<T>::nodeDequeue(void)
{
	Node<T>* pHighestPriorityNode = pFront;
	Node<T>* pSeek = pFront;

	while (pSeek != nullptr)
	{
		if (pSeek->priority < pHighestPriorityNode->priority) pHighestPriorityNode = pSeek;
		pSeek = pSeek->pPrevious; // If the seeking pointer finds a node that has a higher priority (1 being the highest!)
	} //Then make highestprioritynode pointer equal to the seeking node

	if (pHighestPriorityNode == nullptr) throw "Error!"; // just to check nothing has gone wrong

	if (pFront == pHighestPriorityNode)
	{
		pFront = pFront->pPrevious; // we are removing the first item
		if (pFront != nullptr) pFront->pNext = nullptr;
		else pTop = pFront; // last node has been removed
	}
	else if (pTop == pHighestPriorityNode)
	{
		pTop = pTop->pNext; // we are removing the last item
		if (pTop != nullptr) pTop->pPrevious = nullptr;
		else pTop = pFront; // last node has been removed
	}
	else
	{
		//There must be at least 3 nodes left to get here 
		//Making two node pointers to be able to hold the positions in the chain when the 
		//Highest priority node has been removed
		Node<T>* pBefore = pHighestPriorityNode->pPrevious;
		Node<T>* pAfter = pHighestPriorityNode->pNext;
		//join both ends together 
		pBefore->pNext = pAfter;
		pAfter->pPrevious = pBefore;
	}

	return pHighestPriorityNode;
}
template<typename T>
T BasicScheduler<T>::dequeue(void)
{
	if (count == 0) throw "Scheduler Empty!";

	T ret;

	Node<T>* pHighestPriorityNode = nodeDequeue();
	ret = pHighestPriorityNode->value;
	count--;

	delete pHighestPriorityNode;
	return ret;
}
int main(void)
{
	Queue<int>* MyQueue = new Queue<int>;

	for (int count = 0; count < 10; count++)
	{
		MyQueue->enqueue(count);
	}
	try
	{
		while (true)
		{
			cout << MyQueue->dequeue() << " ";
		}
	}
	catch (char* message)
	{
		cout << message << endl;
	}

	delete MyQueue;
	return 0;
}