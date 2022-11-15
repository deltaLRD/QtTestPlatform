# README

## 免责声明

本仓库只是想让njust的Qt课设的评测进行的更加顺畅

同时帮助老师同学解决问题

仅仅关注技术问题

不针对个人

不人身攻击

## 如何参与开发

### 前提依赖

`g++` 
`git`
首先在命令行中测试以下命令

```shell
g++ -v
git -v
```

### 改进设想

1. 将学生文件放入`./student_data/pvp1` `./student_data/pvp2` `./student_data/pve`
2. 将代码中的C语言风格的代码改为C++17
3. 将所有硬编码的路径改为相对路径
4. 测试评测功能

### 开发流程

1. 电脑上找个地方
2. 打开终端切换到刚刚找到的地方
3. `git clone https://github.com/deltaLRD/QtTestPlatform.git`
4. `git branch new_feature_XXX` XXX可以是功能或者开发者自己的名字,但请使用英文
5. 编写自己的代码
6. `git push --set-upstream origin new_feature_XXX`

### 编译命令

```shell
g++ -std=c++17 deal_pve.cpp -o deal_pve.exe
```