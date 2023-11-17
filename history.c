#include "shell.h"

/**
 * getHistory_file - gets the history file
 * @info: parameter struct
 *
 * Return: allocated string containg history file
 */

char *getHistory_file(CommandInfo *info)
{
	char *buf, *dir;

	dir = _getenv(info, "HOME=");
	if (!dir)
		return (NULL);
	buf = malloc(sizeof(char) * (_strlen(dir) + _strlen(HIST_FILE) + 2));
	if (!buf)
		return (NULL);
	buf[0] = 0;
	_strCopy(buf, dir);
	_strConcat(buf, "/");
	_strConcat(buf, HIST_FILE);
	return (buf);
}

/**
 * writeHist - creates a file, or appends to an existing file
 * @info: the parameter struct
 *
 * Return: 1 on success, else -1
 */
int writeHist(CommandInfo *info)
{
	ssize_t fd;
	char *filename = getHistory_file(info);
	LIST *node = NULL;

	if (!filename)
		return (-1);

	fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);
	if (fd == -1)
		return (-1);
	for (node = info->hist; node; node = node->next)
	{
		_putsfd(node->str, fd);
		_putfd('\n', fd);
	}
	_putfd(BUF_FLUSH, fd);
	close(fd);
	return (1);
}

/**
 * readHist - reads history from file
 * @info: the parameter struct
 *
 * Return: histCount on success, 0 otherwise
 */
int readHist(CommandInfo *info)
{
	int i, last = 0, linecount = 0;
	ssize_t fd, rdlen, fsize = 0;
	struct stat st;
	char *buf = NULL, *filename = getHistory_file(info);

	if (!filename)
		return (0);

	fd = open(filename, O_RDONLY);
	free(filename);
	if (fd == -1)
		return (0);
	if (!fstat(fd, &st))
		fsize = st.st_size;
	if (fsize < 2)
		return (0);
	buf = malloc(sizeof(char) * (fsize + 1));
	if (!buf)
		return (0);
	rdlen = read(fd, buf, fsize);
	buf[fsize] = 0;
	if (rdlen <= 0)
		return (free(buf), 0);
	close(fd);
	for (i = 0; i < fsize; i++)
		if (buf[i] == '\n')
		{
			buf[i] = 0;
			bHistory_list(info, buf + last, linecount++);
			last = i + 1;
		}
	if (last != i)
		bHistory_list(info, buf + last, linecount++);
	free(buf);
	info->histCount = linecount;
	while (info->histCount-- >= HIST_MAX)
		delNode_index(&(info->hist), 0);
	reNum_history(info);
	return (info->histCount);
}

/**
 * bHistory_list - adds entry to a history linked list
 * @info: Structure containing potential arguments. Used to maintain
 * @buf: buffer
 * @linecount: the history linecount, histCount
 *
 * Return: Always 0
 */
int bHistory_list(CommandInfo *info, char *buf, int linecount)
{
	LIST *node = NULL;

	if (info->hist)
		node = info->hist;
	addNode_end(&node, buf, linecount);

	if (!info->hist)
		info->hist = node;
	return (0);
}

/**
 * reNum_history - renumbers the history linked list after changes
 * @info: Structure containing potential arguments. Used to maintain
 *
 * Return: the new histCount
 */
int reNum_history(CommandInfo *info)
{
	LIST *node = info->hist;
	int i = 0;

	while (node)
	{
		node->num = i++;
		node = node->next;
	}
	return (info->histCount = i);
}
