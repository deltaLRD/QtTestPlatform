#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <filesystem>
namespace fs = std::filesystem;

// not support subdir
std::vector<fs::path> getfiles(const fs::path& path)
{
	std::vector<fs::path> files;
	if(!fs::is_directory(path))
	{
		std::cout << path.c_str() << " is not a directory" << std::endl;
		return files;
	}
	for(auto const& dir_entry: fs::directory_iterator(path))
	{
		std::cout << dir_entry.path().filename() << std::endl;
		files.push_back(dir_entry.path());
	}
	return files;
}

int main(){
	fs::path workpath{fs::current_path()};
	fs::path pvepath = workpath / "student_data" / "pve";
	if(!fs::exists(pvepath))
	{
		std::cout << pvepath.c_str() << " do not exist" << std::endl;
	}
	std::vector<fs::path> pvefiles = getfiles(pvepath);
	
// //获取该路径下的所有文件
// //C:\\Users\\yang\\Desktop\\2.cpp
// 	getFiles(filePath, files);
// 	int size = files.size();
// 	char* a="gcc D:\\code\\AI_Process\\main.cpp ";
// 	string b;
// 	char* c=" -o C:\\Users\\yang\\Desktop\\2022cd\\zxl\\pve_exe\\";
// 	char* d=".exe -lstdc++ -std=c++11";
// 	//b=a+files[22]+' '+files1[56]+c+to_string(22)+'-'+to_string(56)+d;
// 	for(int i=0;i<size;i++){
// 		//cout<<files[i].substr(41,12)<<'\n';
// 	//	cout<<i<<": "<<files[i]<<'\n';
// 		//b=a+files[i]+c+files[i].substr(37,12)+d;
// 		//cout<<i<<": "<<b<<'\n';
// 	}

// 	for(int j=0;j<size-1;j++){
// 		b=a+files[j]+c+files[j].substr(37,12)+d;
// 		//cout<<b<<endl;
// 		system(b.c_str());
// 	}
// 	cout<<'\n'<<"over";
	return 0;
}
