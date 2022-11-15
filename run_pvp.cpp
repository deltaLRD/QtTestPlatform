#include<iostream>
#include <vector>
#include <fstream>
#include<string>
#include<io.h>
using namespace std;
struct people{
	string a;
	int win=0;
	int lose=0;
	int ping=0;
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
	char * filePath = "C:\\Users\\yang\\Desktop\\cd\\lou\\pvp1";
	vector<string> files;
	getFiles(filePath, files);
	int size = files.size();
	for(int i=0;i<size-1;i++){
		student[i].a=files[i].substr(34,12);
		//cout<<i<<": "<<student[i].a<<'\n';
	}
	
	
	char * filePath2 = "C:\\Users\\yang\\Desktop\\cd\\lou\\pvp_exe";
	vector<string> files2;
	getFiles(filePath2, files2);
	char* begin="start ";
	string b;
	int size2 = files2.size();
	int p1,p2;
	for(int i=0;i<size2;i++){
		cout<<i<<": ";
		b=begin+files2[i];
		p1=0;
		p2=0;
		int j=0;
		for(;j<files2[i].size();j++){
			if(files2[i][j]>='0'&&files2[i][j]<='9')
				p1=p1*10+files2[i][j]-48;
			if(files2[i][j]=='-')
			break;
		}
		j++;
		for(;j<files2[i].size();j++){
			if(files2[i][j]!='.'){
				p2=p2*10+files2[i][j]-48;
			}
			else
			break;
		}
		cout<<b<<endl;
		system(b.c_str());
		_sleep(20*1000);
		FILE *fp;
		int result;
		fp=fopen("D:\\out.txt","r");	
		fscanf(fp,"%d",&result);
		fclose(fp);
		if(result==0){
			student[p1].ping++;
			student[p2].ping++;
		}
		if(result==1){
			student[p1].win++;
			student[p2].lose++;
		}
		if(result==2){
			student[p1].lose++;
			student[p2].win++;
		}
	}
	
	
	FILE *fp;
	fp=fopen("C:\\Users\\yang\\Desktop\\cd\\lou\\pvp.txt","w");
	for(int i=0;i<size-1;i++){
		fputs(student[i].a.c_str(),fp);
		fputc(' ',fp);
		fprintf(fp,"%d",student[i].win);
		fputc(' ',fp);
		fprintf(fp,"%d",student[i].lose);
		fputc(' ',fp);
		fprintf(fp,"%d",student[i].ping);
		fprintf(fp,"\n");
	}
	
	
	return 0;
}
