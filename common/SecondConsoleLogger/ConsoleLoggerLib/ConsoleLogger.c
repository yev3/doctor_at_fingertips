////////////////////////////////////////////////////////////////////////////////

#include "ConsoleLogger.h"
#include "windows.h"
#include <time.h>
#include <stdio.h>
#include "stdlib.h"
#include <fcntl.h>
#include "io.h"
#include "direct.h"


static const char *CONSOLE_NAME = DEFAULT_WIND_TITLE;

static char	pipeName[64] = { 0 };
static HANDLE pipeHandle = INVALID_HANDLE_VALUE;

//////////////////////////////////////////////////////////////////////////
// Create: create a new console (logger)
//////////////////////////////////////////////////////////////////////////
long LoggerInitWindow(const char *newWindowTitle,
                      const char *helperExecutableName)
{
	
	// Ensure there's no pipe connected
	if (pipeHandle != INVALID_HANDLE_VALUE)
	{
		DisconnectNamedPipe(pipeHandle);
		CloseHandle(pipeHandle);
		pipeHandle=INVALID_HANDLE_VALUE;
	}
	strcpy(pipeName,"\\\\.\\pipe\\");

	if (!newWindowTitle)
	{	// no name was give , create name based on the current address+time
		// (you can modify it to use PID , rand() ,...
	  const unsigned long now = GetTickCount();
		newWindowTitle = pipeName+ strlen(pipeName);
		sprintf((char*)newWindowTitle,"logger%d_%lu",(int)pipeName,now);
	}
	else
	{	// just use the given name
		strcat(pipeName,newWindowTitle);
	}
	
	// Create the pipe
	pipeHandle = CreateNamedPipe( 
		  pipeName,					// pipe name 
		  PIPE_ACCESS_OUTBOUND,		// read/write access, we're only writing...
		  PIPE_TYPE_MESSAGE |       // message type pipe 
		  PIPE_READMODE_BYTE|		// message-read mode 
		  PIPE_WAIT,                // blocking mode 
		  1,						// max. instances  
		  4096,						// output buffer size 
		  0,						// input buffer size (we don't read data, so 0 is fine)
		  1,						// client time-out 
		  NULL);                    // no security attribute 
	if (pipeHandle==INVALID_HANDLE_VALUE)
	{	// failure
		MessageBox(NULL,"CreateNamedPipe failed","ConsoleLogger failed",MB_OK);
		return -1;
	}

	// Extra console : create another process , it's role is to display the pipe's output
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	GetStartupInfo(&si);
	
	char cmdline[MAX_PATH];;
	if (!helperExecutableName)
		helperExecutableName=DEFAULT_HELPER_EXE;

	sprintf(cmdline,"%s %s",helperExecutableName,newWindowTitle);
	BOOL bRet = CreateProcess(NULL,cmdline,NULL,NULL,FALSE,CREATE_NEW_CONSOLE,NULL,NULL,&si,&pi);
	if (!bRet)
	{	// on failure - try to get the path from the environment
		char *path = getenv("ConsoleLoggerHelper");
		if (path)
		{
			sprintf(cmdline,"%s %s",path,newWindowTitle);
			bRet = CreateProcess(NULL,cmdline,NULL,NULL,FALSE,CREATE_NEW_CONSOLE,NULL,NULL,&si,&pi);
		}
		if (!bRet)
		{
			MessageBox(NULL,"Helper executable not found","ConsoleLogger failed",MB_OK);
			CloseHandle(pipeHandle);
			pipeHandle = INVALID_HANDLE_VALUE;
			return -1;
		}
	}
	
	BOOL bConnected = ConnectNamedPipe(pipeHandle, NULL) ? 
		 TRUE : (GetLastError() == ERROR_PIPE_CONNECTED); 
	if (!bConnected)
	{
		MessageBox(NULL,"ConnectNamedPipe failed","ConsoleLogger failed",MB_OK);
		
		CloseHandle(pipeHandle);
		pipeHandle = INVALID_HANDLE_VALUE;
		return -1;
	}
	
	DWORD cbWritten;

	//////////////////////////////////////////////////////////////////////////
	// In order to easily add new future-features , i've chosen to pass the "extra"
	// parameters just the HTTP protocol - via textual "headers" .
	// the last header should end with NULL
	//////////////////////////////////////////////////////////////////////////
	
	char buffer[128];
	// Send title
	if (!newWindowTitle)	newWindowTitle=pipeName+9;
	sprintf(buffer,"TITLE: %s\r\n",newWindowTitle);
	WriteFile(pipeHandle,buffer,strlen(buffer),&cbWritten,NULL);
	if (cbWritten!=strlen(buffer))
	{
		MessageBox(NULL,"WriteFile failed(1)","ConsoleLogger failed",MB_OK);
		DisconnectNamedPipe(pipeHandle);
		CloseHandle(pipeHandle);
		pipeHandle=INVALID_HANDLE_VALUE;
		return -1;
	}

	// send NULL as "end of header"
	buffer[0]=0;
	WriteFile(pipeHandle,buffer,1,&cbWritten,NULL);
	if (cbWritten!=1)
	{
		MessageBox(NULL,"WriteFile failed(3)","ConsoleLogger failed",MB_OK);
		DisconnectNamedPipe(pipeHandle);
		CloseHandle(pipeHandle);
		pipeHandle=INVALID_HANDLE_VALUE;
		return -1;
	}
	return 0;
}

long LoggerInit() {
  return LoggerInitWindow(DEFAULT_WIND_TITLE, DEFAULT_HELPER_EXE);
}

int writeConsolePipe(const char *lpszText,int iSize)
{
	DWORD dwWritten=(DWORD)-1;
  BOOL writeResult = WriteFile( pipeHandle,lpszText,iSize,&dwWritten,NULL);
	return (!writeResult || (int)dwWritten!=iSize) ? -1 : (int)dwWritten;
}

//////////////////////////////////////////////////////////////////////////
// printf: print a formatted string
//////////////////////////////////////////////////////////////////////////
int LoggerPrintf(const char *format,...)
{
	if (pipeHandle==INVALID_HANDLE_VALUE)
		return -1;

	int ret;
	char tmp[1024];

	va_list argList;
	va_start(argList, format);
	#ifdef WIN32
	 		ret = _vsnprintf(tmp,sizeof(tmp)-1,format,argList);
	#else
	 		ret = vsnprintf(tmp,sizeof(tmp)-1,format,argList);
	#endif
	tmp[ret]=0;

	va_end(argList);

	return writeConsolePipe(tmp,ret);
}

int LoggerWrite(const char *buf, int size) {
  return writeConsolePipe(buf, size);
}

