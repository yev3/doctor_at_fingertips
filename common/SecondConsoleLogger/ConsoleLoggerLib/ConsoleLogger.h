////////////////////////////////////////////////////////////////////////////////
// Opens up a second console that you can printf to
////////////////////////////////////////////////////////////////////////////////
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif


#define DEFAULT_HELPER_EXE	"ConsoleLoggerHelper.exe"
#define DEFAULT_WIND_TITLE	"ConsoleLoggerHelper"

long LoggerInit();
long LoggerInitWindow(const char *newWindowTitle,
                      const char *helperExecutableName);
int LoggerPrintf(const char *format, ...);
int LoggerWrite(const char* buf, int size);

#ifdef __cplusplus
}
#endif
