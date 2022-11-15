#include<iostream>
#include <vector>
#include <fstream>
#include<string>
#include<io.h>
using namespace std;
struct people{
	string a;
	int score;
};
people student[70];
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
	/*char * filePath = "C:\\Users\\yang\\Desktop\\2022cd\\cyf\\pve";
	vector<string> files;
	getFiles(filePath, files);
	int size = files.size();
	for(int i=0;i<size-1;i++){
		student[i].a=files[i].substr(37,12);
		//cout<<i<<": "<<student[i].a<<'\n';
	}
	*/
	char * filePath2 = "C:\\Users\\yang\\Desktop\\2022cd\\zxl\\pve_exe";
	vector<string> files2;
	getFiles(filePath2, files2);
	char* begin="start ";
	string b,c;
	int size2 = files2.size();
	for(int i=15;i<size2;i++){
		b=begin+files2[i];	
		cout<<b<<endl;
		system(b.c_str());
		_sleep(120*1000);		
	}
	
/*	FILE *fp;
	int result;
	for(int i=0,j=0;i<size2&&j<size-1;i++){
		for(;j<size-1;j++){ 
			if(c==student[j].a)
			break;
		}
		fp=fopen("D:\\out.txt","r");
		fscanf(fp,"%d",&result);
		fclose(fp);
		student[j].score=result;
	}
	fp=fopen("C:\\Users\\yang\\Desktop\\2022cd\\cyf\\cyf.txt","w");
	for(int i=0;i<size-1;i++){
		fputs(student[i].a.c_str(),fp);
		fputc(' ',fp);
		fprintf(fp,"%d",student[i].result);
		fprintf(fp,"\n");
	}*/
	return 0;
}
