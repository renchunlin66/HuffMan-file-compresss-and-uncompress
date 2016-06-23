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
	bool Compress(const char *filename)//�ú�����ͳ�Ƶ�����
	{
		FILE *fout = fopen(filename, "rb");//�Զ�������ʽ���ļ�
		assert(fout);
		unsigned char ch = fgetc(fout);
		while (!feof(fout))
		{
			_Infos[ch]._count++;//ͳ�Ƹ����ַ����ļ��еĸ���
			ch = fgetc(fout);
			COUNT++;//ͳ���ļ����ܵ��ַ�����
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
			while (parent)//��Ҷ�ӽڵ��ߵ������
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

	//��������ļ�ѹ��
	void CompressFile(const char *filename)
	{
		Compress(filename);
		string compressFile = filename;
		compressFile += ".huffman";
		FILE *FinCompress = fopen(compressFile.c_str(), "wb");
		assert(FinCompress);//��ѹ���ļ�����������
		
		GenerateHuffManCode();//��������
		FILE *fout = fopen(filename, "rb");
		assert(fout);

		//�����ļ�ѹ��
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
		}//���ܵ��ַ���д�������ļ�
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
					char ch[256]; //ת���ɵ��ַ������㹻��
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
	cout << "ѹ���ɹ�" << endl;
}
