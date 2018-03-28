// ConsoleLoggerHelper.c : Defines the entry point for the console application.
//

#include "stdio.h"
#include "windows.h"
#include <stdbool.h>


CONSOLE_SCREEN_BUFFER_INFO	g_ConsoleBufferInfo;
HANDLE	g_hPipe;
BOOL	g_bExtendedConsole=FALSE;
HANDLE	g_hConsole = INVALID_HANDLE_VALUE;
DWORD	g_dwConsoleSize=0;
COORD	g_dwConsoleCoords;

long ConsoleLoop(void)
{
	
	DWORD cbRead,cbWritten;
	char chBuf[1024];
	BOOL bSuccess;
	long lResult=-1;
	do
	{
		// Read from the pipe.
		bSuccess = ReadFile(
			g_hPipe,    // pipe handle
			chBuf,    // buffer to receive reply
			sizeof(chBuf),      // size of buffer
			&cbRead,  // number of bytes read
			NULL);    // not overlapped

		if (! bSuccess && GetLastError() != ERROR_MORE_DATA)
			break; // stop only on failure

		// Reply from the pipe is written to STDOUT.  , just like printf...
		if (! WriteFile(g_hConsole,chBuf, cbRead, &cbWritten, NULL))
		{
			break;
		}

	} while (true/*!bSuccess*/);  // repeat loop if ERROR_MORE_DATA

	return lResult;

}


int main(int argc, char* argv[])
{

	if (argc==1 || !argv[1] || !argv[1][0])
	{
		MessageBox(NULL,"\nFailed to start logger\n","FAILED",MB_OK);
		return -1;
	}

	char szPipename[64];
	sprintf(szPipename,"\\\\.\\pipe\\%s",argv[1]);

	while (1)
	{
		g_hPipe = CreateFile(
			szPipename,   // pipe name
			GENERIC_READ/*|GENERIC_WRITE*/ /* GENERIC_WRITE*/, // read and write access
			0,              // no sharing
			NULL,           // no security attributes
			OPEN_EXISTING,  // opens existing pipe
			0,              // default attributes
			NULL);          // no template file

		// Break if the pipe handle is valid.

		if (g_hPipe != INVALID_HANDLE_VALUE)
			break;

		// Exit if an error other than ERROR_PIPE_BUSY occurs.

		if (GetLastError() != ERROR_PIPE_BUSY)
		{
			MessageBox(NULL,"Could not open pipe(1)","FAILED",MB_OK);
			getchar();
			return 0;
		}

		// All pipe instances are busy, so wait for 20 seconds.
		printf("Wait for pipe...");
		if (!WaitNamedPipe(szPipename, 20000))
		{
			MessageBox(NULL,"Could not open pipe(2)","FAILED",MB_OK);
			getchar();
			return 0;
		}
		Sleep(500);
	}

	// The pipe connected; change to message-read mode.

	
	BOOL bSuccess ;
	DWORD cbRead;
	//////////////////////////////////////////////////////////////////////////
	// Read header
	//////////////////////////////////////////////////////////////////////////
	{
		char *p1,*p2;
		char header[1024];
		int len=0;
		char c=1;

		header[sizeof(header)-1]=0;

		do
		{

			// Read from the pipe.
			bSuccess = ReadFile(
				g_hPipe,    // pipe handle
				&c,    // buffer to receive reply
				1,      // size of buffer
				&cbRead,  // number of bytes read
				NULL);    // not overlapped

			if (! bSuccess && GetLastError() != ERROR_MORE_DATA)
			{
				MessageBox(NULL,"ReadFile failed","FAILED",MB_OK);
				return -1; // stop only on failure
			}
			header[len++]=c;
		} while (c && len<=sizeof(header)-1);
		header[len]=0;

		
		// Set title
		{
			p1 = strstr(header,"TITLE:");
			if (p1)
			{
				p1 += 6;
				while (*p1==' ' || *p1=='\t')
					p1++;
			}
			p2=p1;
			while (*p2 && *p2!='\r' && *p2!='\n')
				p2++;
			c=*p2;
			*p2=0;
			SetConsoleTitle(p1);
			*p2=c;
		}

		// Set buffer-size
		{
			COORD coord;
			coord.X=coord.Y=0;
			p1 = strstr(header,"BUFFER-SIZE:");
			if (p1)
			{
				p1 += 12;
				while (*p1==' ' || *p1=='\t')
					p1++;
			
				p2=p1;
				while (*p2 && *p2!=',' && *p2!='x')
					p2++;
				c=*p2;
				*p2=0;
				coord.X= (SHORT)atoi(p1);
				*p2=c;
				p1=p2+1;
				while (*p2 && *p2!='\r' && *p2!='\n')
					p2++;
				c=*p2;
				*p2=0;
				coord.Y = (SHORT)atoi(p1);
				*p2=c;
				
				if (coord.X && coord.Y)
					SetConsoleScreenBufferSize( GetStdHandle(STD_OUTPUT_HANDLE), coord);
			}
			
		}

		// Is it "extended" console ? (with extra-functionality , such as cls,colors,...)
		{
			if (strstr(header,"Extended-Console: TRUE"))
				g_bExtendedConsole=TRUE;
		}


	}


	//////////////////////////////////////////////////////////////////////////
	// Get console static information
	//////////////////////////////////////////////////////////////////////////
	g_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (!GetConsoleScreenBufferInfo(g_hConsole,&g_ConsoleBufferInfo))
	{
		MessageBox(NULL,"GetConsoleScreenBufferInfo failed","FAILURE",MB_OK);
		return -3;
	}
	g_dwConsoleCoords = g_ConsoleBufferInfo.dwSize;
	g_dwConsoleSize = g_ConsoleBufferInfo.dwSize.X * g_ConsoleBufferInfo.dwSize.Y;





  ConsoleLoop();
	CloseHandle(g_hPipe);


	//printf("\r\nPress any key to end...\r\n");
	//getchar();
	printf("\r\nConsoleHelper Ended\r\n");
	return 0;
}

