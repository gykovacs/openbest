#include <stdlib.h>

#include "openbest-ds/config-ds.h"
#include "openbest-ds/printOut.h"
#include "openbest-ds/optionTable.h"

option* createOption(char* name, optionType optiontype, char* p, unsigned int n, char* description)
{
    option* o= (option*)malloc(sizeof(option));
    o->p= p;
    o->n= n;
    strcpy(o->name, name);
    o->optiontype= optiontype;
    strcpy(o->description, description);
    o->next= NULL;
    return o;
}

void displayO(option* o)
{
    char tmp[strlen(o->name) + strlen(o->description) + 100];

    switch (o->optiontype)
    {
        case OPTION_INT:
            sprintf(tmp, "\t%-20s%-4d%-8s%s (default: %d)", o->name, o->n, "int", o->description, *(int*)(o->p));
            break;
        case OPTION_LONGINT:
            sprintf(tmp, "\t%-20s%-4d%-8s%s (default: %ld)", o->name, o->n, "long int", o->description, *(long int*)(o->p));
            break;
        case OPTION_FLOAT:
            sprintf(tmp, "\t%-20s%-4d%-8s%s (default: %.2f)", o->name, o->n, "float", o->description, *(float*)(o->p));
            break;
        case OPTION_DOUBLE:
            sprintf(tmp, "\t%-20s%-4d%-8s%s (default: %.2f)", o->name, o->n, "double", o->description, *(double*)(o->p));
            break;
        case OPTION_BOOL:
            sprintf(tmp, "\t%-20s%-4d%-8s%s", o->name, o->n, "bool", o->description);
            break;
        case OPTION_CHAR:
            sprintf(tmp, "\t%-20s%-4d%-8s%s (default: %s)", o->name, o->n, "char", o->description, (char*)(o->p));
            break;
        case OPTION_HELP:
            sprintf(tmp, "\n\t%s\n", o->description);
            break;
        case OPTION_SEPARATOR:
            sprintf(tmp, "\n\t%s", o->description);
    }
    
    printf("%s\n", tmp);
}

optionTable* createOptionTable()
{
    optionTable* o= (optionTable*)malloc(sizeof(optionTable));
    o->prefix[0]= '\0';
    o->postfix[0]= '\0';
    o->usages[0]= '\0';
    o->optionList= NULL;
    
    return o;
}

void addOption(optionTable* ot, char* name, optionType optiontype, char* p, unsigned int n, char* description)
{
    option* o= createOption(name, optiontype, p, n, description);
    o->next= ot->optionList;
    ot->optionList= o;
}

int processArgs(optionTable* ot, int* argc, char** argv)
{
    unsigned int i, j, k;

    for (i= 0; (int)i < *argc; ++i)
    {
        if (strcmp(argv[i], "--help") == 0)
        {
            displayOT(ot);
            return 1;
        }
    }

    option* o= ot->optionList;
    while (o != NULL)
    {
        for (i = 1; (int)i < *argc; ++i)
        {
            if (strcmp(argv[i], o->name) == 0 && o->optiontype != OPTION_SEPARATOR)
            {
                for (j = 0; j <= o->n; ++j)
                    if (i + j <= (unsigned int)(*argc))
                        switch (o->optiontype)
                        {
                            case OPTION_LONGINT:
                                if (j != 0)
                                    sscanf(argv[i + j], "%ld", ((long int *) (o->p)) + j - 1);
                                break;
                            case OPTION_INT:
                                if (j != 0)
                                    sscanf(argv[i + j], "%d", ((int *) (o->p)) + j - 1);
                                break;
                            case OPTION_FLOAT:
                                if (j != 0)
                                    sscanf(argv[i + j], "%f", ((float *) (o->p)) + j - 1);
                                break;
                            case OPTION_CHAR:
                                if (j != 0)
                                    sscanf(argv[i + j], "%s", ((char *) (o->p)) + j - 1);
                                break;
                            case OPTION_BOOL:
                                *((bool *)(o->p))= true;
                                break;
                            case OPTION_DOUBLE:
                                if (j != 0) {
                                    double d;
                                    sscanf(argv[i + j], "%lf", &d);
                                    *(((double *) (o->p) + j - 1)) = d;
                                }
                                break;
                            case OPTION_HELP:
                                break;
                            case OPTION_SEPARATOR:
                                break;
                        };

                for (k = i; k < *argc - j; ++k)
                {
                    argv[k] = argv[k + j];
                }
                *argc -= j;
                break;
            }
        }
        o= o->next;
    }
    
    for ( i= 1; i < *argc; ++i )
        if ( argv[i][0] == '-' && ( argv[i][1] == '-' || argv[i][1] < '0' || argv[i][1] > '9' )  )
        {
            tprintf("Error: unknown argument %s\n", argv[i]);
            return 1;
        }
    
    return 0;
}

void displayOT(optionTable* ot)
{
    printf("%s\n\n", ot->prefix);

    printf("Parameters (parameter name, number of arguments, type of arguments, description):\n");

    displayOptionsReverse(ot->optionList);

    printf("\n\tUsages\n");
    printf("%s\n\n", ot->usages);
    if ( strcmp(ot->postfix, "") != 0 )
      printf("\n%s\n", ot->postfix);
}

void displayOptionsReverse(option* o)
{
  if ( o->next )
    displayOptionsReverse(o->next);
  printf("%t");
  displayO(o);
}

void setPrefix(optionTable* ot, char* prefix)
{
    strcpy(ot->prefix, prefix);
}

void setPostfix(optionTable* ot, char* postfix)
{
    strcpy(ot->postfix, postfix);
}

void addUsage(optionTable* ot, char* usages)
{
    strcat(ot->usages, "\t\t");
    strcat(ot->usages, usages);
}

void destroyOptionTable(optionTable* ot)
{
    option* o;
    while ( ot->optionList != NULL )
    {
        o= ot->optionList->next;
        free(ot->optionList);
        ot->optionList= o;
    }
    free(ot);
}
