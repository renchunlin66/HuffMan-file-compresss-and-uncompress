# include<iostream>
using namespace std;
# include"HuffMan.h"
# include"filecompress.h"

class Uncompress
{
private:
	FileInfo _UNinfos[256];
	LongType Count;
public:
	Uncompress()//哈希表的初始化
	{
		for (int i = 0; i < 256; i++)
		{
			_UNinfos[i]._ch = i;
		}
		Count = 0;
	}
	bool _Uncompress(const char *Ufilename)//读配置文件
	{
		string Configfile = Ufilename;
		Configfile += ".config";
		FILE *fpConfig = fopen(Configfile.c_str(), "rb");
		assert(fpConfig);

		string line;
	    unsigned char ch = fgetc(fpConfig);
		while (ch != '\n')
		{   
			line += ch;
			ch =fgetc(fpConfig);		
		}//读取第一个字符
		Count = atoi(line.substr(0).c_str());//（总的字符个数）
		ch = fgetc(fpConfig);//读入下一行字符
		line.clear();
		int j = 0;
		while (!feof(fpConfig))
		{
			
			j++;
			while (ch != '\n')
			{
				line += ch;
				ch = fgetc(fpConfig);

			}
			if (line.empty())
			{
				line += '\n';
				ch = fgetc(fpConfig);
				while (ch != '\n')
				{
					line += ch;
					ch = fgetc(fpConfig);
				}
			}
			ch = fgetc(fpConfig);
			unsigned char tempch = line[0];//将第一个字符转换成无符号数和下标对应起来
			                               //尤其要注意这里稍微不注意就全乱码了  
			_UNinfos[tempch]._count = atoi(line.substr(2).c_str());//截取字符串并转换成整型数据
			line.clear();
		}
		return true;
	}
	void GenrateHuffManCode(HuffManNode<FileInfo>* root)//重构哈夫曼树
	{
		if (root == NULL)
		{
			return;
		}
		GenrateHuffManCode(root->_left);
		GenrateHuffManCode(root->_right);

		if ((root->_left == NULL) && (root->_right == NULL))
		{
			HuffManNode<FileInfo>*cur = root;
			HuffManNode<FileInfo>*parent = cur->_parent;
			string &code = _UNinfos[cur->_weight._ch]._code;
			while (parent)//从叶子节点走到根结点
			{
				if (parent->_left == cur)

					code += '0';
				else
					code += '1';
				cur = parent;
				parent = cur->_parent;
			}
			reverse(code.begin(), code.end());
		}
	}

	bool UncomPress(const char *UNfilename)//文件的解压缩过程
	{
		_Uncompress(UNfilename);
		HuffMan<FileInfo> Re_huffTree;
		FileInfo invalid;
		HuffManNode<FileInfo>*root = Re_huffTree.CreatTree(_UNinfos, 256, invalid);//重构哈夫曼树
		GenrateHuffManCode(root);

		//打开文件
		string UnComPressFile = UNfilename;
		UnComPressFile += ".Unhuffman";
		FILE *UCPfile = fopen(UnComPressFile.c_str(), "wb");
		string ComPressFile = UNfilename;
		ComPressFile += ".huffman";
		FILE *CPfile = fopen(ComPressFile.c_str(), "rb");

		//解压缩字符写入文件


		HuffManNode<FileInfo>*tempRoot = root;//获得其根结点
		while (!feof(CPfile))
		{
			unsigned char ch = fgetc(CPfile);
			int bitCount = 7;
			for (int i = bitCount; i >= 0; i--)
			{
				if (ch&(1 << i))
				{
					tempRoot = tempRoot->_right;
				}
				else
				{
					tempRoot = tempRoot->_left;
				}
				if (!tempRoot->_left&&!tempRoot->_right)//做到这里
				{
					fputc(tempRoot->_weight._ch, UCPfile);
					Count--;
					tempRoot = root;
				}
				if (Count <= 0)
				{
					break;
				}
			}
			if (Count <= 0)
			{
				break;
			}
		}
		return true;
	}

};
void TestUNCP()
{
	Uncompress Uncp;
	Uncp.UncomPress("fin.txt");
}