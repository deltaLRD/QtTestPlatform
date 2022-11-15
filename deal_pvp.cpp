#include<iostream>
#include <vector>
#include <fstream>
#include<string>
#include<io.h>
using namespace std;
void getFiles( string path, vector<string>& files )
{
	//文件句柄
	long   hFile   =   0;
	//文件信息
	struct _finddata_t fileinfo;
	string p;
	if((hFile = _findfirst(p.assign(path).append("\\*").c_str(),&fileinfo)) !=  -1)
	{
		do
		{
			//如果是目录,迭代之
			//如果不是,加入列表
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
	char * filePath = "C:\\Users\\yang\\Desktop\\2022cd\\zxl\\pvp1";
	vector<string> files;
	char * filePath1 = "C:\\Users\\yang\\Desktop\\2022cd\\zxl\\pvp2";
	vector<string> files1;
 
//获取该路径下的所有文件
//C:\\Users\\yang\\Desktop\\2.cpp
	getFiles(filePath, files);
	getFiles(filePath1, files1);
	int size = files1.size();
	char* a="gcc C:\\Users\\yang\\Desktop\\p_gcc\\main.cpp C:\\Users\\yang\\Desktop\\p_gcc\\ball.cpp C:\\Users\\yang\\Desktop\\p_gcc\\ballpool.cpp C:\\Users\\yang\\Desktop\\p_gcc\\bullet.cpp C:\\Users\\yang\\Desktop\\p_gcc\\bulletpool.cpp C:\\Users\\yang\\Desktop\\p_gcc\\core.cpp C:\\Users\\yang\\Desktop\\p_gcc\\flyobject.cpp C:\\Users\\yang\\Desktop\\p_gcc\\objectpool.cpp C:\\Users\\yang\\Desktop\\p_gcc\\plane.cpp C:\\Users\\yang\\Desktop\\p_gcc\\pooledobject.cpp  ";
	string b;
	char* c=" -o C:\\Users\\yang\\Desktop\\2022cd\\zxl\\pvp_exe\\";
	char* d=".exe -lstdc++ -std=c++11";
	//b=a+files[22]+' '+files1[56]+c+to_string(22)+'-'+to_string(56)+d;
	
	
	/*for(int i=0;i<size;i++){
		cout<<i<<": "<<files[i]<<" "<<files1[i]<<'\n';
	}*/
	
	
	for(int i=0;i<size-2;i++){
		//cout<<files[i].substr(36,20)<<'\n'
		for(int j=i+1;j<size-1;j++){
			b=a+files[i]+" "+files1[j]+c+to_string(i)+"-"+to_string(j)+d;
			cout<<b<<endl;
			system(b.c_str());
		}
	}
	return 0;
}
