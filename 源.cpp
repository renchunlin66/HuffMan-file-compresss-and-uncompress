#define _CRT_SECURE_NO_WARNINGS

# include<iostream>
# include<cassert>
using namespace std;
//# include"filecompress.h"
# include"Uncompress.h"

int main()
{
 //FileCompress FC;
// FC.CompressFile("fin.txt");
 //cout << "Ñ¹Ëõ³É¹¦" << endl;
	Uncompress Uncp;
    Uncp.UncomPress("fin.txt");

}