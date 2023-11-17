#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

/* for command chaining */
#define CMD_NORM	0
#define CMD_OR		1
#define CMD_AND		2
#define CMD_CHAIN	3

/* for convert_number() */
#define CONVERT_LOWERCASE	1
#define CONVERT_UNSIGNED	2

/* 1 if using system getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HIST_FILE	".simple_shell_history"
#define HIST_MAX	4096

extern char **environ;


/**
 * struct liststr - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct liststr
{
	int num;
	char *str;
	struct liststr *next;
} LIST;

/**
 *struct passinfo - contains pseudo-arguements to pass into a function,
 *		allowing uniform prototype for function pointer struct
 *@arg: a string generated from getline containing arguements
 *@argv: an array of strings generated from arg
 *@path: a string path for the current command
 *@argc: the argument count
 *@lineCount: the error count
 *@errorNum: the error code for exit()s
 *@lineCount_flg: if on count this line of input
 *@fileName: the program filename
 *@env: linked list local copy of environ
 *@environ: custom modified copy of environ from LL env
 *@hist: the history node
 *@alias: the alias node
 *@environChanged: on if environ was changed
 *@stat: the return status of the last exec'd command
 *@cmdBuffer: address of pointer to cmd_buf, on if chaining
 *@cmdBuffer_type: CMD_type ||, &&, ;
 *@readfd: the fd from which to read line input
 *@histCount: the history line number count
 */
typedef struct passinfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int lineCount;
	int errorNum;
	int lineCount_flg;
	char *fileName;
	LIST *env;
	LIST *hist;
	LIST *alias;
	char **environ;
	int environChanged;
	int stat;

	char **cmdBuffer; /* pointer to cmd ; chain buffer, for memory mangement */
	int cmdBuffer_type; /* CMD_type ||, &&, ; */
	int readfd;
	int histCount;
} CommandInfo;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

/**
 *struct builtin - contains a builtin string and related function
 *@type: the builtin command flag
 *@func: the function
 */
typedef struct builtin
{
	char *type;
	int (*func)(CommandInfo *);
} builtinTab;


/* toem_shloop.c */
int hsh(CommandInfo *, char **);
int findCommand(CommandInfo *);
void find_cmd(CommandInfo *);
void forkCmd(CommandInfo *);

/* toem_parser.c */
int is_cmd(CommandInfo *, char *);
char *duplicateChars(char *, int, int);
char *find_path(CommandInfo *, char *, char *);

/* loophsh.c */
int loophsh(char **);

/* toem_errors.c */
void _eputs(char *);
int _eputchar(char);
int _putfd(char c, int fd);
int _putsfd(char *str, int fd);

/* toem_string.c */
int _strlen(char *);
int _strcmp(char *, char *);
char *beginsWith(const char *, const char *);
char *_strConcat(char *, char *);

/* toem_string1.c */
char *_strCopy(char *, char *);
char *_strdup(const char *);
void _puts(char *);
int _putchar(char);

/* toem_exits.c */
char *_strnCopy(char *, char *, int);
char *_strncat(char *, char *, int);
char *_strChracter(char *, char);

/* toem_tokenizer.c */
char **strtow(char *, char *);
char **strtow02(char *, char);

/* toem_realloc.c */
char *_memset(char *, char, unsigned int);
void ffree(char **);
void *_realloc(void *, unsigned int, unsigned int);

/* toem_memory.c */
int bfree(void **);

/* toem_atoi.c */
int interactive(CommandInfo *);
int is_delim(char, char *);
int _isalpha(int);
int _atoi(char *);

/* toem_errors1.c */
int _erratoi(char *);
void prntError(CommandInfo *, char *);
int print_d(int, int);
char *convtNum(long int, int, int);
void rm_Comments(char *);

/* toem_builtin.c */
int _myexit(CommandInfo *);
int _mycd(CommandInfo *);
int _myhelp(CommandInfo *);

/* toem_builtin1.c */
int _myHistory(CommandInfo *);
int _myAlias(CommandInfo *);

/*toem_getline.c */
ssize_t get_inputs(CommandInfo *);
int _getline(CommandInfo *, char **, size_t *);
void _sigintHandler(int);

/* toem_getinfo.c */
void clearInfo(CommandInfo *);
void setInfo(CommandInfo *, char **);
void freeInfo(CommandInfo *, int);

/* toem_environ.c */
char *_getenv(CommandInfo *, const char *);
int _myenv(CommandInfo *);
int _mysetenv(CommandInfo *);
int _myunsetenv(CommandInfo *);
int populate_env_list(CommandInfo *);

/* toem_getenv.c */
char **get_environ(CommandInfo *);
int _unsetEnviron(CommandInfo *, char *);
int _setEnviron(CommandInfo *, char *, char *);

/* toem_history.c */
char *getHistory_file(CommandInfo *info);
int writeHist(CommandInfo *info);
int readHist(CommandInfo *info);
int bHistory_list(CommandInfo *info, char *buf, int linecount);
int reNum_history(CommandInfo *info);

/* toem_lists.c */
LIST *addNode(LIST **, const char *, int);
LIST *addNode_end(LIST **, const char *, int);
size_t printList_str(const LIST *);
int delNode_index(LIST **, unsigned int);
void freeList(LIST **);

/* toem_lists1.c */
size_t list_len(const LIST *);
char **listTo_strn(LIST *);
size_t printList(const LIST *);
LIST *nodeStarts_with(LIST *, char *, char);
ssize_t node_index(LIST *, LIST *);

/* toem_vars.c */
int isChain(CommandInfo *, char *, size_t *);
void checkChain(CommandInfo *, char *, size_t *, size_t, size_t);
int r_alias(CommandInfo *);
int r_variable(CommandInfo *);
int replaceString(char **, char *);

#endif
