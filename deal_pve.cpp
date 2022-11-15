#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <filesystem>
namespace fs = std::filesystem;

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

bool compilePveFile(const fs::path& p, const std::vector<fs::path>& deps, const std::vector<fs::path>& include_dirs)
{
	fs::path path = p;
	std::string compile = "g++";
	std::vector<std::string> compileflags;
	compileflags.push_back("-std=c++17");
	compileflags.push_back("-g");
	for(auto include_dir:include_dirs)
	{
		compileflags.push_back("-I"+include_dir.string()+" ");
	}
	std::string flagsstr = "";
	for(auto flag:compileflags)
	{
		flagsstr.append(flag+" ");
	}
	std::string srcs;
	for(auto dep:deps)
	{
		srcs.append(dep.string()+" ");
	}
	srcs.append(path.string()+" ");
	std::string compileCommand = compile+" "+flagsstr+srcs+"-o "+path.replace_extension(".exe").string();
	compileCommand.append(" >"+path.string()+".log 2>&1");
	std::cout << compileCommand << std::endl;
	return system(compileCommand.c_str())==0;
	
}

int main(){
	fs::path workpath{fs::current_path()};
	fs::path pvepath = workpath / "student_data" / "pve";
	fs::path depsdir = workpath / "AI_Process";
	if(!fs::exists(pvepath))
	{
		std::cout << pvepath.c_str() << " do not exist" << std::endl;
	}
	if(!fs::exists(depsdir))
	{
		std::cout << depsdir.c_str() << " do not exist" << std::endl;
	}
	std::vector<fs::path> pvefiles = getfiles(pvepath,[](const fs::path& path){return path.extension()==".cpp";});
	std::vector<fs::path> deps = getfiles(depsdir, [](const fs::path& path){return path.filename()=="main.cpp";});
	std::vector<fs::path> include_dirs;
	include_dirs.push_back(workpath/"AI_Process");
	for(auto file:pvefiles)
	{
		compilePveFile(file,deps,include_dirs);
	}
	
	return 0;
}
