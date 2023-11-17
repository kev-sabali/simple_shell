#include "shell.h"

/**
 * isChain - test if current char in buffer is a chain delimeter
 * @info: the parameter struct
 * @buf: the char buffer
 * @p: address of current position in buf
 *
 * Return: 1 if chain delimeter, 0 otherwise
 */
int isChain(CommandInfo *info, char *buf, size_t *p)
{
	size_t j = *p;

	if (buf[j] == '|' && buf[j + 1] == '|')
	{
		buf[j] = 0;
		j++;
		info->cmdBuffer_type = CMD_OR;
	}
	else if (buf[j] == '&' && buf[j + 1] == '&')
	{
		buf[j] = 0;
		j++;
		info->cmdBuffer_type = CMD_AND;
	}
	else if (buf[j] == ';') /* found end of this command */
	{
		buf[j] = 0; /* replace semicolon with null */
		info->cmdBuffer_type = CMD_CHAIN;
	}
	else
		return (0);
	*p = j;
	return (1);
}

/**
 * checkChain - checks we should continue chaining based on last status
 * @info: the parameter struct
 * @buf: the char buffer
 * @p: address of current position in buf
 * @i: starting position in buf
 * @len: length of buf
 *
 * Return: Void
 */
void checkChain(CommandInfo *info, char *buf, size_t *p, size_t i, size_t len)
{
	size_t j = *p;

	if (info->cmdBuffer_type == CMD_AND)
	{
		if (info->stat)
		{
			buf[i] = 0;
			j = len;
		}
	}
	if (info->cmdBuffer_type == CMD_OR)
	{
		if (!info->stat)
		{
			buf[i] = 0;
			j = len;
		}
	}

	*p = j;
}

/**
 * r_alias - replaces an aliases in the tokenized string
 * @info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int r_alias(CommandInfo *info)
{
	int i;
	LIST *node;
	char *p;

	for (i = 0; i < 10; i++)
	{
		node = nodeStarts_with(info->alias, info->argv[0], '=');
		if (!node)
			return (0);
		free(info->argv[0]);
		p = _strChracter(node->str, '=');
		if (!p)
			return (0);
		p = _strdup(p + 1);
		if (!p)
			return (0);
		info->argv[0] = p;
	}
	return (1);
}

/**
 * r_variable - replaces vars in the tokenized string
 * @info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int r_variable(CommandInfo *info)
{
	int i = 0;
	LIST *node;

	for (i = 0; info->argv[i]; i++)
	{
		if (info->argv[i][0] != '$' || !info->argv[i][1])
			continue;

		if (!_strcmp(info->argv[i], "$?"))
		{
			replaceString(&(info->argv[i]),
				_strdup(convtNum(info->stat, 10, 0)));
			continue;
		}
		if (!_strcmp(info->argv[i], "$$"))
		{
			replaceString(&(info->argv[i]),
				_strdup(convtNum(getpid(), 10, 0)));
			continue;
		}
		node = nodeStarts_with(info->env, &info->argv[i][1], '=');
		if (node)
		{
			replaceString(&(info->argv[i]),
				_strdup(_strChracter(node->str, '=') + 1));
			continue;
		}
		replaceString(&info->argv[i], _strdup(""));

	}
	return (0);
}

/**
 * replaceString - replaces string
 * @old: address of old string
 * @new: new string
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replaceString(char **old, char *new)
{
	free(*old);
	*old = new;
	return (1);
}
