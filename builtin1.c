#include "shell.h"

/**
 * _myHistory - displays the history CommandInfo, one command by line, preceded
 *              with line numbers, starting at 0.
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int _myHistory(CommandInfo *info)
{
	printList(info->hist);
	return (0);
}

/**
 * unset_alias - sets alias to string
 * @info: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int unset_alias(CommandInfo *info, char *str)
{
	char *p, c;
	int ret;

	p = _strChracter(str, '=');
	if (!p)
		return (1);
	c = *p;
	*p = 0;
	ret = delNode_index(&(info->alias),
		node_index(info->alias, nodeStarts_with(info->alias, str, -1)));
	*p = c;
	return (ret);
}

/**
 * set_alias - sets alias to string
 * @info: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int set_alias(CommandInfo *info, char *str)
{
	char *p;

	p = _strChracter(str, '=');
	if (!p)
		return (1);
	if (!*++p)
		return (unset_alias(info, str));

	unset_alias(info, str);
	return (addNode_end(&(info->alias), str, 0) == NULL);
}

/**
 * print_alias - prints an alias string
 * @node: the alias node
 *
 * Return: Always 0 on success, 1 on error
 */
int print_alias(LIST *node)
{
	char *p = NULL, *a = NULL;

	if (node)
	{
		p = _strChracter(node->str, '=');
		for (a = node->str; a <= p; a++)
			_putchar(*a);
		_putchar('\'');
		_puts(p + 1);
		_puts("'\n");
		return (0);
	}
	return (1);
}

/**
 * _myAlias - mimics the alias builtin (man alias)
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int _myAlias(CommandInfo *info)
{
	int i = 0;
	char *p = NULL;
	LIST *node = NULL;

	if (info->argc == 1)
	{
		node = info->alias;
		while (node)
		{
			print_alias(node);
			node = node->next;
		}
		return (0);
	}
	for (i = 1; info->argv[i]; i++)
	{
		p = _strChracter(info->argv[i], '=');
		if (p)
			set_alias(info, info->argv[i]);
		else
			print_alias(nodeStarts_with(info->alias, info->argv[i], '='));
	}

	return (0);
}
