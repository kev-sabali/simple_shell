#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>


/*Macros*/

#define PROMPT_MSG "sabali-_- "
#define UNUSED __attribute__((unused))
#define BUFFER_SIZE 1024

/* HELP BUILT IN */
#define HELP_CD_MSG "cd command help message"
#define HELP_EXIT_MSG "exit command mesage"
#define HELP_ENV_MSG "help environment"
#define HELP_SETENV_MSG "add environment variable"
#define HELP_UNSETENV_MSG "delete variable from environment"
#define HELP_MSG "gives detailed help on commands"


/************* STRUCTURES **************/

/* Struct: ARWEAVE
 * ---------------
 * Represents the data structure containing information related to the shell.
 *
 * Members:
 *   - prgName:    Name of the program (argv[0]).
 *   - inputLine:  User input line.
 *   - cmdName:    Command name extracted from user input.
 *   - xCounter:   Counter for the number of shell commands executed.
 *   - fileDesc:   File descriptor for input source (stdin or file).
 *   - tokens:     Array of command tokens.
 *   - env:        Array of environment variables.
 *   - aliasList:  Array of aliases.
 */
typedef struct info
{
	char *prgName;
	char *inputLine;
	char *cmdName;
	int xCounter;
	int fileDesc;
	char **tokens;
	char **env;
	char **aliasList;
} ARWEAVE;


typedef struct command
{
	char *builtin;
	int (*function)(ARWEAVE *data);
} command;


/**FUNCTIONS STARTS HERE**/


/*========  shell.c  ========*/

void iniData(ARWEAVE *data, int arc, char *argv[], char **env);
void loopInfinite(char *prompt, ARWEAVE *data);
void hnldControl(int opr UNUSED);

/*========  _getline.c  ========*/

int _getline(ARWEAVE *data);
int checkLogic(char *array_commands[], int i, char array_operators[]);

/*======== expansions.c ========*/

void expVar(ARWEAVE *data);
void expAlias(ARWEAVE *data);
int buff_Add(char *buffer, char *str_to_add);

/*======== str_tok.c ========*/

void tokenise(ARWEAVE *data);
char *_strtok(char *line, char *delim);

/*======== exe.c ========*/

int _exe(ARWEAVE *data);

/*======== builtins_list.c ========*/

int cmdList(ARWEAVE *data);

/*======== find_in_path.c ========*/

char **tokenPath(ARWEAVE *data);
int identifyProgram(ARWEAVE *data);

/*****MEMORY MANAGEMENT******/

/*======== helpers_free.c ========*/

void freeArray_p(char **directories);
void freeData(ARWEAVE *data);
void freeData_all(ARWEAVE *data);


/**** COMMANDS ****/

/*======== builtins_more.c ========*/

int cmdExit(ARWEAVE *data);
int cmdCd(ARWEAVE *data);
int setWork_dir(ARWEAVE *data, char *new_dir);
int cmdHelp(ARWEAVE *data);
int cmdAlias(ARWEAVE *data);

/*======== builtins_env.c ========*/

int cmdEnv(ARWEAVE *data);
int cmdSet_env(ARWEAVE *data);
int cmdUnset_env(ARWEAVE *data);

/********* ENVIRON VARIABLES MANAGEMENT *******/

/*======== env_management.c ========*/

char *environKey(char *name, ARWEAVE *data);
int environSet_key(char *key, char *value, ARWEAVE *data);
int environRemove_key(char *key, ARWEAVE *data);
void prntEnviron(ARWEAVE *data);


/****** PRINTING **************/

/*======== helpers_print.c ========*/

int _puts(char *string);
int _putchar(char *string);
int _printErr(int errorcode, ARWEAVE *data);


/******* STRINGS MANAGEMENT ********/

/*======== helpers_string.c ========*/

int strLength(char *string);
char *strDuplicate(char *string);
int strComp(char *string1, char *string2, int number);
char *strConcat(char *string1, char *string2);
void strVerse(char *string);

/*======== helpers_numbers.c ========*/

void lngString(long number, char *string, int base);
int _atoi(char *s);
int countChar(char *string, char *character);

/*======== alias_management.c ========*/

int prntAlias(ARWEAVE *data, char *alias);
char *getAlias(ARWEAVE *data, char *alias);
int setAlias(char *alias_string, ARWEAVE *data);


#endif

