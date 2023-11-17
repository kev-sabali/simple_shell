#include "shell.h"

/**
 * get_environ - returns the string array copy of our environ
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
char **get_environ(CommandInfo *info)
{
	if (!info->environ || info->environChanged)
	{
		info->environ = listTo_strn(info->env);
		info->environChanged = 0;
	}

	return (info->environ);
}

/**
 * _unsetEnviron - Remove an environment variable
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: 1 on delete, 0 otherwise
 * @var: the string env var property
 */
int _unsetEnviron(CommandInfo *info, char *var)
{
	LIST *node = info->env;
	size_t i = 0;
	char *p;

	if (!node || !var)
		return (0);

	while (node)
	{
		p = beginsWith(node->str, var);
		if (p && *p == '=')
		{
			info->environChanged = delNode_index(&(info->env), i);
			i = 0;
			node = info->env;
			continue;
		}
		node = node->next;
		i++;
	}
	return (info->environChanged);
}

/**
 * _setEnviron - Initialize a new environment variable,
 *             or modify an existing one
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @var: the string env var property
 * @value: the string env var value
 *  Return: Always 0
 */
int _setEnviron(CommandInfo *info, char *var, char *value)
{
	char *buf = NULL;
	LIST *node;
	char *p;

	if (!var || !value)
		return (0);

	buf = malloc(_strlen(var) + _strlen(value) + 2);
	if (!buf)
		return (1);
	_strCopy(buf, var);
	_strConcat(buf, "=");
	_strConcat(buf, value);
	node = info->env;
	while (node)
	{
		p = beginsWith(node->str, var);
		if (p && *p == '=')
		{
			free(node->str);
			node->str = buf;
			info->environChanged = 1;
			return (0);
		}
		node = node->next;
	}
	addNode_end(&(info->env), buf, 0);
	free(buf);
	info->environChanged = 1;
	return (0);
}
