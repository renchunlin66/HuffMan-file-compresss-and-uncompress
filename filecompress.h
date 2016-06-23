# include<iostream>
# include<cassert>
# include<string>
# include<algorithm>
# include"HuffMan.h"
using namespace std;
typedef unsigned long long LongType;
struct FileInfo
{
  unsigned	char _ch;
  LongType  _count;
  string  _code;
  FileInfo(unsigned char ch=0)
	  :_ch(ch)
	  , _count(0)
  {}
 FileInfo operator+(FileInfo filein)
  {
	 FileInfo temp;
	 temp._count=_count + filein._count;
	 return temp;
  }
 bool operator<( const FileInfo filein)const               
 {
	 return _count < filein._count;
 }
 bool operator!=(const FileInfo  Invalid)const
 {
	 return _count != Invalid._count;
 }
};
class FileCompress
{
protected:
	FileInfo _Infos[256];
	LongType COUNT = 0;
public:
	FileCompress()
	{
		for (int i = 0; i < 256;i++)
		{
			_Infos[i]._ch = i;
		}
	}
	bool Compress(const char *filename)//该函数起到统计的作用
	{
		FILE *fout = fopen(filename, "rb");//以二进制形式打开文件
		assert(fout);
		unsigned char ch = fgetc(fout);
		while (!feof(fout))
		{
			_Infos[ch]._count++;//统计各种字符在文件中的个数
			ch = fgetc(fout);
			COUNT++;//统计文件中总的字符个数
		}
		fclose(fout);
		return true;
	}
	void GenerateHuffManCode()
	{
		HuffMan<FileInfo> t;
		FileInfo invalid;
		t.CreatTree(_Infos, 256, invalid);
		HuffManNode<FileInfo>*root = t.GetRoot();
		_GenrateHuffManCode(root);
	}
	void _GenrateHuffManCode(HuffManNode<FileInfo>* root)
	{
		if (root == NULL)
		{
			return;
		}
		_GenrateHuffManCode(root->_left);
		_GenrateHuffManCode(root->_right);

		if ((root->_left == NULL) && (root->_right == NULL))
		{
			HuffManNode<FileInfo>*cur = root;
			HuffManNode<FileInfo>*parent = cur->_parent;
			string &code = _Infos[cur->_weight._ch]._code;
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

	//下面进行文件压缩
	void CompressFile(const char *filename)
	{
		Compress(filename);
		string compressFile = filename;
		compressFile += ".huffman";
		FILE *FinCompress = fopen(compressFile.c_str(), "wb");
		assert(FinCompress);//对压缩文件的命名处理
		
		GenerateHuffManCode();//产生编码
		FILE *fout = fopen(filename, "rb");
		assert(fout);

		//进行文件压缩
		 unsigned char inch = 0;
		int index = 0;
		char ch = fgetc(fout);
		while (ch!=EOF)
		{
			string&code = _Infos[(unsigned char)ch]._code;
			for (int i = 0; i < code.size(); ++i)
			{
				++index;
				inch <<= 1;
				if (code[i] == '1')
				{
					inch |= 1;
				}
				if (index == 8)
				{
					fputc(inch, FinCompress);
					index = 0;
					inch = 0;
				}		
			}
			ch = fgetc(fout);
		}
		if (index != 0)
		{
			inch <<= (8 - index);
			fputc(inch,FinCompress);
		}
		fclose(fout);
		FileInfo invalid;
		CreateConfig(filename,invalid);
	}
	void CreateConfig( const char* filename,FileInfo invalid)
	{
		string ConfigFile = filename;
		ConfigFile += ".config";
		FILE *FileConfig = fopen(ConfigFile.c_str(), "wb");
		assert(FileConfig);

		char ch[256];
		string tempcount;
		int i = 0;
		tempcount=	_itoa(COUNT, ch, 10);
		while (i < tempcount.size())
		{
			fputc(tempcount[i],FileConfig);
			i++;
		}//将总的字符数写入配置文件
		fputc('\n', FileConfig);
		for (size_t i = 0; i < 256; i++)
		{
			if (_Infos[i] != invalid)
			{
				string chInfo;
				chInfo.clear();

				if (_Infos[i]._count>0)
				{
					chInfo += _Infos[i]._ch;
					chInfo += ',';
					char ch[256]; //转换成的字符可能足够长
					_itoa(_Infos[i]._count,ch, 10);
					chInfo += ch;
					for (int j = 0; j < chInfo.size(); j++)
					{
						fputc(chInfo[j], FileConfig);
					}
								
						fputc('\n', FileConfig);					
				}
			}
		}
		fclose(FileConfig);
	}

};
void TestFileCompress()
{
	FileCompress FC;
	FC.CompressFile("fin.txt");
	cout << "压缩成功" << endl;
}
