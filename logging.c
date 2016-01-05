#include <stdio.h>
#include <syslog.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include "logging.h"

//#define     LOG_FILE_SIZE   10*1024*1024

    void LOG(const char* ms, ... )
    {
        char wzLog[1024] = {0};
        char buffer[1024] = {0};
        va_list args;
        va_start(args, ms);
        vsprintf( wzLog ,ms,args);
        va_end(args);

        time_t now;
        time(&now);
        struct tm *local;
        local = localtime(&now);
        //printf("%04d-%02d-%02d %02d:%02d:%02d %s\n", local->tm_year+1900, local->tm_mon,
             //   local->tm_mday, local->tm_hour, local->tm_min, local->tm_sec,
               // wzLog);
        sprintf(buffer,"%04d-%02d-%02d %02d:%02d:%02d %s\n", local->tm_year+1900, local->tm_mon,
                local->tm_mday, local->tm_hour, local->tm_min, local->tm_sec,
                wzLog);
        //unsigned long  file_size = get_file_size("testResult.log");
        //if(file_size > LOG_FILE_SIZE  )
        //    ClearLog();

        FILE* file = fopen("sunclound.log","a+");
        fwrite(buffer,1,strlen(buffer),file);
        fclose(file);

        //syslog(LOG_INFO,wzLog);
        return ;
    }

    /*
    unsigned long get_file_size(const char *path)
    {
        unsigned long filesize = -1;    
        struct stat statbuff;
        if(stat(path, &statbuff) < 0){
            return filesize;
        }else{
            filesize = statbuff.st_size;
        }
        return filesize;
    }
    void    ClearLog()
    {
        system("cat  /dev/null > testResut.log");
    }
    */


