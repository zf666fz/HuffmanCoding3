#include"main.h"
using namespace std;



HuffTree<byte>* tree;//哈夫曼树
std::map<byte, int>numberMap;//存储输入文件中个字符数的字典
std::map<byte, std::string>codeMap;//存储哈夫曼编码表的字典
string in_file_name;//输入文件的名字

void menu()
{
	cout << "0.退出" << endl;
	cout << "1.压缩" << endl;
	cout << "2.解压" << endl;	
}
int main()
{
	menu();
	char choice;
	cin >> choice;
	while (choice!= '0')
	{
		switch (choice)
		{
		case '0':
			return 0;
		case '1':
			MessageBox(GetForegroundWindow(), (LPCWSTR)TEXT("请选择一个非空文件作为待压缩文件"), (LPCWSTR)TEXT("提示"), MB_OK);
			fileByteNumber();//读文件并将文件各字符数存储在次数字典中
			numberMapToTree();//利用存储字符数的字典建立哈夫曼树
			treeTocodeMap(tree->root());//将哈夫曼树中的叶节点存入编码字典中
			writeEncodeFile();//写编码文件
			break;
		case '2':
			writeDecodeFile();//写解码文件
			break;
		default:
			cout << "输入错误，请重新输入！" << endl;
			break;
		}
		menu();
		cin >> choice;
	}
	return 0;
}