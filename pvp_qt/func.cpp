#include "windows.h"
#include <QString>
#include <QDebug>
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
