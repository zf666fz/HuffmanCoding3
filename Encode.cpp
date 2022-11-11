#include"HuffTree.cpp"
//#include"FileIO.h"
extern HuffTree<byte>* tree;//哈夫曼树
extern std::map<byte, std::string>codeMap;//存储哈夫曼编码表的字典
extern std::map<byte, int>numberMap;//存储输入文件中个字符数的字典
extern string in_file_name;//输入文件的名字

//利用存储字符数的字典建立哈夫曼树
void numberMapToTree()
{
	const int mapSize = numberMap.size();//次数字典中的键值对个数
	HuffTree<byte>** NodeArray = (HuffTree<byte>**)malloc(sizeof(HuffTree<byte>*) * mapSize);//定义存储哈夫曼子树的数组
	int i = 0;
	for (auto iter = numberMap.begin(); iter != numberMap.end() && i < mapSize; ++iter, ++i)//创建哈夫曼树的子树
	{
		NodeArray[i] = new HuffTree<byte>(iter->first, iter->second);
	}
	tree = buildHuff(NodeArray, mapSize);//建立哈夫曼树
	string str0 = "";//测试哈夫曼树
	traverse(tree->root(), str0);
}

//八位只含0和1的字符串转int
int StringtoInt(string str)
{
	int i = 0;
	for (int t = 7, temp = 128; t >= 0; t--, temp /= 2)
	{
		if (str.at(7 - t) == '1')i += temp;
	}
	return i;
}

//从文件获取哈夫曼编码
string getCodeFromFile(ifstream& in_file)
{
	
	char c;
	string outString = "";
	while (in_file.peek() != EOF)//按字节读二进制文件
	{
		in_file.read((char*)&c, 1);
		byte bt = (byte)c;
		for (auto iter = codeMap.begin(); iter != codeMap.end(); ++iter)
		{
			if (iter->first == bt) //若字典中有当前字符的键则字符串加上该字符的编码
			{
				outString += iter->second;
			}
		}
	}
	
	return outString;
}

//写入编码表
void writeCodeList(ofstream &out_file)
{
	int length = numberMap.size();//编码表长度
	out_file.write((char*)&length, sizeof(length));//1.写入编码表长度
	for (auto iter = numberMap.begin(); iter != numberMap.end(); ++iter)
	{
		byte codeChar = iter->first;//编码字符
		out_file.write((char*)&codeChar, sizeof(codeChar));//2.写入编码字符
		int number = iter->second;//字符出现次数
		out_file.write((char*)&number, sizeof(number));//3.写入字符出现次数
	}
}

//写入哈夫曼编码
void writeHuffCode(ofstream& out_file,string outString)
{
	string outPut = "";
	int stringLength = outString.size() / 8 + 1;//字符串长度
	out_file.write((char*)&stringLength, sizeof(stringLength));//4.写入字符串长度
	int tail = 8 - outString.size() % 8;//末尾未补'0'的长度
	out_file.write((char*)&tail, sizeof(tail));//5.写入最后一个字节无效位数
	for (int i = 0; i < outString.size() / 8; i++)
	{
		string codeString(outString, i * 8, 8);
		char ch = StringtoInt(codeString);
		out_file.write((char*)&ch, sizeof(ch));//6.将哈夫曼编码以二进制形式写入文件
	}
	string tailString(outString, outString.size() / 8 * 8, outString.size() % 8);//末尾字符串
	for (int i = 0; i < 8 - outString.size() % 8; i++)//给末尾字符串补'0'
	{
		tailString += '0';
	}
	char tailChar = (char)StringtoInt(tailString);//末尾字符
	out_file.write((char*)&tailChar, sizeof(tailChar));//7.写入末尾字符
}

//清空全局变量
void encodeClearAll()
{
	tree->clear();
	numberMap.clear();
	codeMap.clear();
	in_file_name = "";
}
//写编码文件
void writeEncodeFile()
{
	
	ifstream in_file(in_file_name, ios::in | ios::binary);//以二进制形式读文件
	string outString = getCodeFromFile(in_file);//从文件获取哈夫曼编码
	in_file.close();//关闭输入文件流

	MessageBox(GetForegroundWindow(), (LPCWSTR)TEXT("请创建一个文件或选择一个非空文件作为压缩后文件"), (LPCWSTR)TEXT("提示"), MB_OK);
	string encoderFileName = get_path();
	while (encoderFileName == "")
	{
		MessageBox(GetForegroundWindow(), (LPCWSTR)TEXT("请创建一个文件或选择一个非空文件！"), (LPCWSTR)TEXT("错误"), MB_OK);
		encoderFileName = get_path();
	}
	ofstream out_file(/*"Encoder.txt"*/encoderFileName, ios::out | ios::binary);//以二进制形式写文件	
	writeCodeList(out_file);//写入编码表
	writeHuffCode(out_file, outString);//写入哈夫曼编码
	out_file.close();//关闭输出文件流
	
	encodeClearAll();//清空全局变量
}
