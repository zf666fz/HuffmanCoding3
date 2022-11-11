#include"HuffTree.cpp"

std::map<byte, std::string>decodeMap;//�洢�������������ֵ�
HuffTree<byte>* decodeTree;//��������
std::map<byte, int>decodeNumberMap;//�洢�����ļ��и��ַ������ֵ�

//��intתΪ��λ�������ַ���
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

//����decodeNumberMap������������
void decodeNumberMapToTree()
{
	const int mapSize = decodeNumberMap.size();//�����ֵ��еļ�ֵ�Ը���
	HuffTree<byte>** NodeArray = (HuffTree<byte>**)malloc(sizeof(HuffTree<byte>*) * mapSize);//����洢����������������
	int i = 0;
	for (auto iter = decodeNumberMap.begin(); iter != decodeNumberMap.end() && i < mapSize; ++iter, ++i)//������������������
	{
		NodeArray[i] = new HuffTree<byte>(iter->first, iter->second);
	}
	decodeTree = buildHuff(NodeArray, mapSize);//������������
	string str1 = "";
	traverse(decodeTree->root(), str1);
}

//���ݹ�����������������ֵ�
template<class T>
void treeTodecodeMap(HuffNode<T>* root)
{
	if (root == NULL)  return; //���򷵻�
	if (root->isLeaf())//Ҷ�ڵ��������ֵ���
	{
		decodeMap[root->val()] = root->getCode();
		
		return;
	}
	treeTodecodeMap(root->left());//�������ݹ�
	treeTodecodeMap(root->right());//�������ݹ�
}

//���ļ����Ƚ����ַ������ֵ䣬�ٵõ����������������������ֵ�
void readFileToMap(ifstream& in_file)
{
	int length;//�������
	in_file.read((char*)&length, sizeof(length));//1.����������
	for (int i = 0; i < length; i++)
	{
		byte codeChar;//�����ַ�
		in_file.read((char*)&codeChar, sizeof(codeChar));//2.��������ַ�
		int number;//�ַ����ִ���
		in_file.read((char*)&number, sizeof(number));//3.�����ַ����ִ���
		decodeNumberMap[codeChar] = number;//д���ֵ�
	}
	decodeNumberMapToTree();//������������
	treeTodecodeMap(decodeTree->root());//����������ֵ�
}

//���ļ������ض������ַ���
string readFileToString(ifstream& in_file)
{
	int stringLength;//�ַ�������
	in_file.read((char*)&stringLength, sizeof(stringLength));//4.д���ַ�������
	int tail;//ĩβδ��'0'�ĳ���
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
//���ȫ�ֱ���
void decoderClearAll()
{
	decodeMap.clear();
	decodeNumberMap.clear();
	decodeTree->clear();
}

//д�����ļ�
void writeDecodeFile()
{
	MessageBox(GetForegroundWindow(), (LPCWSTR)TEXT("�봴��һ���ļ���ѡ��һ���ǿ��ļ���Ϊ����ѹ�ļ�"), (LPCWSTR)TEXT("��ʾ"), MB_OK);
	string encoderFileName = get_path();
	while (encoderFileName == "")
	{
		MessageBox(GetForegroundWindow(), (LPCWSTR)TEXT("�봴��һ���ļ���ѡ��һ���ǿ��ļ���"), (LPCWSTR)TEXT("����"), MB_OK);
		encoderFileName = get_path();
	}
	ifstream in_file(/*"Encoder.txt"*/encoderFileName, ios::in | ios::binary);//�����ƴ��ļ�
	readFileToMap(in_file);//���ļ����Ƚ����ַ������ֵ䣬�ٵõ����������������������ֵ�
	string code = readFileToString(in_file);//���ļ������ض������ַ���
	in_file.close();//�ر��ļ�������

	MessageBox(GetForegroundWindow(), (LPCWSTR)TEXT("�봴��һ���ļ���ѡ��һ���ǿ��ļ���Ϊ��ѹ���ļ�"), (LPCWSTR)TEXT("��ʾ"), MB_OK);
	string decoderFileName = get_path();
	while (decoderFileName == "")
	{
		MessageBox(GetForegroundWindow(), (LPCWSTR)TEXT("�봴��һ���ļ���ѡ��һ���ǿ��ļ���"), (LPCWSTR)TEXT("����"), MB_OK);
		decoderFileName = get_path();
	}
	ofstream out_file(/*"Decoder.txt"*/decoderFileName, ios::out | ios::binary);//�����ƴ��ļ�
	writeStringToFile(out_file, code);
	out_file.close();//�ر��ļ������
	
	decoderClearAll();//���ȫ�ֱ���
}