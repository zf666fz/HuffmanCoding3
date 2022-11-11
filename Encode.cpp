#include"HuffTree.cpp"
//#include"FileIO.h"
extern HuffTree<byte>* tree;//��������
extern std::map<byte, std::string>codeMap;//�洢�������������ֵ�
extern std::map<byte, int>numberMap;//�洢�����ļ��и��ַ������ֵ�
extern string in_file_name;//�����ļ�������

//���ô洢�ַ������ֵ佨����������
void numberMapToTree()
{
	const int mapSize = numberMap.size();//�����ֵ��еļ�ֵ�Ը���
	HuffTree<byte>** NodeArray = (HuffTree<byte>**)malloc(sizeof(HuffTree<byte>*) * mapSize);//����洢����������������
	int i = 0;
	for (auto iter = numberMap.begin(); iter != numberMap.end() && i < mapSize; ++iter, ++i)//������������������
	{
		NodeArray[i] = new HuffTree<byte>(iter->first, iter->second);
	}
	tree = buildHuff(NodeArray, mapSize);//������������
	string str0 = "";//���Թ�������
	traverse(tree->root(), str0);
}

//��λֻ��0��1���ַ���תint
int StringtoInt(string str)
{
	int i = 0;
	for (int t = 7, temp = 128; t >= 0; t--, temp /= 2)
	{
		if (str.at(7 - t) == '1')i += temp;
	}
	return i;
}

//���ļ���ȡ����������
string getCodeFromFile(ifstream& in_file)
{
	
	char c;
	string outString = "";
	while (in_file.peek() != EOF)//���ֽڶ��������ļ�
	{
		in_file.read((char*)&c, 1);
		byte bt = (byte)c;
		for (auto iter = codeMap.begin(); iter != codeMap.end(); ++iter)
		{
			if (iter->first == bt) //���ֵ����е�ǰ�ַ��ļ����ַ������ϸ��ַ��ı���
			{
				outString += iter->second;
			}
		}
	}
	
	return outString;
}

//д������
void writeCodeList(ofstream &out_file)
{
	int length = numberMap.size();//�������
	out_file.write((char*)&length, sizeof(length));//1.д��������
	for (auto iter = numberMap.begin(); iter != numberMap.end(); ++iter)
	{
		byte codeChar = iter->first;//�����ַ�
		out_file.write((char*)&codeChar, sizeof(codeChar));//2.д������ַ�
		int number = iter->second;//�ַ����ִ���
		out_file.write((char*)&number, sizeof(number));//3.д���ַ����ִ���
	}
}

//д�����������
void writeHuffCode(ofstream& out_file,string outString)
{
	string outPut = "";
	int stringLength = outString.size() / 8 + 1;//�ַ�������
	out_file.write((char*)&stringLength, sizeof(stringLength));//4.д���ַ�������
	int tail = 8 - outString.size() % 8;//ĩβδ��'0'�ĳ���
	out_file.write((char*)&tail, sizeof(tail));//5.д�����һ���ֽ���Чλ��
	for (int i = 0; i < outString.size() / 8; i++)
	{
		string codeString(outString, i * 8, 8);
		char ch = StringtoInt(codeString);
		out_file.write((char*)&ch, sizeof(ch));//6.�������������Զ�������ʽд���ļ�
	}
	string tailString(outString, outString.size() / 8 * 8, outString.size() % 8);//ĩβ�ַ���
	for (int i = 0; i < 8 - outString.size() % 8; i++)//��ĩβ�ַ�����'0'
	{
		tailString += '0';
	}
	char tailChar = (char)StringtoInt(tailString);//ĩβ�ַ�
	out_file.write((char*)&tailChar, sizeof(tailChar));//7.д��ĩβ�ַ�
}

//���ȫ�ֱ���
void encodeClearAll()
{
	tree->clear();
	numberMap.clear();
	codeMap.clear();
	in_file_name = "";
}
//д�����ļ�
void writeEncodeFile()
{
	
	ifstream in_file(in_file_name, ios::in | ios::binary);//�Զ�������ʽ���ļ�
	string outString = getCodeFromFile(in_file);//���ļ���ȡ����������
	in_file.close();//�ر������ļ���

	MessageBox(GetForegroundWindow(), (LPCWSTR)TEXT("�봴��һ���ļ���ѡ��һ���ǿ��ļ���Ϊѹ�����ļ�"), (LPCWSTR)TEXT("��ʾ"), MB_OK);
	string encoderFileName = get_path();
	while (encoderFileName == "")
	{
		MessageBox(GetForegroundWindow(), (LPCWSTR)TEXT("�봴��һ���ļ���ѡ��һ���ǿ��ļ���"), (LPCWSTR)TEXT("����"), MB_OK);
		encoderFileName = get_path();
	}
	ofstream out_file(/*"Encoder.txt"*/encoderFileName, ios::out | ios::binary);//�Զ�������ʽд�ļ�	
	writeCodeList(out_file);//д������
	writeHuffCode(out_file, outString);//д�����������
	out_file.close();//�ر�����ļ���
	
	encodeClearAll();//���ȫ�ֱ���
}
