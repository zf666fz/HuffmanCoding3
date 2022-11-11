#include<iostream>
#include<assert.h>
#include<queue>
#include<windows.h>
using namespace std;

//交换数组中两个下标的元素
template <class T>
void Swap(T array[], int pos1, int pos2);

//堆类
template <class T> class heap 
{
private:
	T* Heap; //数组指针
	int maxsize; //最大容量
	int n; //当前容量
	//下沉算法
	void siftdown(int pos)
	{
		while (!isLeaf(pos)) //如果是叶节点就停止循环
		{
			int j = leftchild(pos);
			int rc = rightchild(pos);
			if ((rc < n) && (Heap[rc]->weight() < Heap[j]->weight()))j = rc; //将j赋值为较大的子节点的值
			if (Heap[pos]->weight() < Heap[j]->weight()) return; //父节点的值最大时完成
			Swap(Heap, pos, j);//交换父节点与较大子节点的值
			pos = j; //执行下一个
		}
	}
public:
	//构造函数
	heap(T* h, int num, int max) { Heap = h; n = num; maxsize = max; buildHeap(); }
	//返回当前堆的大小
	int size() const { return n; }
	//判断是否为叶节点
	bool isLeaf(int pos) const { return (pos >= n / 2) && (pos < n); }
	//返回左子节点的位置
	int leftchild(int pos) const{ return 2 * pos + 1; } 
	//返回右子节点的位置
	int rightchild(int pos) const{ return 2 * pos + 2; } 
	//返回父节点的位置
	int parent(int pos) const { return (pos - 1) / 2; }
	//构建堆
	void buildHeap() { for (int i = n / 2 - 1; i >= 0; i--) siftdown(i); }
	//将data插入到堆中
	void insert(const T& data) 
	{
		assert(n < maxsize);
		int curr = n++;
		Heap[curr] = data; //从堆的末尾开始
		// Now sift up until curr’s parent > curr
		while ((curr != 0) && (Heap[curr]->weight() < Heap[parent(curr)]->weight())) 
		{
			Swap(Heap, curr, parent(curr));
			curr = parent(curr);
		}
	}
	//移除堆中的第一个元素
	T removefirst() 
	{
		assert(n > 0);
		Swap(Heap, 0, --n); //交换首尾元素
		if (n != 0) siftdown(0); //下沉跟元素
		return Heap[n]; //返回删除的元素
	}
};

//抽象节点类
template<class T>
class HuffNode
{
public:
	virtual int weight() = 0;//权重
	virtual bool isLeaf() = 0;//判断是否为叶节点
	virtual T val() = 0;//存储元素值
	virtual HuffNode<T>* left() const = 0;//左子节点
	virtual HuffNode<T>* right() const = 0;//右子节点
	virtual void setWeight(int w) = 0;//设置权重
	virtual string getCode() = 0;//哈夫曼编码
	virtual void setCode(string icode) = 0;//设置哈夫曼编码
};

//叶节点类
template<class T>
class LeafNode : public HuffNode<T>
{
private:
	T data;//数据
	int wgt;//权重
	string code;
public:
	//构造方法
	LeafNode(const T& val, int freq) { data = val; wgt = freq; }

	//返回权重
	int weight() { return wgt; }

	//返回数据
	T val() { return data; }

	//判断是否为叶节点
	bool isLeaf() { return true; }

	//左子节点（do nothing）
	HuffNode<T>* left() const { return NULL; }

	//右子节点（do nothing）
	HuffNode<T>* right() const { return NULL; }

	//设置权重
	void setWeight(int w) { wgt = w; }

	//哈夫曼编码
	string getCode() { return code; }

	//设置哈夫曼编码
	void setCode(string icode) { code = icode; }
};

//内部节点类
template<class T>
class IntlNode : public HuffNode<T>
{
private:
	HuffNode<T>* leftChild;//左节点
	HuffNode<T>* rightChild;//右节点
	int wgt;
	string code;
public:
	//构造方法
	IntlNode(HuffNode<T>* l, HuffNode<T>* r) { wgt = l->weight() + r->weight(); leftChild = l; rightChild = r; }

	//返回权重
	int weight() { return wgt; }

	//判断是否为叶节点
	bool isLeaf() { return false; }

	//返回左节点
	HuffNode<T>* left() const { return leftChild; }

	//设置左节点
	void setLeft(HuffNode<T>* b) { leftChild = (HuffNode<T>*)b; }

	//返回右节点
	HuffNode<T>* right() const { return rightChild; }

	//设置右节点
	void setRight(HuffNode<T>* b) { rightChild = (HuffNode<T>*)b; }

	//返回元素值（do nothing）
	T val() { return NULL; }

	//设置权重
	void setWeight(int w) { wgt = w; }

	//哈夫曼编码（do nothing）
	string getCode() { return NULL; }

	//设置哈夫曼编码（do nothing）
	void setCode(string icode) {}
};

//哈夫曼树类
template<class T>
class HuffTree
{
private:
	HuffNode<T>* Root;//根节点
public:
	//叶节点构造方法
	HuffTree(T val, int freq) { Root = new LeafNode<T>(val, freq); }
	//内部节点构造方法
	HuffTree(HuffTree<T>* l, HuffTree<T>* r) { Root = new IntlNode<T>(l->root(), r->root()); }
	//返回根节点
	HuffNode<T>* root() { return Root; }
	//返回跟权重
	int weight() { return Root->weight(); }
	//清空哈夫曼树
	void clear() { Root = NULL; }
};

//构建哈夫曼树
template <class T>
HuffTree<T>* buildHuff(HuffTree<T>** TreeArray, int count);

//遍历得到哈夫曼编码并存入对应节点中
template <class T>
void traverse(HuffNode<T>* node, string code);

//将哈夫曼树中的叶节点存入编码字典中
template<class T>
void treeTocodeMap(HuffNode<T>* root);