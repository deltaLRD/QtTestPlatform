#include <iostream>
#include <Windows.h>
#include <string>
#include <cstring>
#include <stdio.h>
#include <thread>
#include <algorithm>
#include <vector>
#include <fstream>
#include <io.h>
#include "DataStruct.h"

#define DATA_SHARED  L"DataShared"
#define KEY_SHARED L"KeyShared"
#define PLAYER_SHARED L"PlayerShared"
#define START_STOP_SHARED L"StartStopShared" 
//PVP,PVE用不同的共享内存存储进程信息并通信 
#define PROCESS_INFO_SHARED L"ProcessInfoShared" 

using namespace std;
/* run this program using the console pauser or add your own getch, system("pause") or input loop */
//声明的pve接口 
void ai_pve(DataStruct*data,KeyStruct*key);
void*threadFuc(void*threadarg){
	struct ThreadArg *thrdArg;
 	thrdArg = (struct ThreadArg *) threadarg;
 	ai_pve(thrdArg->data,thrdArg->key);
 	//运行结束将timeout置0
	thrdArg->timeout=0; 
}
void*timer(void*threadarg){
	Sleep(10);
}
void showDataStruct(DataStruct data){
    cout<<"balls:\n";
    for(int i=0;i<data.ball_size;i++){
        cout<<"x="<<data.ball[i].x<<" y="<<data.ball[i].y<<endl;
    }
    cout<<"plane1:\n x="<<data.plane1.x<<" y="<<data.plane1.y<<endl;
    cout<<"plane2:\n x="<<data.plane2.x<<" y="<<data.plane2.y<<endl;
}
int findLastIndexOfCharFromArr(char ch,char*str){
	for(int i=strlen(str)-1;i>=0;i--){
		if(str[i]==ch){
			return i;
		}
	}
	return -1;
}
int main(int argc, char** argv) {

	//初始化key结构体，作为参数传给ai_pvp和ai_pve 
	struct KeyStruct key; 
	//存放读取的结构体数据 
	struct DataStruct data;
	
	int timeout_count=0;
 
    int offset=0;
    //主进程在游戏结束后，发送标志位让AI进程结束
    bool startFlag=false,stopFlag=false;

    //共享文件的句柄
    HANDLE data_Handle=NULL,key_Handle=NULL,/*start_stop_Handle=NULL,*/process_info_Handle=NULL;
    //共享文件的指针
    LPVOID data_ptr=NULL,key_ptr=NULL,/*start_stop_ptr=NULL,*/process_info_ptr=NULL;
    
    	//获取进程名称 
	cout<<argv[0]<<endl;
	int index_Dot = findLastIndexOfCharFromArr('.',argv[0]);
	int index_Slash = findLastIndexOfCharFromArr('\\',argv[0]);
//	cout<<index_Dot<<endl<<index_Slash<<endl;
	//存储进程的名称 
	char name[20];
	memcpy(name,argv[0]+index_Slash+1,(index_Dot-index_Slash-1));
	name[index_Dot-index_Slash-1]='\0';
//	cout<<"name="<<name<<strlen(name)<<endl;

	//将进程信息写入共享内存
	/*
	process_info_Handle= CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_EXECUTE_READWRITE, 0, 0x11111111, PROCESS_INFO_SHARED);
	process_info_ptr = MapViewOfFile(process_info_Handle,FILE_MAP_ALL_ACCESS,0,0,0x11111111);
	
	struct ProcessInfoStruct pInfo;
	strcpy(pInfo.name,name);
	memcpy(process_info_ptr,&pInfo,sizeof(ProcessInfoStruct));*/
    
    
    //创建key句柄，并映射得到key_ptr
    key_Handle = CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_EXECUTE_READWRITE, 0, 0x11111111, KEY_SHARED);
    key_ptr = MapViewOfFile(key_Handle,FILE_MAP_ALL_ACCESS,0,0,0x11111111);
	
	
	
	//打开共享文件，查看主进程是否开启，是则写入Flag表明ai进程启动 
//	FlagStruct flag_Main,flag_Self={1,1};
	struct ProcessInfoStruct pInfo_Main,pInfo_Self;
	pInfo_Self.id=1;
	pInfo_Self.exist=1;
	strcpy(pInfo_Self.name,name);
	
	/*start_stop_Handle=OpenFileMappingW(FILE_MAP_ALL_ACCESS, false, START_STOP_SHARED);

	if(start_stop_Handle){
		start_stop_ptr = MapViewOfFile(start_stop_Handle,FILE_MAP_ALL_ACCESS,0,0,0x11111111);
    	//读取主进程Flag 
    	memcpy(&flag_Main,start_stop_ptr,sizeof(FlagStruct));
    	if(flag_Main.id==0&&flag_Main.exist==1){
    		memcpy((unsigned char*)start_stop_ptr+sizeof(FlagStruct),&flag_Self,sizeof(FlagStruct));
    		cout<<"main process & ai process both start！！\n";
    		startFlag = true; 
		}
	}else{
		cout<<"main process not started!!\n";
	}*/
	
	process_info_Handle=OpenFileMappingW(FILE_MAP_ALL_ACCESS, false, PROCESS_INFO_SHARED);
		//如果主进程没启动，可能需要循环等待 
	if(process_info_Handle){
		process_info_ptr=MapViewOfFile(process_info_Handle,FILE_MAP_ALL_ACCESS,0,0,0x11111111);
		//读取主进程信息
		memcpy(&pInfo_Main,process_info_ptr,sizeof(ProcessInfoStruct));
		if(pInfo_Main.id==0&&pInfo_Main.exist==1){
			memcpy((unsigned char*)process_info_ptr+sizeof(ProcessInfoStruct),&pInfo_Self,sizeof(ProcessInfoStruct));
			cout<<"main process & ai process both start！！\n";
    		startFlag = true; 
		} 
	}else{
		cout<<"main process not started!!\n";
	}
	

	
    while(!stopFlag&&startFlag){
    	//获取系统时间 
    	/*
    		
		SYSTEMTIME sys; 
    	GetLocalTime( &sys ); 
    	printf( "\nstart while= %4d/%02d/%02d %02d:%02d:%02d.%03d 星期%1d\n",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute, sys.wSecond,sys.wMilliseconds,sys.wDayOfWeek);
    	
    	//循环读取Flag，查看是否需要停止进程运行
		memcpy(&flag_Main,(unsigned char*)start_stop_ptr+2*sizeof(FlagStruct),sizeof(FlagStruct));
		if(flag_Main.id==0&&flag_Main.exist==-1){
//			cout<<"Main process want to stop!!!\n"; 
			stopFlag=false;
			startFlag=false; 
			//将自己停止的flag写入共享内存
			flag_Self={1,-1};
			memcpy((unsigned char*)start_stop_ptr+3*sizeof(FlagStruct),&flag_Self,sizeof(FlagStruct));
			break;
		}*/
		
		memcpy(&pInfo_Main,(unsigned char*)process_info_ptr+2*sizeof(ProcessInfoStruct),sizeof(ProcessInfoStruct));
		if(pInfo_Main.id==0&&pInfo_Main.exist==-1){
			//cout<<"Main process want to stop!!!\n"; 
			stopFlag=false;
			startFlag=false; 
			//将自己停止的flag写入共享内存
			pInfo_Self.id=1;
			pInfo_Self.exist=-1;
			memcpy((unsigned char*)process_info_ptr+3*sizeof(ProcessInfoStruct),&pInfo_Self,sizeof(ProcessInfoStruct));
			break;
		}
		
        //打开共享的data文件对象
        data_Handle = OpenFileMappingW(FILE_MAP_ALL_ACCESS, false, DATA_SHARED);
        //判断句柄是否存在
        if (data_Handle)
        {
            //通过映射得到文件对象的指针
            data_ptr = MapViewOfFile(data_Handle, FILE_MAP_ALL_ACCESS, 0, 0, 0);
            //从共享文件中读取data结构体数据 
			memcpy(&data,(unsigned char *)data_ptr+offset*sizeof (DataStruct),sizeof(DataStruct));
//			cout<<"data.id="<<data.id<<" offset="<<offset<<endl;
			
			if(offset==data.id){	
//				cout<<"data.id & data.ack match!!!\n";
//				showDataStruct(data);
				
//				ai1_pve(&data,&key); 
				/*
				启动一个线程去调用接口函数
				根据timeout判断是否超时 
				*/ 
				ThreadArg threadArg;
				threadArg.timeout=1;
				threadArg.key=&key;
				threadArg.data=&data;
				pthread_t tid=1;
				pthread_create(&tid,NULL,threadFuc,&threadArg);
				Sleep(10);
				pthread_cancel(tid); 
				if(threadArg.timeout==0){
//					cout<<"offset="<<offset<<" AI no timeout!\n";
					timeout_count=0;
				}else if(threadArg.timeout==1){
					cout<<"offset="<<offset<<" AI  timeout!\n";
					timeout_count++;
				}
				//利用key.id=-1标志超时 
				if(timeout_count>=3){
					key.id=-1;
					cout<<"timeout reached three times!!!\n" ;
				}else{
					key.id=offset;
				}
//				key=threadArg.key; 
				
				memcpy((unsigned char*)key_ptr+offset*sizeof(KeyStruct),&key,sizeof(KeyStruct));
				if(key_Handle){
//					cout<<"key_handle exist!\n";
				}
//				memcpy(&key,(unsigned char*)key_ptr+offset*sizeof(KeyStruct),sizeof(KeyStruct));
//				cout<<"key id="<<key.id<<" forward="<<key.forward<<" left="<<key.rotate_left<<" right="<<key.rotate_right<<" shoot="<<key.shoot<<" shield="<<key.shield<<endl;
				offset=data.id+1;
			}else{
//				cout<<"not expected data\n";
				Sleep(1);
			}

            UnmapViewOfFile(data_ptr);
            CloseHandle(data_Handle);
        }else{
            //输出
//            cout<<"data file not exist!!!  last error:"<<GetLastError()<<endl;
            Sleep(10);

        }

    }
	
	//解除映射
    UnmapViewOfFile(key_ptr);
//    UnmapViewOfFile(start_stop_ptr);
    UnmapViewOfFile(process_info_ptr);
    //关闭句柄

    CloseHandle(key_Handle);
//    CloseHandle(start_stop_Handle);
    CloseHandle(process_info_Handle);
    cout<<"AI process stoped\n";
//    pthread_exit(NULL);
	return 0;
}
