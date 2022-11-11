#include<windows.h>
#include<iostream>
#include<string>
#include<fstream>
#include<map>
using namespace std;

extern std::map<byte, int>numberMap;//�洢�����ļ��и��ַ������ֵ�
extern string in_file_name;//�����ļ�������
extern std::map<byte, std::string>codeMap;//�洢�������������ֵ�

//��һ��LPCWSTRת��Ϊstring
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

//�½�һ���Ի����ڣ�ѡ���ļ�
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

//���ļ������ļ����ַ����洢�ڴ����ֵ���
void fileByteNumber()
{
	//numberMap.clear();
	string s = "";
	s = get_path();
	ifstream in_file(s, ios::in | ios::binary);//�Զ�������ʽ���ļ�
	char c;
	
	if (in_file.peek() == EOF)//δѡ���ļ���ѡ����ļ�
	{
		MessageBox(GetForegroundWindow(), (LPCWSTR)TEXT("��ѡ��һ���ǿ��ļ���"), (LPCWSTR)TEXT("����"), MB_OK);
		fileByteNumber();
		return;
	}
	else
	{
		in_file_name = s;//��¼�����ļ�������
	}
	while (in_file.peek() != EOF)//���ֽڶ��������ļ�
	{
		in_file.read((char*)&c, sizeof(c));
		byte bt = (byte)c;
		bool have = false;//��¼�ֵ����Ƿ���ĳһ�ַ���ֵ�Եı���
		for (auto iter = numberMap.begin(); iter != numberMap.end(); ++iter) 
		{
			if (iter->first == bt) //���ֵ�����ĳһ�ַ��ļ�ֵ������ִ�����һ�������have
			{
				iter->second++;
				have = true;
			}
		}
		if (!have)//���ֵ���û�и��ַ���ֵ���򴴽�
		{
			numberMap[bt] = 1;
		}
	}
	//for (auto iter = numberMap.begin(); iter != numberMap.end(); ++iter)//����numberMap
	//{
	//	cout << iter->first << " " << iter->second << endl;
	//}
	in_file.close();//�ر��ļ���
}

