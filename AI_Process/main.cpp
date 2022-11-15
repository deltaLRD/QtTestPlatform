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
//PVP,PVE�ò�ͬ�Ĺ����ڴ�洢������Ϣ��ͨ�� 
#define PROCESS_INFO_SHARED L"ProcessInfoShared" 

using namespace std;
/* run this program using the console pauser or add your own getch, system("pause") or input loop */
//������pve�ӿ� 
void ai_pve(DataStruct*data,KeyStruct*key);
void*threadFuc(void*threadarg){
	struct ThreadArg *thrdArg;
 	thrdArg = (struct ThreadArg *) threadarg;
 	ai_pve(thrdArg->data,thrdArg->key);
 	//���н�����timeout��0
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

	//��ʼ��key�ṹ�壬��Ϊ��������ai_pvp��ai_pve 
	struct KeyStruct key; 
	//��Ŷ�ȡ�Ľṹ������ 
	struct DataStruct data;
	
	int timeout_count=0;
 
    int offset=0;
    //����������Ϸ�����󣬷��ͱ�־λ��AI���̽���
    bool startFlag=false,stopFlag=false;

    //�����ļ��ľ��
    HANDLE data_Handle=NULL,key_Handle=NULL,/*start_stop_Handle=NULL,*/process_info_Handle=NULL;
    //�����ļ���ָ��
    LPVOID data_ptr=NULL,key_ptr=NULL,/*start_stop_ptr=NULL,*/process_info_ptr=NULL;
    
    	//��ȡ�������� 
	cout<<argv[0]<<endl;
	int index_Dot = findLastIndexOfCharFromArr('.',argv[0]);
	int index_Slash = findLastIndexOfCharFromArr('\\',argv[0]);
//	cout<<index_Dot<<endl<<index_Slash<<endl;
	//�洢���̵����� 
	char name[20];
	memcpy(name,argv[0]+index_Slash+1,(index_Dot-index_Slash-1));
	name[index_Dot-index_Slash-1]='\0';
//	cout<<"name="<<name<<strlen(name)<<endl;

	//��������Ϣд�빲���ڴ�
	/*
	process_info_Handle= CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_EXECUTE_READWRITE, 0, 0x11111111, PROCESS_INFO_SHARED);
	process_info_ptr = MapViewOfFile(process_info_Handle,FILE_MAP_ALL_ACCESS,0,0,0x11111111);
	
	struct ProcessInfoStruct pInfo;
	strcpy(pInfo.name,name);
	memcpy(process_info_ptr,&pInfo,sizeof(ProcessInfoStruct));*/
    
    
    //����key�������ӳ��õ�key_ptr
    key_Handle = CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_EXECUTE_READWRITE, 0, 0x11111111, KEY_SHARED);
    key_ptr = MapViewOfFile(key_Handle,FILE_MAP_ALL_ACCESS,0,0,0x11111111);
	
	
	
	//�򿪹����ļ����鿴�������Ƿ���������д��Flag����ai�������� 
//	FlagStruct flag_Main,flag_Self={1,1};
	struct ProcessInfoStruct pInfo_Main,pInfo_Self;
	pInfo_Self.id=1;
	pInfo_Self.exist=1;
	strcpy(pInfo_Self.name,name);
	
	/*start_stop_Handle=OpenFileMappingW(FILE_MAP_ALL_ACCESS, false, START_STOP_SHARED);

	if(start_stop_Handle){
		start_stop_ptr = MapViewOfFile(start_stop_Handle,FILE_MAP_ALL_ACCESS,0,0,0x11111111);
    	//��ȡ������Flag 
    	memcpy(&flag_Main,start_stop_ptr,sizeof(FlagStruct));
    	if(flag_Main.id==0&&flag_Main.exist==1){
    		memcpy((unsigned char*)start_stop_ptr+sizeof(FlagStruct),&flag_Self,sizeof(FlagStruct));
    		cout<<"main process & ai process both start����\n";
    		startFlag = true; 
		}
	}else{
		cout<<"main process not started!!\n";
	}*/
	
	process_info_Handle=OpenFileMappingW(FILE_MAP_ALL_ACCESS, false, PROCESS_INFO_SHARED);
		//���������û������������Ҫѭ���ȴ� 
	if(process_info_Handle){
		process_info_ptr=MapViewOfFile(process_info_Handle,FILE_MAP_ALL_ACCESS,0,0,0x11111111);
		//��ȡ��������Ϣ
		memcpy(&pInfo_Main,process_info_ptr,sizeof(ProcessInfoStruct));
		if(pInfo_Main.id==0&&pInfo_Main.exist==1){
			memcpy((unsigned char*)process_info_ptr+sizeof(ProcessInfoStruct),&pInfo_Self,sizeof(ProcessInfoStruct));
			cout<<"main process & ai process both start����\n";
    		startFlag = true; 
		} 
	}else{
		cout<<"main process not started!!\n";
	}
	

	
    while(!stopFlag&&startFlag){
    	//��ȡϵͳʱ�� 
    	/*
    		
		SYSTEMTIME sys; 
    	GetLocalTime( &sys ); 
    	printf( "\nstart while= %4d/%02d/%02d %02d:%02d:%02d.%03d ����%1d\n",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute, sys.wSecond,sys.wMilliseconds,sys.wDayOfWeek);
    	
    	//ѭ����ȡFlag���鿴�Ƿ���Ҫֹͣ��������
		memcpy(&flag_Main,(unsigned char*)start_stop_ptr+2*sizeof(FlagStruct),sizeof(FlagStruct));
		if(flag_Main.id==0&&flag_Main.exist==-1){
//			cout<<"Main process want to stop!!!\n"; 
			stopFlag=false;
			startFlag=false; 
			//���Լ�ֹͣ��flagд�빲���ڴ�
			flag_Self={1,-1};
			memcpy((unsigned char*)start_stop_ptr+3*sizeof(FlagStruct),&flag_Self,sizeof(FlagStruct));
			break;
		}*/
		
		memcpy(&pInfo_Main,(unsigned char*)process_info_ptr+2*sizeof(ProcessInfoStruct),sizeof(ProcessInfoStruct));
		if(pInfo_Main.id==0&&pInfo_Main.exist==-1){
			//cout<<"Main process want to stop!!!\n"; 
			stopFlag=false;
			startFlag=false; 
			//���Լ�ֹͣ��flagд�빲���ڴ�
			pInfo_Self.id=1;
			pInfo_Self.exist=-1;
			memcpy((unsigned char*)process_info_ptr+3*sizeof(ProcessInfoStruct),&pInfo_Self,sizeof(ProcessInfoStruct));
			break;
		}
		
        //�򿪹����data�ļ�����
        data_Handle = OpenFileMappingW(FILE_MAP_ALL_ACCESS, false, DATA_SHARED);
        //�жϾ���Ƿ����
        if (data_Handle)
        {
            //ͨ��ӳ��õ��ļ������ָ��
            data_ptr = MapViewOfFile(data_Handle, FILE_MAP_ALL_ACCESS, 0, 0, 0);
            //�ӹ����ļ��ж�ȡdata�ṹ������ 
			memcpy(&data,(unsigned char *)data_ptr+offset*sizeof (DataStruct),sizeof(DataStruct));
//			cout<<"data.id="<<data.id<<" offset="<<offset<<endl;
			
			if(offset==data.id){	
//				cout<<"data.id & data.ack match!!!\n";
//				showDataStruct(data);
				
//				ai1_pve(&data,&key); 
				/*
				����һ���߳�ȥ���ýӿں���
				����timeout�ж��Ƿ�ʱ 
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
				//����key.id=-1��־��ʱ 
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
            //���
//            cout<<"data file not exist!!!  last error:"<<GetLastError()<<endl;
            Sleep(10);

        }

    }
	
	//���ӳ��
    UnmapViewOfFile(key_ptr);
//    UnmapViewOfFile(start_stop_ptr);
    UnmapViewOfFile(process_info_ptr);
    //�رվ��

    CloseHandle(key_Handle);
//    CloseHandle(start_stop_Handle);
    CloseHandle(process_info_Handle);
    cout<<"AI process stoped\n";
//    pthread_exit(NULL);
	return 0;
}
