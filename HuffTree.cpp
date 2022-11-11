#include"HuffTree.h"
#include"FileIO.h"
using namespace std;

extern std::map<byte, std::string>codeMap;//�洢�������������ֵ�

//���������������±��Ԫ��
template <class T>
void Swap(T array[], int pos1, int pos2)
{
	T temp = array[pos1];
	array[pos1] = array[pos2];
	array[pos2] = temp;
}

//������������
template <class T>
HuffTree<T>* buildHuff(HuffTree<T>** TreeArray, int count) 
{
	heap<HuffTree<T>*>* forest = new heap<HuffTree<T>*>(TreeArray, count, count);
	HuffTree<T>* temp1, * temp2, * temp3 = NULL;
	while (forest->size() > 1) 
	{
		temp1 = forest->removefirst(); //����ǰ���������Ƴ�ɭ��
		temp2 = forest->removefirst(); 
		temp3 = new HuffTree<T>(temp1, temp2);
		forest->insert(temp3); //����������������ɭ��
		delete temp1; //ɾ���������
		delete temp2; 
	}
	return temp3;
}


//�����õ����������벢�����Ӧ�ڵ���
template <class T>
void traverse(HuffNode<T>* node, string code)
{
	if (node->isLeaf())
	{
		node->setCode(code);
	}
	else
	{
		traverse(node->left(), code + '0');//�����߱���Ϊ0
		traverse(node->right(), code + '1');//�����߱���Ϊ1
	}
}


//�����������е�Ҷ�ڵ��������ֵ���
template<class T>
void treeTocodeMap(HuffNode<T>* root)
{
	
	if (root == NULL)	return;//���򷵻�
	if (root->isLeaf())//Ҷ�ڵ��������ֵ���
	{
		codeMap[root->val()] = root->getCode();
		return;
	}
	treeTocodeMap(root->left());//�������ݹ�
	treeTocodeMap(root->right());//�������ݹ�
}