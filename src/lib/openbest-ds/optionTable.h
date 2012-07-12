#ifndef _OPTION_TABLE_H_
#define _OPTION_TABLE_H_

#include <stdio.h>
#include <string.h>

typedef enum
{
    OPTION_INT,
    OPTION_FLOAT,
    OPTION_CHAR,
    OPTION_DOUBLE,
    OPTION_BOOL,
    OPTION_HELP,
    OPTION_SEPARATOR,
    OPTION_LONGINT,
} optionType;

/**
  *Option class specify a possible option in the command line argument vector.
  */
typedef struct opt
{
    /**
      * pointer to the argument
      */
    char* p;

    /**
      * number of parameters following the switch in the command line
      */
    unsigned int n;

    /**
      * name of the option
      */
    char name[40];

    /**
      * type of the option
      */
    optionType optiontype;

    /**
      * description of the option
      */
    char description[200];
    
    struct opt* next;
} option;

/**
  * OptionTable class to cover a set of Options.
  */
typedef struct
{
    /**
      * prefix text to print out with the option table
      */
    char prefix[200];

    /**
      * postfix text to print out with the option table
      */
    char postfix[200];
    
		
    char usages[1000];
    
    option* optionList;
} optionTable;

option* createOption(char* name, optionType option, char* p, unsigned int n, char* description);

void displayO(option* o);

optionTable* createOptionTable();

void addOption(optionTable* ot, char* name, optionType option, char* p, unsigned int n, char* description);

int processArgs(optionTable* ot, int* argc, char** argv);

void displayOT(optionTable* o);

void displayOptionsReverse(option* o);

void setPrefix(optionTable* o, char* prefix);

void setPostfix(optionTable* o, char* postfix);

void addUsage(optionTable* o, char* usage);

void destroyOptionTable(optionTable* ot);

#endif
