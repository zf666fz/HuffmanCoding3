#include"HuffTree.h"
#include"FileIO.h"
using namespace std;

extern std::map<byte, std::string>codeMap;//存储哈夫曼编码表的字典

//交换数组中两个下标的元素
template <class T>
void Swap(T array[], int pos1, int pos2)
{
	T temp = array[pos1];
	array[pos1] = array[pos2];
	array[pos2] = temp;
}

//构建哈夫曼树
template <class T>
HuffTree<T>* buildHuff(HuffTree<T>** TreeArray, int count) 
{
	heap<HuffTree<T>*>* forest = new heap<HuffTree<T>*>(TreeArray, count, count);
	HuffTree<T>* temp1, * temp2, * temp3 = NULL;
	while (forest->size() > 1) 
	{
		temp1 = forest->removefirst(); //将最前面两棵树移出森林
		temp2 = forest->removefirst(); 
		temp3 = new HuffTree<T>(temp1, temp2);
		forest->insert(temp3); //将构建的新树移入森林
		delete temp1; //删除残余的树
		delete temp2; 
	}
	return temp3;
}


//遍历得到哈夫曼编码并存入对应节点中
template <class T>
void traverse(HuffNode<T>* node, string code)
{
	if (node->isLeaf())
	{
		node->setCode(code);
	}
	else
	{
		traverse(node->left(), code + '0');//往左走编码为0
		traverse(node->right(), code + '1');//往右走编码为1
	}
}


//将哈夫曼树中的叶节点存入编码字典中
template<class T>
void treeTocodeMap(HuffNode<T>* root)
{
	
	if (root == NULL)	return;//空则返回
	if (root->isLeaf())//叶节点存入编码字典中
	{
		codeMap[root->val()] = root->getCode();
		return;
	}
	treeTocodeMap(root->left());//左子树递归
	treeTocodeMap(root->right());//右子树递归
}