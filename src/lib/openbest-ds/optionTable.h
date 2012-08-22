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

/**
  * displays the parameter option instance
  * @param o option distance to display
  */
void displayO(option* o);

/**
  * creates a new option table instance
  * @returns pointer to a new optionTable instance
  */
optionTable* createOptionTable();

/**
  * adds a new option to an option table
  * @param ot optionTable to add a new option to
  * @param name name of the new option
  * @param option type of the new option
  * @param p pointer to the option variable
  * @param n number of elements to read to p
  * @param description description of the option
  */
void addOption(optionTable* ot, char* name, optionType option, char* p, unsigned int n, char* description);

/**
  * processes the command line arguments using the parameter option table
  * @param ot optionTable instance
  * @param argc number of command line arguments
  * @param argv command line arguments
  * @returns 0 if the processing is successful, non-0 otherwise
  */
int processArgs(optionTable* ot, int* argc, char** argv);

/**
  * displays the properties of the parameter optionTable instance
  * @param o optionTable instance to display
  */
void displayOT(optionTable* o);

/**
  * displays the list of options in reverse order of storing
  * @param o head of the list of option objects to display
  */
void displayOptionsReverse(option* o);

/**
  * sets a prefix to the option table
  * @param o optionTable instance
  * @param prefix prefix to add to o
  */
void setPrefix(optionTable* o, char* prefix);

/**
  * sets a postfix to the option table
  * @param o optionTable instance
  * @param postfix postfix to add to o
  */
void setPostfix(optionTable* o, char* postfix);

/**
  * adds usages to the option table
  * @param o optionTable instance
  * @param usage usages to add to o
  */
void addUsage(optionTable* o, char* usage);

/**
  * destroys the parameter optionTable instance
  * @param ot optionTable to destroy
  */
void destroyOptionTable(optionTable* ot);

#endif
