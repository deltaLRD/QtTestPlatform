#include<iostream>
#include <vector>
#include <fstream>
#include<string>
#include<io.h>
using namespace std;
void getFiles( string path, vector<string>& files )
{
	//�ļ����
	long   hFile   =   0;
	//�ļ���Ϣ
	struct _finddata_t fileinfo;
	string p;
	if((hFile = _findfirst(p.assign(path).append("\\*").c_str(),&fileinfo)) !=  -1)
	{
		do
		{
			//�����Ŀ¼,����֮
			//�������,�����б�
			if((fileinfo.attrib &  _A_SUBDIR))
			{
				if(strcmp(fileinfo.name,".") != 0  &&  strcmp(fileinfo.name,"..") != 0)
					getFiles( p.assign(path).append("\\").append(fileinfo.name), files );
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name) );
			}
		}while(_findnext(hFile, &fileinfo)  == 0);
		_findclose(hFile);
	}
}
int main(){
	char * filePath = "C:\\Users\\yang\\Desktop\\2022cd\\zxl\\pve";
	vector<string> files;
 
//��ȡ��·���µ������ļ�
//C:\\Users\\yang\\Desktop\\2.cpp
	getFiles(filePath, files);
	int size = files.size();
	char* a="gcc D:\\code\\AI_Process\\main.cpp ";
	string b;
	char* c=" -o C:\\Users\\yang\\Desktop\\2022cd\\zxl\\pve_exe\\";
	char* d=".exe -lstdc++ -std=c++11";
	//b=a+files[22]+' '+files1[56]+c+to_string(22)+'-'+to_string(56)+d;
	for(int i=0;i<size;i++){
		//cout<<files[i].substr(41,12)<<'\n';
	//	cout<<i<<": "<<files[i]<<'\n';
		//b=a+files[i]+c+files[i].substr(37,12)+d;
		//cout<<i<<": "<<b<<'\n';
	}

	for(int j=0;j<size-1;j++){
		b=a+files[j]+c+files[j].substr(37,12)+d;
		//cout<<b<<endl;
		system(b.c_str());
	}
	cout<<'\n'<<"over";
	return 0;
}
