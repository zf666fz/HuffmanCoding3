//#include"HuffTree.cpp"
//#include"FileIO.h"


using namespace std;

//利用存储字符数的字典建立哈夫曼树
void numberMapToTree();

//写编码文件
void writeEncodeFile();

//八位只含0和1的字符串转int
int StringtoInt(string str);

//从文件获取哈夫曼编码
string getCodeFromFile();

//写入编码表
void writeCodeList(ofstream& out_file);

//写入哈夫曼编码
void writeHuffCode(ofstream& out_file, string outString);

//写编码文件
void writeEncodeFile();
