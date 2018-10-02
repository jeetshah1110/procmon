using namespace std
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <string.h>
#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <io.h>

typedef struct LogFile
{
    char ProcessName[100];
    unsigned int pid;
    unsigned int ppid;
    unsigned int thread_cnt;
}LOGFILE;

class ThreadInfo
{
private:
    DWORD PID;
    HANDLE hThreadSnap;
    THREADENTRY32 te32;
public:
    ThreadInfo(DWORD);
    BOOL ThreadsDisplay();
};

ThreadInfo::ThreadInfo(DWORD no)
{
    PID = no;
    hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,PID);
    if(hThreadSnap == INVALID_HANDLE_VALUE)
    {
        cout<<"Unable to create the snapshot of the current thread pool"<<endl;
        return;
    }
    te32.dwSize = sizeof(THREADENTRY32);
}

BOOL ThreadInfo::ThreadsDisplay()
{
    if(!Thread32First(hThreadSnap, &te32))
    {
        cout<<"Error in getting the first thread"<<endl;
        CloseHandle(hThreadSnap);
        return false;
    }
    cout<<endl<<"THREAD OF THIS PROCESS: ";
    do
    {
        if(te32.th32OwnerProessID == PID)
        {
            cout<<"\tTHREAD ID : "<<te32.th32ThreadId<<endl;
        }
    }while(Thread32Next(hThreadSnap, &te32));
    CloseHandle(hThreadSnap);
    return true;
}

class DLLInfo
{
private:
    DWORD PID;
    MODULEENTRY32 me32;
    HANDLE hProcessSnap;
public:
    DLLInfo(DWORD);
    BOOL DependentDLLDisplay();
};

DLLInfo::DLLInfo(DWORD no)
{
    PID = no;
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, PID);
    if(hProcessSnap == INVALID_HANDLE_VALUE)
    {
        cout<<"Unable to create the snapshot of the current thread pool"<<endl;
        return;
    }
    me32.dwSize = sizeof(MODULEENTRY32);
}

BOOL DLLInfo::DependentDLLDisplay()
{
    char arr[200];
    if(Module32First(hProcessSnap, &me32))
    {
        cout<<"FAILED to get DLL Information"<<endl;
        CloseHandle(hProcessSnap);
        return false;
    }
    cout<<"DEPENENT DLL OF THIS PROCESS: "<<endl;
    do
    {
        wcstombs_s(NULL,arr,200,me32.szModule,200);
        cout<<arr<<endl;
    }while(Module32Next(hProcessSnap, &me32));
    CloseHandle(hProcessSnap);
    return true;
}

class ProcessInfo
{
private:
    DWORD PID;
    DLLInfo *pdobj;
    ThreadInfo *ptobj;
    HANDLE hProcessSnap;
    PROCESSENTRY pe32;
public:
    ProcessInfo();
    BOOL ProcessDisplay(char *);
    BOOL ProcessLog;
    BOOL ReadLog(DWORD,DWORD,DWORD,DWORD);
    BOOL ProcessSearch(char *);
    BOOL KillProcess(char*);
};

ProcessInfo::ProcessInfo()
{
    ptobj = NULL;
    pdobj = NULL;

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, 0);
    if(hProcessSnap == INVALID_HANDLE_VALUE)
    {
        cout<<"Unable to create the snapshot of the current running process"<<endl;
        return;
    }
    pe32.dwSize = sizeof(PROCESSENTRY32);
}

BOOL ProcessInfo::ProcessLog()
{
    char *month[] = {"JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC"};
    char FileName[50], arr[512];
    int ret = 0, fd 0, count = 0;
    SYSTEMTIME It;
    LOGFILE fobj;
    FILE *fp;

    GetLocalTime(&It);

    sprintf_s(FieName, "C://Marvellous %02d_%02d_%02d %s.txt", It.wHour, It.wMinute, It.wDay, month[It.wMonth - 1]);
    fp = fopen(FileName, "wb");
    if(fp == NULL)
    {
        cout<<"Unable to create a Log File<<endl";
        return false;
    }
    else
    {
        cout<<"Log File successfully created as "<<FileName<<endl;
        cout<<"Time of Log File creation: "<<IT.wHour<<"."<<IT.wMinute<<"."<<IT.wDay<<"th"<<month[It.wMonth - 1]<<endl;
    }
    if(!Process32First(hProcessSnap, &pe32))
    {
        cout<<"Error in finding the first process"<<endl;
        CloseHandle(hProcessSnap);
        return false;
    }
    do
    {
        wcstombs_s(NULL,arr,200,pe32.szExeFile,200);
        strcpy_s(fobj.ProcessName,arr);
        fobj.pid = pe32.th32ProcessID;
        fobj.ppid = pe32.th32ParentProcessID;
        fobj.thread_cnt = pe32.cntThreads;
        fwrite(&fobj, sizeof(fobj), 1, fp);
    }while(Process32Next(hProcessSnap, &pe32));
    CloseHandle(hProcessSnap);
    fclose(fp);
    return true;
}

BOOL ProcessInfo::ProcessDisplay(char* option)
{
    char arr[200];
    if(!Process32First(hProcessSnap, &pe32))
    {
        cout<<"Error in finding the first process"<<endl;
        CloseHandle(hProcessSnap);
        return false;
    }
    do
    {
        cout<<endl<<"--------------------------------------";
        wcstombs_s(NULL,arr,200,pe32.szExeFile,200);
        cout<<endl<<"PROCESS NAME: "<<arr;
        cout<<endl<<"PID: "<<pe32.th32ProcessID;
        cout<<endl<<"Parent ID: "<<pe32.th32ParentProcessID;
        cout<<endl<<"No of Thread: "<<pe32.cntThreads;

        if((_stricmp(option, "-a") == 0) || (_stricmp(option, "-d") == 0) || (_stricmp(option, "-t") == 0))
        {
            if((_stricmp(option, "-t") == 0) || (_stricmp(option, "-a") == 0))
            {
                ptobj = new ThreadInfo(pe32.th32processID);
                ptobj->ThreadsDisplay();
                delete ptobj;
            }
            if((_stricmp(option, "-d") == 0) || (_stricmp(option, "-a") == 0))
            {
                pdobj = new DLLInfo(pe32.th32processID);
                pdobj->DependentDLLDisplay();
                delete pdobj;
            }
        }
        cout<<endl<<"--------------------------------------";
    }while(Process32Next(hProcessSnap, &pe32));
    CloseHandle(hProcessSnap);
    return true;
}
