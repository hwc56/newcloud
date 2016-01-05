#ifndef __LOGGING_H__
#define  __LOGGING_H__
#ifdef  __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <syslog.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>

//#define     LOG_FILE_SIZE   10*1024*1024

    void LOG(const char* ms, ... );
#ifdef  __cplusplus
}
#endif

#endif
