#include<windows.h>
#include<iostream>
#include<string>
#include<fstream>
#include<map>
using namespace std;

extern std::map<byte, int>numberMap;//存储输入文件中个字符数的字典
extern string in_file_name;//输入文件的名字
extern std::map<byte, std::string>codeMap;//存储哈夫曼编码表的字典

//将一个LPCWSTR转换为string
string Lpcwstr2String(LPCWSTR lps) 
{
	int len = WideCharToMultiByte(CP_ACP, 0, lps, -1, NULL, 0, NULL, NULL);
	if (len <= 0) {
		return "";
	}
	else {
		char* dest = new char[len];
		WideCharToMultiByte(CP_ACP, 0, lps, -1, dest, len, NULL, NULL);
		dest[len - 1] = 0;
		string str(dest);
		delete[] dest;
		return str;
	}
}

//新建一个对话窗口，选择文件
string get_path() 
{
	OPENFILENAME ofn;
	char szFile[300];

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = (LPWSTR)szFile;
	ofn.lpstrFile[0] = '\0';
	LPTSTR        lpstrCustomFilter;
	DWORD         nMaxCustFilter;
	ofn.nFilterIndex = 1;
	LPTSTR        lpstrFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"ALL\0*.*\0Text\0*.TXT\0";
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;

	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	string path_image = "";
	if (GetOpenFileName(&ofn)) {
		path_image = Lpcwstr2String(ofn.lpstrFile);
		return path_image;
	}
	else {
		return "";
	}
}

//读文件并将文件各字符数存储在次数字典中
void fileByteNumber()
{
	//numberMap.clear();
	string s = "";
	s = get_path();
	ifstream in_file(s, ios::in | ios::binary);//以二进制形式读文件
	char c;
	
	if (in_file.peek() == EOF)//未选择文件或选择空文件
	{
		MessageBox(GetForegroundWindow(), (LPCWSTR)TEXT("请选择一个非空文件！"), (LPCWSTR)TEXT("错误"), MB_OK);
		fileByteNumber();
		return;
	}
	else
	{
		in_file_name = s;//记录输入文件的名字
	}
	while (in_file.peek() != EOF)//按字节读二进制文件
	{
		in_file.read((char*)&c, sizeof(c));
		byte bt = (byte)c;
		bool have = false;//记录字典中是否有某一字符键值对的变量
		for (auto iter = numberMap.begin(); iter != numberMap.end(); ++iter) 
		{
			if (iter->first == bt) //若字典中有某一字符的键值对则出现次数加一，并标记have
			{
				iter->second++;
				have = true;
			}
		}
		if (!have)//若字典中没有该字符键值对则创建
		{
			numberMap[bt] = 1;
		}
	}
	//for (auto iter = numberMap.begin(); iter != numberMap.end(); ++iter)//测试numberMap
	//{
	//	cout << iter->first << " " << iter->second << endl;
	//}
	in_file.close();//关闭文件流
}

