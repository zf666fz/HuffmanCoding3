#include<iostream>
#include<assert.h>
#include<queue>
#include<windows.h>
using namespace std;

//���������������±��Ԫ��
template <class T>
void Swap(T array[], int pos1, int pos2);

//����
template <class T> class heap 
{
private:
	T* Heap; //����ָ��
	int maxsize; //�������
	int n; //��ǰ����
	//�³��㷨
	void siftdown(int pos)
	{
		while (!isLeaf(pos)) //�����Ҷ�ڵ��ֹͣѭ��
		{
			int j = leftchild(pos);
			int rc = rightchild(pos);
			if ((rc < n) && (Heap[rc]->weight() < Heap[j]->weight()))j = rc; //��j��ֵΪ�ϴ���ӽڵ��ֵ
			if (Heap[pos]->weight() < Heap[j]->weight()) return; //���ڵ��ֵ���ʱ���
			Swap(Heap, pos, j);//�������ڵ���ϴ��ӽڵ��ֵ
			pos = j; //ִ����һ��
		}
	}
public:
	//���캯��
	heap(T* h, int num, int max) { Heap = h; n = num; maxsize = max; buildHeap(); }
	//���ص�ǰ�ѵĴ�С
	int size() const { return n; }
	//�ж��Ƿ�ΪҶ�ڵ�
	bool isLeaf(int pos) const { return (pos >= n / 2) && (pos < n); }
	//�������ӽڵ��λ��
	int leftchild(int pos) const{ return 2 * pos + 1; } 
	//�������ӽڵ��λ��
	int rightchild(int pos) const{ return 2 * pos + 2; } 
	//���ظ��ڵ��λ��
	int parent(int pos) const { return (pos - 1) / 2; }
	//������
	void buildHeap() { for (int i = n / 2 - 1; i >= 0; i--) siftdown(i); }
	//��data���뵽����
	void insert(const T& data) 
	{
		assert(n < maxsize);
		int curr = n++;
		Heap[curr] = data; //�Ӷѵ�ĩβ��ʼ
		// Now sift up until curr��s parent > curr
		while ((curr != 0) && (Heap[curr]->weight() < Heap[parent(curr)]->weight())) 
		{
			Swap(Heap, curr, parent(curr));
			curr = parent(curr);
		}
	}
	//�Ƴ����еĵ�һ��Ԫ��
	T removefirst() 
	{
		assert(n > 0);
		Swap(Heap, 0, --n); //������βԪ��
		if (n != 0) siftdown(0); //�³���Ԫ��
		return Heap[n]; //����ɾ����Ԫ��
	}
};

//����ڵ���
template<class T>
class HuffNode
{
public:
	virtual int weight() = 0;//Ȩ��
	virtual bool isLeaf() = 0;//�ж��Ƿ�ΪҶ�ڵ�
	virtual T val() = 0;//�洢Ԫ��ֵ
	virtual HuffNode<T>* left() const = 0;//���ӽڵ�
	virtual HuffNode<T>* right() const = 0;//���ӽڵ�
	virtual void setWeight(int w) = 0;//����Ȩ��
	virtual string getCode() = 0;//����������
	virtual void setCode(string icode) = 0;//���ù���������
};

//Ҷ�ڵ���
template<class T>
class LeafNode : public HuffNode<T>
{
private:
	T data;//����
	int wgt;//Ȩ��
	string code;
public:
	//���췽��
	LeafNode(const T& val, int freq) { data = val; wgt = freq; }

	//����Ȩ��
	int weight() { return wgt; }

	//��������
	T val() { return data; }

	//�ж��Ƿ�ΪҶ�ڵ�
	bool isLeaf() { return true; }

	//���ӽڵ㣨do nothing��
	HuffNode<T>* left() const { return NULL; }

	//���ӽڵ㣨do nothing��
	HuffNode<T>* right() const { return NULL; }

	//����Ȩ��
	void setWeight(int w) { wgt = w; }

	//����������
	string getCode() { return code; }

	//���ù���������
	void setCode(string icode) { code = icode; }
};

//�ڲ��ڵ���
template<class T>
class IntlNode : public HuffNode<T>
{
private:
	HuffNode<T>* leftChild;//��ڵ�
	HuffNode<T>* rightChild;//�ҽڵ�
	int wgt;
	string code;
public:
	//���췽��
	IntlNode(HuffNode<T>* l, HuffNode<T>* r) { wgt = l->weight() + r->weight(); leftChild = l; rightChild = r; }

	//����Ȩ��
	int weight() { return wgt; }

	//�ж��Ƿ�ΪҶ�ڵ�
	bool isLeaf() { return false; }

	//������ڵ�
	HuffNode<T>* left() const { return leftChild; }

	//������ڵ�
	void setLeft(HuffNode<T>* b) { leftChild = (HuffNode<T>*)b; }

	//�����ҽڵ�
	HuffNode<T>* right() const { return rightChild; }

	//�����ҽڵ�
	void setRight(HuffNode<T>* b) { rightChild = (HuffNode<T>*)b; }

	//����Ԫ��ֵ��do nothing��
	T val() { return NULL; }

	//����Ȩ��
	void setWeight(int w) { wgt = w; }

	//���������루do nothing��
	string getCode() { return NULL; }

	//���ù��������루do nothing��
	void setCode(string icode) {}
};

//����������
template<class T>
class HuffTree
{
private:
	HuffNode<T>* Root;//���ڵ�
public:
	//Ҷ�ڵ㹹�췽��
	HuffTree(T val, int freq) { Root = new LeafNode<T>(val, freq); }
	//�ڲ��ڵ㹹�췽��
	HuffTree(HuffTree<T>* l, HuffTree<T>* r) { Root = new IntlNode<T>(l->root(), r->root()); }
	//���ظ��ڵ�
	HuffNode<T>* root() { return Root; }
	//���ظ�Ȩ��
	int weight() { return Root->weight(); }
	//��չ�������
	void clear() { Root = NULL; }
};

//������������
template <class T>
HuffTree<T>* buildHuff(HuffTree<T>** TreeArray, int count);

//�����õ����������벢�����Ӧ�ڵ���
template <class T>
void traverse(HuffNode<T>* node, string code);

//�����������е�Ҷ�ڵ��������ֵ���
template<class T>
void treeTocodeMap(HuffNode<T>* root);