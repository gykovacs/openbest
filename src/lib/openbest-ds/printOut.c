#include <time.h>
#include <stdio.h>
#include <stdarg.h>

#include "openbest-ds/printOut.h"
#include "openbest-ds/config-ds.h"

int tprintf(const char* fmt, ...)
{
    if ( printOut )
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
    }

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

void printArrayI(char* name, int* a, int n)
{
    printf("%s %d:\n", name, n);
    int i;
    for ( i= 0; i < n; ++i )
        printf("%d ", a[i]);
    printf("\n");
}

void printArray2I(char* name, int* a, int n1, int n2)
{
    printf("%s %dx%d:\n", name, n1, n2);
    int i, j;
    for ( i= 0; i < n1; ++i )
    {
        for ( j= 0; j < n2; ++j )
            printf("%d ", a[i*n2 + j]);
        printf("\n");
    }
}

void printArrayB(char* name, bool* a, int n)
{
    printf("%s %d:\n", name, n);
    int i;
    for ( i= 0; i < n; ++i )
        printf("%d ", a[i]);
    printf("\n");
}

void printArray2B(char* name, bool* a, int n1, int n2)
{
    printf("%s %dx%d:\n", name, n1, n2);
    int i, j;
    for ( i= 0; i < n1; ++i )
    {
        for ( j= 0; j < n2; ++j )
            printf("%d ", a[i*n2 + j]);
        printf("\n");
    }
}

void printArrayFloat(char* name, float* a, int n)
{
    printf("%s %d:\n", name, n);
    int i;
    for ( i= 0; i < n; ++i )
        printf("%f ", a[i]);
    printf("\n");
}

void printArray2Float(char* name, float* a, int n1, int n2)
{
    printf("%s %dx%d:\n", name, n1, n2);
    int i, j;
    for ( i= 0; i < n1; ++i )
    {
        for ( j= 0; j < n2; ++j )
            printf("%f ", a[i*n2 + j]);
        printf("\n");
    }
}

void printArrayD(char* name, double* a, int n)
{
    printf("%s %d:\n", name, n);
    int i;
    for ( i= 0; i < n; ++i )
        printf("%f ", a[i]);
    printf("\n");
}

void printArray2D(char* name, double* a, int n1, int n2)
{
    printf("%s %dx%d:\n", name, n1, n2);
    int i, j;
    for ( i= 0; i < n1; ++i )
    {
        for ( j= 0; j < n2; ++j )
            printf("%f ", a[i*n2 + j]);
        printf("\n");
    }
}

void printArrayR(char* name, real* a, int n)
{
    printf("%s %d:\n", name, n);
    int i;
    for ( i= 0; i < n; ++i )
        printf("%f ", a[i]);
    printf("\n");
}

void printArray2R(char* name, real* a, int n1, int n2)
{
    printf("%s %dx%d:\n", name, n1, n2);
    int i, j;
    for ( i= 0; i < n1; ++i )
    {
        for ( j= 0; j < n2; ++j )
            printf("%f ", a[i*n2 + j]);
        printf("\n");
    }
}

void printArrayIFile(char* name, int* a, int n)
{
    FILE* f= fopen(name, "w");

    //fprintf(f, "%s %d:\n", name, n);
    int i;
    for ( i= 0; i < n; ++i )
        fprintf(f, "%d ", a[i]);
    fprintf(f, "\n");

    fclose(f);
}

void printArray2IFile(char* name, int* a, int n1, int n2)
{
    FILE* f= fopen(name, "w");

    //fprintf(f, "%s %dx%d:\n", name, n1, n2);
    int i, j;
    for ( i= 0; i < n1; ++i )
    {
        for ( j= 0; j < n2; ++j )
            fprintf(f, "%d ", a[i*n2 + j]);
        fprintf(f, "\n");
    }
    fclose(f);
}

void printArrayBFile(char* name, bool* a, int n)
{
    FILE* f= fopen(name, "w");

    //fprintf(f, "%s %d:\n", name, n);
    int i;
    for ( i= 0; i < n; ++i )
        fprintf(f, "%d ", a[i]);
    fprintf(f, "\n");
    fclose(f);
}

void printArray2BFile(char* name, bool* a, int n1, int n2)
{
    FILE* f= fopen(name, "w");

    //fprintf(f, "%s %dx%d:\n", name, n1, n2);
    int i, j;
    for ( i= 0; i < n1; ++i )
    {
        for ( j= 0; j < n2; ++j )
            fprintf(f, "%d ", a[i*n2 + j]);
        fprintf(f, "\n");
    }
    fclose(f);
}

void printArrayFloatFile(char* name, float* a, int n)
{
    FILE* f= fopen(name, "w");

    //fprintf(f, "%s %d:\n", name, n);
    int i;
    for ( i= 0; i < n; ++i )
        fprintf(f, "%f ", a[i]);
    fprintf(f, "\n");
    fclose(f);
}

void printArray2FloatFile(char* name, float* a, int n1, int n2)
{
    FILE* f= fopen(name, "w");

    //fprintf("%s %dx%d:\n", name, n1, n2);
    int i, j;
    for ( i= 0; i < n1; ++i )
    {
        for ( j= 0; j < n2; ++j )
            fprintf(f, "%f ", a[i*n2 + j]);
        fprintf(f, "\n");
    }
    fclose(f);
}

void printArrayDFile(char* name, double* a, int n)
{
    FILE* f= fopen(name, "w");

    //fprintf(f, "%s %d:\n", name, n);
    int i;
    for ( i= 0; i < n; ++i )
        fprintf(f, "%f ", a[i]);
    fprintf(f, "\n");
    fclose(f);
}

void printArray2DFile(char* name, double* a, int n1, int n2)
{
    FILE* f= fopen(name, "w");

    //fprintf(f, "%s %dx%d:\n", name, n1, n2);
    int i, j;
    for ( i= 0; i < n1; ++i )
    {
        for ( j= 0; j < n2; ++j )
            fprintf(f, "%f ", a[i*n2 + j]);
        fprintf(f, "\n");
    }
    fclose(f);
}

void printArrayRFile(char* name, real* a, int n)
{
    FILE* f= fopen(name, "w");

    //fprintf(f, "%s %d:\n", name, n);
    int i;
    for ( i= 0; i < n; ++i )
        fprintf(f, "%f ", a[i]);
    fprintf(f, "\n");
    fclose(f);
}

void printArray2RFile(char* name, real* a, int n1, int n2)
{
    FILE* f= fopen(name, "w");

    //fprintf(f, "%s %dx%d:\n", name, n1, n2);
    int i, j;
    for ( i= 0; i < n1; ++i )
    {
        for ( j= 0; j < n2; ++j )
            fprintf(f, "%f ", a[i*n2 + j]);
        fprintf(f, "\n");
    }
    fclose(f);
}
