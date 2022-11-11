#include"HuffTree.cpp"

std::map<byte, std::string>decodeMap;//存储哈夫曼编码表的字典
HuffTree<byte>* decodeTree;//哈夫曼树
std::map<byte, int>decodeNumberMap;//存储输入文件中个字符数的字典

//将int转为八位二进制字符串
string InttoString(int in)
{
	string str = "";
	int temp = 128;
	for (int temp = 128; temp > 0; temp /= 2)
	{
		if (in >= temp)
		{
			str += '1';
			in -= temp;
		}
		else
		{
			str += '0';
		}
	}
	return str;
}

//根据decodeNumberMap建立哈夫曼树
void decodeNumberMapToTree()
{
	const int mapSize = decodeNumberMap.size();//次数字典中的键值对个数
	HuffTree<byte>** NodeArray = (HuffTree<byte>**)malloc(sizeof(HuffTree<byte>*) * mapSize);//定义存储哈夫曼子树的数组
	int i = 0;
	for (auto iter = decodeNumberMap.begin(); iter != decodeNumberMap.end() && i < mapSize; ++iter, ++i)//创建哈夫曼树的子树
	{
		NodeArray[i] = new HuffTree<byte>(iter->first, iter->second);
	}
	decodeTree = buildHuff(NodeArray, mapSize);//建立哈夫曼树
	string str1 = "";
	traverse(decodeTree->root(), str1);
}

//根据哈夫曼树建立编码表字典
template<class T>
void treeTodecodeMap(HuffNode<T>* root)
{
	if (root == NULL)  return; //空则返回
	if (root->isLeaf())//叶节点存入编码字典中
	{
		decodeMap[root->val()] = root->getCode();
		
		return;
	}
	treeTodecodeMap(root->left());//左子树递归
	treeTodecodeMap(root->right());//右子树递归
}

//读文件，先建立字符次数字典，再得到哈夫曼树，最后建立编码表字典
void readFileToMap(ifstream& in_file)
{
	int length;//编码表长度
	in_file.read((char*)&length, sizeof(length));//1.读入编码表长度
	for (int i = 0; i < length; i++)
	{
		byte codeChar;//编码字符
		in_file.read((char*)&codeChar, sizeof(codeChar));//2.读入编码字符
		int number;//字符出现次数
		in_file.read((char*)&number, sizeof(number));//3.读入字符出现次数
		decodeNumberMap[codeChar] = number;//写入字典
	}
	decodeNumberMapToTree();//建立哈夫曼树
	treeTodecodeMap(decodeTree->root());//建立编码表字典
}

//读文件并返回二进制字符串
string readFileToString(ifstream& in_file)
{
	int stringLength;//字符串长度
	in_file.read((char*)&stringLength, sizeof(stringLength));//4.写入字符串长度
	int tail;//末尾未补'0'的长度
	in_file.read((char*)&tail, sizeof(tail));
	string codeString = "";
	/*char c;
	in_file.read((char*)&c, stringLength);*/
	for (int i = 0; i < stringLength; i++)
	{
		char chh;
		in_file.read((char*)&chh, sizeof(chh));
		int c1 = chh;
		string str = InttoString(c1 < 0 ? (c1 + 256) : c1);
		codeString += str;
	}
	string code(codeString, 0, codeString.size() - tail);
	return code;
}

//
void writeStringToFile(ofstream& out_file, string code)
{
	for (int pos = 0; pos < code.size(); )
	{
		bool done = false;
		for (int length = 0; !done; length++)
		{
			string temp(code, pos, length);
			for (auto iter = decodeMap.begin(); iter != decodeMap.end() && !done; ++iter)
			{
				if (temp == iter->second)
				{
					char ch = (char)iter->first;
					out_file.write((char*)&ch, sizeof(ch));
					pos += length;
					done = true;
				}
			}
		}
	}
}
//清空全局变量
void decoderClearAll()
{
	decodeMap.clear();
	decodeNumberMap.clear();
	decodeTree->clear();
}

//写解码文件
void writeDecodeFile()
{
	MessageBox(GetForegroundWindow(), (LPCWSTR)TEXT("请创建一个文件或选择一个非空文件作为待解压文件"), (LPCWSTR)TEXT("提示"), MB_OK);
	string encoderFileName = get_path();
	while (encoderFileName == "")
	{
		MessageBox(GetForegroundWindow(), (LPCWSTR)TEXT("请创建一个文件或选择一个非空文件！"), (LPCWSTR)TEXT("错误"), MB_OK);
		encoderFileName = get_path();
	}
	ifstream in_file(/*"Encoder.txt"*/encoderFileName, ios::in | ios::binary);//二进制打开文件
	readFileToMap(in_file);//读文件，先建立字符次数字典，再得到哈夫曼树，最后建立编码表字典
	string code = readFileToString(in_file);//读文件并返回二进制字符串
	in_file.close();//关闭文件输入流

	MessageBox(GetForegroundWindow(), (LPCWSTR)TEXT("请创建一个文件或选择一个非空文件作为解压后文件"), (LPCWSTR)TEXT("提示"), MB_OK);
	string decoderFileName = get_path();
	while (decoderFileName == "")
	{
		MessageBox(GetForegroundWindow(), (LPCWSTR)TEXT("请创建一个文件或选择一个非空文件！"), (LPCWSTR)TEXT("错误"), MB_OK);
		decoderFileName = get_path();
	}
	ofstream out_file(/*"Decoder.txt"*/decoderFileName, ios::out | ios::binary);//二进制打开文件
	writeStringToFile(out_file, code);
	out_file.close();//关闭文件输出流
	
	decoderClearAll();//清空全局变量
}