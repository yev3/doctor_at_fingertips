// ConsoleLogger_Demo.cpp : Defines the entry point for the console application.
//

#include "ConsoleLogger.h"
#include <stdio.h>

int main(int argc, char* argv[])
{
	printf ("Hi , i'm the main-process , and  i'm going to show you 2 more consoles...");
  LoggerInit();
  LoggerPrintf("Hi");

	getchar();
	
	
	return 0;
}

