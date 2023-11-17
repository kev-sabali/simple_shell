#include "shell.h"

/**
 * clearInfo - initializes CommandInfo struct
 * @info: struct address
 */
void clearInfo(CommandInfo *info)
{
	info->arg = NULL;
	info->argv = NULL;
	info->path = NULL;
	info->argc = 0;
}

/**
 * setInfo - initializes CommandInfo struct
 * @info: struct address
 * @av: argument vector
 */
void setInfo(CommandInfo *info, char **av)
{
	int i = 0;

	info->fileName = av[0];
	if (info->arg)
	{
		info->argv = strtow(info->arg, " \t");
		if (!info->argv)
		{

			info->argv = malloc(sizeof(char *) * 2);
			if (info->argv)
			{
				info->argv[0] = _strdup(info->arg);
				info->argv[1] = NULL;
			}
		}
		for (i = 0; info->argv && info->argv[i]; i++)
			;
		info->argc = i;

		r_alias(info);
		r_variable(info);
	}
}

/**
 * freeInfo - frees CommandInfo struct fields
 * @info: struct address
 * @all: true if freeing all fields
 */
void freeInfo(CommandInfo *info, int all)
{
	ffree(info->argv);
	info->argv = NULL;
	info->path = NULL;
	if (all)
	{
		if (!info->cmdBuffer)
			free(info->arg);
		if (info->env)
			freeList(&(info->env));
		if (info->hist)
			freeList(&(info->hist));
		if (info->alias)
			freeList(&(info->alias));
		ffree(info->environ);
			info->environ = NULL;
		bfree((void **)info->cmdBuffer);
		if (info->readfd > 2)
			close(info->readfd);
		_putchar(BUF_FLUSH);
	}
}
