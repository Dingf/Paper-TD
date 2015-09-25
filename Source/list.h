#ifndef INC_LINKED_LIST_H
#define INC_LINKED_LIST_H

#include "type.h"

template <class T>
class List
{
	public:
		List()
		{
			size = NULL;
			first = NULL;
			last = NULL;
			current = NULL;
		}
		~List()
		{
			Unload();
		}
		
		const T& operator [] (uint32 index) const { return GetNode(index)->data; }
		      T& operator [] (uint32 index)       { return GetNode(index)->data; }

		const T& GetData(uint32 index) const { return GetNode(index)->data; }
		      T& GetData(uint32 index)       { return GetNode(index)->data; }

		const T& GetCurrent() const { return current->data; }
		      T& GetCurrent()       { return current->data; }

		const T& GetLast() const { return last->data; }
		      T& GetLast()       { return last->data; }

		const T& GetFirst() const { return first->data; }
		      T& GetFirst()       { return first->data; }

		bool GetState() const { return (first != NULL); }
		bool GetCurrentState() const { return (current != NULL); }

		bool CheckEnd() const { return (current == last); }
		bool CheckStart() const { return (current == first); }

		uint32 GetSize() const { return size; }

		void ToStart() { current = first; }
		void ToEnd() { current = last; }

		bool ToNext() 
		{
			if (current->next->GetState() == true)
			{
				current = current->next;
				return true;
			}
			return false;
		}
		bool ToPrevious()
		{
			if (current->previous->GetState() == true)
			{
				current = current->previous;
				return true;
			}
			return false;
		}

		bool SetData(const T data, uint32 index)
		{
			if (index <= size)
			{
				Node<T> * node = GetNode(index);
				node->data = data;
				return true;
			}
			return false;
		}

		bool Insert(const T& data, uint32 index)
		{
			uint32 listSize = size;
			if (index <= listSize)
			{
				Node<T> * node = new Node<T>;
				node->data = data;

				Node<T> * prevNode = GetNode(index - 1);
				Node<T> * nextNode = GetNode(index);
				if (prevNode != NULL)
				{
					prevNode->next = node;
					node->previous = prevNode;
				}
				else
				{
					first = node;
				}
				if (nextNode != NULL)
				{
					nextNode->previous = node;
					node->next = nextNode;
				}
				else
				{
					last = node;
				}
				size++;
				return true;
			}
			return false;
		}

		bool Append(const T& data) { return Insert(data, size); }

		void Remove(uint32 index)
		{
			Node<T> * node = GetNode(index);
			if ((node != NULL) && (node->GetState() == true))
			{
				if (node == current)
				{
					ToPrevious();
				}
				if (node == first)
				{
					first = node->next;
				}
				if (node == last)
				{
					last = node->previous;
				}
				if (node->previous->GetState() == true)
				{
					node->previous->next = node->next;
				}
				if (node->next->GetState() == true)
				{
					node->next->previous = node->previous;
				}
				delete node;
				node = NULL;
				size--;
			}
		}

		void Copy(const List& l)
		{
			Unload();

			for (uint32 i = 0; i < l.size; i++)
			{
				Append(l[i]);
			}
		}

		void Unload()
		{
			Node<T> * node = first;

			while (node != NULL)	//Don't need to call Remove(), just clear everything
			{
				current = node->next;
				delete node;
				node = current;
			}
			first = NULL;
			last = NULL;
			size = NULL;
		}

	private:
		template <class T>
		struct Node
		{
			Node()
			{
				previous = NULL;
				next = NULL;
			}

			bool GetState() const { return this != NULL; }

			T data;
			Node<T> * previous;
			Node<T> * next;
		};

		const Node<T> * GetNode(uint32 index) const
		{
			if (index < size)
			{
				if ((first != NULL) && (index <= size/2))
				{
					Node<T> * node = first;
					for (uint32 i = 0; i < index; i++)
					{
						node = node->next;
					}
					return node;
				}
				else if ((last != NULL) && (index > size/2))
				{
					Node<T> * node = last;
					for (uint32 i = (size - 1); i > index; i--)
					{
						node = node->previous;
					}
					return node;
				}
			}
			return NULL;
		}

		Node<T> * GetNode(uint32 index)
		{
			if (index < size)
			{
				if ((first != NULL) && (index <= size/2))
				{
					Node<T> * node = first;
					for (uint32 i = 0; i < index; i++)
					{
						node = node->next;
					}
					return node;
				}
				else if ((last != NULL) && (index > size/2))
				{
					Node<T> * node = last;
					for (uint32 i = (size - 1); i > index; i--)
					{
						node = node->previous;
					}
					return node;
				}
			}
			return NULL;
		}

		uint32 size;
		Node<T> * first;
		Node<T> * last;
		Node<T> * current;
};

#endif