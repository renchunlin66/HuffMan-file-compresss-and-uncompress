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
	Uncompress()//��ϣ��ĳ�ʼ��
	{
		for (int i = 0; i < 256; i++)
		{
			_UNinfos[i]._ch = i;
		}
		Count = 0;
	}
	bool _Uncompress(const char *Ufilename)//�������ļ�
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
		}//��ȡ��һ���ַ�
		Count = atoi(line.substr(0).c_str());//���ܵ��ַ�������
		ch = fgetc(fpConfig);//������һ���ַ�
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
			unsigned char tempch = line[0];//����һ���ַ�ת�����޷��������±��Ӧ����
			                               //����Ҫע��������΢��ע���ȫ������  
			_UNinfos[tempch]._count = atoi(line.substr(2).c_str());//��ȡ�ַ�����ת������������
			line.clear();
		}
		return true;
	}
	void GenrateHuffManCode(HuffManNode<FileInfo>* root)//�ع���������
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

	bool UncomPress(const char *UNfilename)//�ļ��Ľ�ѹ������
	{
		_Uncompress(UNfilename);
		HuffMan<FileInfo> Re_huffTree;
		FileInfo invalid;
		HuffManNode<FileInfo>*root = Re_huffTree.CreatTree(_UNinfos, 256, invalid);//�ع���������
		GenrateHuffManCode(root);

		//���ļ�
		string UnComPressFile = UNfilename;
		UnComPressFile += ".Unhuffman";
		FILE *UCPfile = fopen(UnComPressFile.c_str(), "wb");
		string ComPressFile = UNfilename;
		ComPressFile += ".huffman";
		FILE *CPfile = fopen(ComPressFile.c_str(), "rb");

		//��ѹ���ַ�д���ļ�


		HuffManNode<FileInfo>*tempRoot = root;//���������
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
				if (!tempRoot->_left&&!tempRoot->_right)//��������
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