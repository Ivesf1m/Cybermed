#pragma once

#include <memory>

using std::shared_ptr;

namespace mf
{
	template<class sObject>
	struct mfKdTreeNode {
		sObject ptr;
		shared_ptr<mfKdTreeNode> pLeft;
		shared_ptr<mfKdTreeNode> pRight;

		mfKdTreeNode()
			: pLeft(nullptr), pRight(nullptr)
		{

		}
	};

	template<class sObject, class sObjectCompare>
	class mfKdTree
	{
	public:
		/** Constructor */
		mfKdTree(int dim);

		/** Destructor */
		~mfKdTree();

		void insert(sObject& obj);

		sObject& nearest(sObject& obj);
		sObject& nearestAndInsert(sObject& obj);

		int size();

	private:
		mfKdTreeNode<sObject>& add(sObject& obj);
		mfKdTreeNode<sObject>& nearestSearch(sObject& obj);

		sObjectCompare comp;
		shared_ptr< mfKdTreeNode<sObject> > root;
		int n;
		int ndim;
	};

	template<class sObject, class sObjectCompare>
	mfKdTree<sObject, sObjectCompare>::mfKdTree(int dim)
		: ndim(dim), n(0), root(nullptr)
	{
		assert(dim);
	}

	template<class sObject, class sObjectCompare>
	mfKdTree<sObject, sObjectCompare>::~mfKdTree()
	{

	}

	template<class sObject, class sObjectCompare>
	void mfKdTree<sObject, sObjectCompare>::insert(sObject& obj)
	{
		add(obj);
	}

	template<class sObject, class sObjectCompare>
	sObject& mfKdTree<sObject, sObjectCompare>::nearest(sObject& obj)
	{
		return nearestSearch(obj).ptr;
	}

	template<class sObject, class sObjectCompare>
	sObject& mfKdTree<sObject, sObjectCompare>::nearestAndInsert(sObject& obj)
	{
		mfKdTreeNode<sObject>& node = nearestSearch(obj);
		add(obj);
		return node.ptr;
	}

	template<class sObject, class sObjectCompare>
	mfKdTreeNode<sObject>& mfKdTree<sObject, sObjectCompare>::add(sObject& obj)
	{
		shared_ptr< mfKdTreeNode<sObject> > ant(nullptr);
		shared_ptr< mfKdTreeNode<sObject> > leaf(nullptr);
		leaf = root;
		int ld = 0, dim = 0;

		while (leaf) {
			ant = leaf;
			if (comp.greater(leaf->ptr, obj, dim)) {
				ld = 0;
				leaf = ant->pRight;
			}
			else {
				ld = 1;
				leaf = ant->pLeft;
			}

			if (++dim == ndim)
				dim = 0;
		}

		leaf = std::make_shared<mfKdTreeNode>();
		leaf->ptr = obj;

		if (ant)
			if (ld == 0)
				ant->pRight = leaf;
			else
				ant->pLeft = leaf;
		else {
			root = leaf;
			ant = root;
		}

		n++;
		return ant->ptr;
	}

	template<class sObject, class sObjectCompare>
	mfKdTreeNode<sObject>& mfKdTree<sObject, sObjectCompare>::nearestSearch(
		sObject& obj)
	{
		shared_ptr< mfKdTreeNode<sObject> > ant(nullptr);
		shared_ptr< mfKdTreeNode<sObject> > leaf(nullptr);
		leaf = root;
		int dim = 0;

		while (leaf) {
			ant = leaf;
			if (comp.greater(leaf->ptr, obj, dim)) {
				leaf = ant->pRight;
			}
			else
				leaf = ant->pLeft;

			if (++dim == ndim)
				dim = 0;
		}

		return *ant;
	}

	template<class sObject, class sObjectCompare>
	int mfKdTree<sObject, sObjectCompare>::size()
	{
		return n;
	}
}