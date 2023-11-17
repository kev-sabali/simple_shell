#include "shell.h"

/**
 * hsh - main shell loop
 * @info: the parameter & return info struct
 * @av: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int hsh(CommandInfo *info, char **av)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	while (r != -1 && builtin_ret != -2)
	{
		clearInfo(info);
		if (interactive(info))
			_puts("$ ");
		_eputchar(BUF_FLUSH);
		r = get_inputs(info);
		if (r != -1)
		{
			setInfo(info, av);
			builtin_ret = findCommand(info);
			if (builtin_ret == -1)
				find_cmd(info);
		}
		else if (interactive(info))
			_putchar('\n');
		freeInfo(info, 0);
	}
	writeHist(info);
	freeInfo(info, 1);
	if (!interactive(info) && info->stat)
		exit(info->stat);
	if (builtin_ret == -2)
	{
		if (info->errorNum == -1)
			exit(info->stat);
		exit(info->errorNum);
	}
	return (builtin_ret);
}

/**
 * findCommand - finds a builtin command
 * @info: the parameter & return info struct
 *
 * Return: -1 if builtin not found,
 *			0 if builtin executed successfully,
 *			1 if builtin found but not successful,
 *			-2 if builtin signals exit()
 */
int findCommand(CommandInfo *info)
{
	int i, built_in_ret = -1;
	builtinTab builtintbl[] = {
		{"exit", _myexit},
		{"env", _myenv},
		{"help", _myhelp},
		{"history", _myHistory},
		{"setenv", _mysetenv},
		{"unsetenv", _myunsetenv},
		{"cd", _mycd},
		{"alias", _myAlias},
		{NULL, NULL}
	};

	for (i = 0; builtintbl[i].type; i++)
		if (_strcmp(info->argv[0], builtintbl[i].type) == 0)
		{
			info->lineCount++;
			built_in_ret = builtintbl[i].func(info);
			break;
		}
	return (built_in_ret);
}

/**
 * find_cmd - finds a command in PATH
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void find_cmd(CommandInfo *info)
{
	char *path = NULL;
	int i, k;

	info->path = info->argv[0];
	if (info->lineCount_flg == 1)
	{
		info->lineCount++;
		info->lineCount_flg = 0;
	}
	for (i = 0, k = 0; info->arg[i]; i++)
		if (!is_delim(info->arg[i], " \t\n"))
			k++;
	if (!k)
		return;

	path = find_path(info, _getenv(info, "PATH="), info->argv[0]);
	if (path)
	{
		info->path = path;
		forkCmd(info);
	}
	else
	{
		if ((interactive(info) || _getenv(info, "PATH=")
			|| info->argv[0][0] == '/') && is_cmd(info, info->argv[0]))
			forkCmd(info);
		else if (*(info->arg) != '\n')
		{
			info->stat = 127;
			prntError(info, "not found\n");
		}
	}
}

/**
 * forkCmd - forks a an exec thread to run cmd
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void forkCmd(CommandInfo *info)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		/* TODO: PUT ERROR FUNCTION */
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(info->path, info->argv, get_environ(info)) == -1)
		{
			freeInfo(info, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}
	else
	{
		wait(&(info->stat));
		if (WIFEXITED(info->stat))
		{
			info->stat = WEXITSTATUS(info->stat);
			if (info->stat == 126)
				prntError(info, "Permission denied\n");
		}
	}
}
