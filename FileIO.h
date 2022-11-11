#include<windows.h>
#include<iostream>
#include<string>
#include<fstream>
#include<map>

//将一个LPCWSTR转换为string
string Lpcwstr2String(LPCWSTR lps);

//新建一个对话窗口，选择文件
string get_path();

//读文件并将文件各字符数存储在次数字典中
void fileByteNumber();
