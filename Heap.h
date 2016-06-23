#include <vector>


template<class T>
struct Less
{
	bool operator() (const T& l, const T& r)
	{
		return l < r; // operator<
	}
};

template<class T>
struct Greater
{
	bool operator() (const T& l, const T& r)
	{
		return l > r; // operator<
	}
};


template<class T, class Compare=Less<T>>//哈夫曼结点的仿函数
class Heap
{
public:
	Heap()
	{}
	Heap(const T* a, size_t size)
	{
		for (size_t i = 0; i < size; ++i)
		{
			_arrays.push_back(a[i]);
		}

		// 建堆
		for (int i = (_arrays.size() - 2) / 2; i >= 0; --i)
		{
			AdjustDown(i);
		}
	}

	void Push(const T& x)
	{
		_arrays.push_back(x);
		AdjustUp(_arrays.size() - 1);
	}

	void Pop()
	{
		assert(_arrays.size() > 0);
		swap(_arrays[0], _arrays[_arrays.size() - 1]);
		_arrays.pop_back();

		AdjustDown(0);
	}

	T& Top()
	{
		assert(_arrays.size() > 0);
		return _arrays[0];
	}

	bool Empty()
	{
		return _arrays.empty();
	}

	size_t Size()
	{
		return _arrays.size();
	}

	void AdjustDown(int root)
	{
		int child = root * 2 + 1;

		Compare com;
		while (child < _arrays.size())
		{
			// 比较出左右孩子中小的那个
			if (child + 1<_arrays.size() &&
				com(_arrays[child + 1], _arrays[child]))
			{
				++child;
			}

			if (com(_arrays[child], _arrays[root]))
			{
				swap(_arrays[child], _arrays[root]);
				root = child;
				child = 2 * root + 1;
			}
			else
			{
				break;
			}
		}
	}

	void AdjustUp(int child)
	{
		int parent = (child - 1) / 2;

		while (child > 0)
		{
			if (Compare()(_arrays[child], _arrays[parent]))
			{
				swap(_arrays[parent], _arrays[child]);
				child = parent;
				parent = (child - 1) / 2;
			}
			else
			{
				break;
			}
		}
	}

	void Print()
	{
		for (size_t i = 0; i < _arrays.size(); ++i)
		{
			cout << _arrays[i] << " ";
		}
		cout << endl;
	}

public:
	vector<T> _arrays;
};

//测试堆 
//void Test1()
//{
//	int a[10] = { 10, 11, 13, 12, 16, 18, 15, 17, 14, 19 };
//	Heap<int, Greater<int> > hp1(a, 10);
//	hp1.Push(1);
//	hp1.Print();
//
//	Heap<int> hp2(a, 10);
//	hp2.Push(1);
//	hp2.Print();
//
//
//	Less<int> less;
//	cout<<less(1, 2)<<endl;
//
//	Greater<int> greater;
//	cout<<greater(1, 2)<<endl;
//}

