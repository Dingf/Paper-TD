#ifndef INC_TREE_H
#define INC_TREE_H

#include "type.h"
#include "list.h"

template <class T>
class Tree
{
	public:
		Tree(const T& data) 
		{
			rootNode = data;
		};
		~Tree()
		{
			SetStart();
			while (GetCurrentState() == true)
			{
				delete children.GetData();
				if (CheckEnd() == true)
				{
					break;
				}
				else
				{
					SetNext();
				}
			}
		}

		bool GetState() const { return children.GetState(); }
		bool GetCurrentState() const { return children.GetCurrentState(); }

		bool CheckEnd() const { return children.CheckEnd(); }
		bool CheckStart() const { return children.CheckStart(); }

		void AddChild(Tree<T> * tree) { children.Append(tree); }
		void AddChild(const T& data) { children.Append(new Tree<T>(data)); }
		void RemoveChild() { children.Remove(); }
		Tree<T> * GetChild() const { return children.GetData(); }
		T GetData() const { return children.GetData()->rootNode; }

		void SetStart() { children.SetStart(); }
		void SetEnd() { children.SetEnd(); }
		void SetNext() { children.SetNext(); }
		void SetPrevious() { children.SetPrevious(); }

		void PreOrderTraversal(void (*operation)(T& data))
		{
			operation(rootNode);
			SetStart();
			while (GetCurrentState() == true)
			{
				Tree<T> * tree = GetChild();
				tree->PreOrderTraversal(operation);
				SetNext();
			}
		}
		void PostOrderTraversal(void (*operation)(T& data))
		{
			SetStart();
			while (GetCurrentState() == true)
			{
				Tree<T> * tree = GetChild();
				tree->PostOrderTraversal(operation);
				SetNext();
			}
			operation(rootNode);
		}
	
	private:
		T rootNode;
		List<Tree<T> *> children;

};

#endif