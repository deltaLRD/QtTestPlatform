#include<iostream>
#include <vector>
#include <fstream>
#include<string>
#include<io.h>
using namespace std;
#include <filesystem>
namespace fs = std::filesystem;
struct people{
	string a;
	int score;
};
people student[70];
/**
 * @brief get the files under the path 
 * @tparam L lambda
 * @param path const fs::path& path --- the path of the directory
 * @param lamb lambda --- the pointer of the function of the filter
 * @return std::vector<fs::path> --- the vector of the files' path
 */
template<typename L>
std::vector<fs::path> getfiles(const fs::path& path, L lamb)
{
	std::vector<fs::path> files;
	if(!fs::is_directory(path))
	{
		std::cout << path.c_str() << " is not a directory" << std::endl;
		return files;
	}
	for(auto const& dir_entry: fs::directory_iterator(path))
	{
		if(lamb(dir_entry.path()) == false)continue;
		std::cout << dir_entry.path().filename() << std::endl;
		files.push_back(dir_entry.path());
	}
	return files;
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
