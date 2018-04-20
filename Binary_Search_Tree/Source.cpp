#include <iostream>
#include <string>
#include <utility>
#include <queue>
#include <cassert>
using namespace std;

template <class Key, class Value>
class BST
{
	struct node
	{
		Key key;
		Value value;
		node* left;
		node* right;
		int times; //TODO maintain all functions relative to count

		node(Key key, Value value): key(move(key)), value(move(value)), left(nullptr), right(nullptr), times(1)
		{
		}

		explicit node(node* new_node): key(new_node->key), value(new_node->value), left(new_node->left),
		                               right(new_node->right), times(new_node->times)
		{
		}
	};

	node* root_;
	int count_;

	node* insert_recursion(node* root, Key key, Value value)
	{
		if (root == nullptr) // if there is no element in the array, return a new pointer as its root
		{
			count_++;
			return new node(key, value);
		}
		++root->times;

		if (root->key == key)
		{
			root->value = value;
		}
		else if (key < root->key)
		{
			root->left = insert_recursion(root->left, key, value);
		}
		else
		{
			root->right = insert_recursion(root->right, key, value);
		}
		return root;
	}

	node* insert_without_recursion(node* root, Key key, Value value) 
	{
		node* root_copy = root;
		node* prev = root;
		bool left = false;
		if (root == nullptr)
		{
			count_++;
			return new node(key, value);
		}
		while (root_copy != nullptr)
		{
			++root_copy->times;
			if (root_copy->key == key)
			{
				root_copy->value = value;
				return root;
			}
			if (key < root_copy->key)
			{
				prev = root_copy;
				root_copy = root_copy->left;
				left = true;
			}
			else
			{
				prev = root_copy;
				root_copy = root_copy->right;
				left = false;
			}
		}
		if (left)
			prev->left = new node(key, value);
		else
			prev->right = new node(key, value);
		count_++;
		return root;
	}

	void destroy(node* node) // free space of binary tree
	{
		if (node != nullptr)
		{
			destroy(node->left);
			destroy(node->right);
			delete node;
			count_--;
		}
	}

	bool contain(Key key, node* root)
	{
		if (root == nullptr)
			return false;
		if (root->key == key)
			return true;
		if (key < root->key)
			return contain(key, root->left);
		return contain(key, root->right);
	}

	Value* search(Key key, node* root) // return a pointer to the value, so the user can change the value directly
	{
		if (root == nullptr)
			return nullptr;
		if (root->key == key)
			return &(root->value);
		if (key < root->key)
			return search(key, root->left);
		return search(key, root->right);
	}

	static void print_info(node* root)
	{
		cout << root->key << "->" << root->value << "->" << root->times << endl;
	}

	void pre_order(node* root)
	{
		if (root != nullptr)
		{
			print_info(root);
			pre_order(root->left);
			pre_order(root->right);
		}
	}

	void in_order(node* root)
	{
		if (root != nullptr)
		{
			in_order(root->left);
			print_info(root);
			in_order(root->right);
		}
	}

	void post_order(node* root)
	{
		post_order(root->left);
		post_order(root->right);
		print_info(root);
	}

	node* minimum(node* root)
	{
		if (root->left == nullptr)
			return root;
		return minimum(root->left);
	}

	node* maximum(node* root)
	{
		if (root->right == nullptr)
			return root;
		return maximum(root->right);
	}

	node* remove_min(node* root)
	{
		--root->times;
		if (root->left == nullptr)
		{
			// This works too
			/*
			count--;
			if (root->right == nullptr)
			{
				delete root;
				return nullptr;
			}
			else
			{
				node* root_copy = root;
				root = root->right;
				delete root_copy;
				return root;
			}
			*/

			node* substitute_node = root->right;
			delete root;
			count_--;
			return substitute_node;
		}

		root->left = remove_min(root->left);
		return root;
	}

	node* remove_max(node* root)
	{
		--root->times;
		if (root->right == nullptr)
		{
			node* left_node = root->left;
			count_--;
			delete root;
			return left_node;
		}
		root->right = remove_max(root->right);
		return root;
	}

	node* remove(node* root, Key key)
	{
		if (root == nullptr)
		{
			return nullptr;
		}
		if (root->key == key)
		{
			if (root->left == nullptr)
			{
				node* right_node = root->right;
				delete root;
				count_--;
				return root->right;
			}
			if (root->right == nullptr)
			{
				node* left_node = root->left;
				delete root;
				count_--;
				return left_node;
			}
			// auto* s = new node(minimum(root->right));
			// count_++;
			// s->right = remove_min(root->right);
			// s->left = root->left;
			// count_--;
			// delete root;
			// return s;

			auto* s = new node(maximum(root->left));
			s->times = root->times - 1;
			count_++;
			s->right = root->right;
			s->left = remove_max(root->left);
			count_--;
			delete root;
			return s;
		}
		--root->times;
		if (key < root->key)
		{
			root->left = remove(root->left, key);
		}
		else
		{
			root->right = remove(root->right, key);
		}
		return root;
	}

	node* find_key(Key key, node* root)
	{
		if (root == nullptr)
			return nullptr;
		if (root->key == key)
			return root;
		if (key < root->key)
			return find_key(key, root->left);
		return find_key(key, root->right);
	}


	node* ceil(Key key, node* root, node* right_bound)
	{
		if (root == nullptr)
			return right_bound;
		if (root->key == key)
			return root;
		if (key < root->key)
			return ceil(key, root->left, root);
		return ceil(key, root->right, right_bound);
	}


	node* floor(Key key, node* root, node* left_bound)
	{
		if (root == nullptr)
			return left_bound;
		if (root->key == key)
			return root;
		if (key < root->key)
			return floor(key, root->left, left_bound);
		return floor(key, root->right, root);
	}


public:
	BST()
	{
		root_ = nullptr;
		count_ = 0;
	}

	~BST()
	{
		destroy(root_);
	}

	int size() const
	{
		return count_;
	}

	bool is_empty() const
	{
		return count_ == 0;
	}

	void insert(Key key, Value value)
	{
		// root_ = insert_recursion(root_, key, value);
		root_ = insert_without_recursion(root_, key, value);
	}

	bool contain(Key key)
	{
		return contain(key, root_);
	}

	Value* search(Key key)
	{
		return search(key, root_);
	}

	void pre_order() // 前序遍历
	{
		pre_order(root_);
	}

	void in_order()
	{
		in_order(root_);
	}

	void post_order()
	{
		post_order(root_);
	}

	//层序遍历
	void level_order()
	{
		queue<node*> q;
		q.push(root_);
		while (!q.empty())
		{
			node* node = q.front();
			q.pop();
			print_info(node);
			if (node->left)
				q.push(node->left);
			if (node->right)
				q.push(node->right);
		}
	}

	Key minimum()
	{
		assert(count_ != 0);
		node* min_mode = minimun(root_);
		return min_mode->key;
	}

	Key maximum() //	without recursion
	{
		assert(count_ != 0);
		node* max_node = maximum(root_);
		return max_node->key;
	}

	void decrease_child_times(node* root)
	{
		--root->times;
		if (root->left != nullptr)
			decrease_child_times(root->left);
		if (root->right != nullptr)
			decrease_child_times(root->right);
	}

	void remove_min()
	{
		if (root_) // the prerequisite of root
		{
			root_ = remove_min(root_);
		}
	}

	void remove_max()
	{
		if (root_)
		{
			root_ = remove_max(root_);
		}
	}

	void remove(Key key)
	{
		root_ = remove(root_, key);
	}

	node* find_key(Key key)
	{
		return find_key(key, root_);
	}

	Key successor(Key key)
	{
		node* root = find_key(key);
		node* s = minimum(root->right);
		return s->key;
	}

	Key predecessor(Key key)
	{
		node* root = find_key(key);
		node* p = maximum(root->left);
		return p->key;
	}

	node* floor(Key key)
	{
		if (root_ != nullptr)
			return floor(key, root_, root_);
		return nullptr;
	}

	node* ceil(Key key)
	{
		if (root_ != nullptr)
			return ceil(key, root_, root_);
		return nullptr;
	}

	static int ranking(Key key, node* root, int rank)
	{
		if (root == nullptr)
			return 0;
		if (root->key == key)
		{
			if (root->left != nullptr) // if the object has left child, add the number of its left children
				return rank + 1 + root->left->times;
			return rank + 1; // else directly output
		}
		if (key < root->key)
		{
			return ranking(key, root->left, rank);
		}
		rank += (root->times) - (root->right->times);
		return ranking(key, root->right, rank);
	}

	int rank(Key key)
	{
		return ranking(key, root_, 0);
	}

	Key select(int target_rank, node* root) //TODO buggy
	{
		if (rank(root->key) == target_rank)
			return root->key;
		if (target_rank < rank(root->key))
			return select(target_rank, root->left);
		return select(target_rank, root->right);
	}

	Key select(int rank)
	{
		return select(rank, root_);
	}
};


int main(int argc, char* argv[])
{
	BST<int, string> a{};

	a.insert(41, "xun");
	a.insert(22, "xun");
	a.insert(58, "xun");
	a.insert(15, "xun");
	a.insert(33, "xun");
	a.insert(50, "xun");
	a.insert(63, "xun");
	a.insert(13, "xun");
	a.insert(13, "xun");
	a.insert(37, "xun");
	a.insert(42, "xun");
	a.insert(53, "xun");
	a.level_order();
	cout << "rank of 63 is " << a.rank(63) << endl;
	cout << "the key " << a.select(1) << " has the ranking of 1" << endl;

	system("pause");
}
