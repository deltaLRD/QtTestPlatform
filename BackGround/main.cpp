#include <QCoreApplication>
#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h>
#include <core.h>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include"func.h"

#define INPUT_SIZE  4096   //内存空间的大小
#define  _CRT_SECURE_NO_WARNINGS
using namespace std;
void MainStop(){

};
void execute_command(const char *command )
{
    LPWSTR ppCmd = new TCHAR[100];
    LPSTR p = (LPSTR)command;
    MultiByteToWideChar(CP_ACP, 0, p, -1, ppCmd, 100);

    SECURITY_ATTRIBUTES   sa;
    HANDLE   hRead,hWrite;

    sa.nLength   =   sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor   =   NULL;
    sa.bInheritHandle   =   TRUE;
    if   (!CreatePipe(&hRead,&hWrite,&sa,0))
    {
        return  ;
    }

    STARTUPINFO   si;
    PROCESS_INFORMATION   pi;
    si.cb   =   sizeof(STARTUPINFO);
    GetStartupInfo(&si);
    si.hStdError   =   hWrite;
    si.hStdOutput   =   hWrite;
    si.wShowWindow   =   SW_HIDE;
    si.dwFlags   =   STARTF_USESHOWWINDOW   |   STARTF_USESTDHANDLES;
    //关键步骤，CreateProcess函数参数意义请查阅MSDN
    if   (!CreateProcess(NULL,   ppCmd
                         ,NULL,NULL,TRUE,NULL,NULL,NULL,&si,&pi))
    {
        return  ;
    }

    CloseHandle(hWrite);

    char   buffer[4096]   =   {0};
    DWORD   bytesRead;
    //ofstream outfile("log.txt");

    while (true)
    {
        if (ReadFile(hRead,buffer,4095,&bytesRead,NULL)   ==   NULL)
            break;


        QString qsMsg = QString::fromLocal8Bit( buffer );
        qDebug() << qsMsg;
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Core::GameType gameType=Core::PVE;
    time_t gameTime=60;

    //读取命令行参数，启动游戏
    QStringList cmdline_args = QCoreApplication::arguments();
    if(cmdline_args.size()==2){
        gameTime=(Core::GameType)cmdline_args[1].toInt();
    }
//    QString ai1_str,ai2_str;

    /*
    * 启动时应该有两个或三个参数
    * 第一个参数 对局时长
    * 第二个参数 0/1代表PVE/PVE
    * 第三个参数 PVE:获取要调用的ai模块
    * 第四个参数 PVP:获取两个要调用的ai模块
    * */
    /*
    if(cmdline_args.size()<2){
        qDebug()<<"too few arguments!! \n";
        //        return 0;
    }
    if(cmdline_args.size()==3){
        gameType=(Core::GameType)cmdline_args[1].toInt();
        gameTime=cmdline_args[0].toInt();

        ai1_str=cmdline_args[2];
    }
    if(cmdline_args.size()==4){
        ai2_str=cmdline_args[3];
    }*/

    while(true){
        cout<<"ready to start a game\n";
        //清空文件内容，写入-1代表游戏未结束
        ofstream outfile("output/out.txt", ios::trunc|ios::binary);
        outfile<<-1;
        outfile.close();
        //初始化游戏类型和游戏时间，确保在不接收到命令行参数的情况下也能启动

        /*
    HANDLE start_stop_Handle = CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_EXECUTE_READWRITE, 0, 0x11111111, START_STOP_SHARED);
    LPVOID start_stop_ptr = MapViewOfFile(start_stop_Handle,FILE_MAP_ALL_ACCESS,0,0,0x11111111);

    HANDLE process_info_Handle=NULL;
    LPVOID process_info_ptr=NULL;
    struct ProcessInfoStruct pInfo;
    */
        //将主进程的信息写入共享内存，表明主进程已启动
        HANDLE process_info_Handle = CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_EXECUTE_READWRITE, 0, 0x11111111, PROCESS_INFO_SHARED);
        LPVOID process_info_ptr = MapViewOfFile(process_info_Handle,FILE_MAP_ALL_ACCESS,0,0,0x11111111);
        struct ProcessInfoStruct pInfo_Main,pInfo_AI1;
        pInfo_Main.id=0;
        pInfo_Main.exist=1;
        strcpy(pInfo_Main.name,"Main");
        memcpy(process_info_ptr,&pInfo_Main,sizeof(ProcessInfoStruct));

        /*FlagStruct flag_Main={0,1},flag_AI1;
    //写入Flag表明主进程启动，等待AI进程启动
    memcpy(start_stop_ptr,&flag_Main,sizeof(FlagStruct));*/

        while(true){
            //读取到ai进程启动时，跳出循环
            //        memcpy(&flag_AI1,(unsigned char*)start_stop_ptr+sizeof (FlagStruct),sizeof(FlagStruct));
            memcpy(&pInfo_AI1,(unsigned char*)process_info_ptr+sizeof (ProcessInfoStruct),sizeof(ProcessInfoStruct));
            //AI_PVE进程启动
            if(pInfo_AI1.id==1&&pInfo_AI1.exist==1)
            {
                //            process_info_Handle=OpenFileMappingW(FILE_MAP_ALL_ACCESS, false, PROCESS_INFO_SHARED);
                if(process_info_Handle){
                    //                process_info_ptr= MapViewOfFile(process_info_Handle,FILE_MAP_ALL_ACCESS,0,0,0x11111111);
                    //                memcpy(&pInfo_AI1,(unsigned char*)process_info_ptr+sizeof (ProcessInfoStruct),sizeof (ProcessInfoStruct));
//                    cout<<pInfo_AI1.name<<" size="<<strlen(pInfo_AI1.name)<<endl;
                    //PVE和PVP是分开的共享文件
                    gameType=Core::PVE;
                    cout<<pInfo_AI1.name<<" started!!!\n";
                    break;
                }
            }
            else{
                //一秒钟探测一次AI进程是否启动
                Sleep(1000);
                cout<<".";
            }
            //AI_PVP进程启动
        }

        Core* core=new Core(nullptr);
//        Core *core;
//        Core core;
        //当发出gameOver信号时，主进程将stopFlag写入Flag共享文件，offset=2

        //在gameOver上传递参数，比如游戏是否正常结束，写入out.txt
         QObject::connect(core,&Core::gameOver,[&](int state,int grade_ai1,int grade_ai2){
            pInfo_Main.id=0;
            pInfo_Main.exist=-1;
            strcpy(pInfo_Main.name,"Main");
            memcpy((unsigned char*)process_info_ptr+2*sizeof(ProcessInfoStruct),&pInfo_Main,sizeof(ProcessInfoStruct));
            /*
            while(true){
                if(state==0){
                    memcpy(&pInfo_AI1,(unsigned char*)process_info_ptr+3*sizeof (ProcessInfoStruct),sizeof(ProcessInfoStruct));
                    //AI_PVE进程启动
                    if(pInfo_AI1.id==1&&pInfo_AI1.exist==-1)
                    {
                        cout<<pInfo_AI1.name<<" stoped!!!\n";
                        break;
                    }
                    else{
                        //一秒钟探测一次AI进程是否启动
                        Sleep(1000);
                        cout<<"wait \n";
                    }
                }
                if(state==1){
                    cout<<pInfo_AI1.name<<" timeout!!!\n";
                    break;
                }
                if(state==2){
                    cout<<pInfo_AI1.name<<" dispeared!!!\n";
                    break;
                }
            }*/
            while(state==0){
                memcpy(&pInfo_AI1,(unsigned char*)process_info_ptr+3*sizeof (ProcessInfoStruct),sizeof(ProcessInfoStruct));
                //AI_PVE进程关闭
                if(pInfo_AI1.id==1&&pInfo_AI1.exist==-1)
                {
                    cout<<pInfo_AI1.name<<" stoped!!!\n";
                    break;
                }
                else{
                    //一秒钟探测一次AI进程是否关闭
                    Sleep(1000);
                    cout<<".";
                }
            }
            if(state==1){
                cout<<pInfo_AI1.name<<" timeout!!!\n";
            }
            if(state==2){
                cout<<pInfo_AI1.name<<" crashed!!!\n";
            }


            /*
            UnmapViewOfFile(start_stop_ptr);
            CloseHandle(start_stop_Handle);*/
//            core->~Core();

            //将游戏结束的状态写入out.txt
            //ofstream outfile("output/out.txt", ios::trunc|ios::binary);
//            ofstream outfile("output/out.txt", ios::trunc);
            ofstream outfile;
            outfile.open("output.txt",ios::app);
            cout<<"state="<<state<<endl;
            cout<<"ai1"<<grade_ai1<<endl;
            outfile<<pInfo_AI1.name<<": "<<grade_ai1<<endl;
            //outfile<<grade_ai1<<endl;
     //      outfile.write(pInfo_AI1,sizeof(ProcessInfoStruct));
     //       outfile<< core->planeOfP1->score;
            outfile.close();
            delete core;
            UnmapViewOfFile(process_info_ptr);
            CloseHandle(process_info_Handle);
            cout<<"ready to get state\n";
            if(state==0){
                cout<<"ready to get recore\n";
            }
            Sleep(10*1000);

//            cout<<"slot end\n";
        });
        core->initial(gameType,QString::fromLocal8Bit(pInfo_AI1.name,20),nullptr,gameTime);
//        core.initial(gameType,QString::fromLocal8Bit(pInfo_AI1.name,20),nullptr,gameTime);
    }

    return a.exec();
}
