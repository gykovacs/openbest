#include <time.h>
#include <stdio.h>
#include <stdarg.h>

#include "openbest-ds/printOut.h"

int tprintf(const char* fmt, ...)
{
    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    va_list arg;
    va_start(arg, fmt);
    printf("%02d:%02d:%02d ", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    vprintf(fmt, arg);
    va_end(arg);

    fflush(stdout);

    return 0;
}

int eprintf(const char* fmt, ...)
{
    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    va_list arg;
    va_start(arg, fmt);
    fprintf(stderr, "%02d:%02d:%02d ERROR: ", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    vprintf(fmt, arg);
    va_end(arg);

    printf("\n");
    fflush(stdout);

    return 0;
}

int wprintf(const char* fmt, ...)
{
    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    va_list arg;
    va_start(arg, fmt);
    fprintf(stderr, "%02d:%02d:%02d WARNING: ", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    vprintf(fmt, arg);
    va_end(arg);

    printf("\n");
    fflush(stdout);

    return 0;
}

void newline()
{
  printf("\n");
}

void enewline()
{
  fprintf(stderr, "\n");
}

void dea(){    printf("a"); fflush(stdout); }
void deb(){    printf("b"); fflush(stdout); }
void dec(){    printf("c"); fflush(stdout); }
void ded(){    printf("d"); fflush(stdout); }
void dee(){    printf("e"); fflush(stdout); }
void def(){    printf("f"); fflush(stdout); }
void deg(){    printf("g"); fflush(stdout); }
void deh(){    printf("h"); fflush(stdout); }
void dei(){    printf("i"); fflush(stdout); }
void dej(){    printf("j"); fflush(stdout); }
void dek(){    printf("k"); fflush(stdout); }
