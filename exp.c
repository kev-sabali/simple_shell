#include "shell.h"

/* Function: expVar
 * ----------------
 * Expands environment variables in the input line of the ARWEAVE structure.
 * Supports expansion of special variables like $? and $$.
 *
 * Parameters:
 *   - data: Pointer to an ARWEAVE structure containing input line information.
 */

void expVar(ARWEAVE *data)
{
	int i, j;
	char line[BUFFER_SIZE] = {0}, expansion[BUFFER_SIZE] = {'\0'}, *temp;

	if (data->inputLine == NULL)
		return;
	buff_Add(line, data->inputLine);
	for (i = 0; line[i]; i++)
		if (line[i] == '#')
			line[i--] = '\0';
		else if (line[i] == '$' && line[i + 1] == '?')
		{
			line[i] = '\0';
			lngString(errno, expansion, 10);
			buff_Add(line, expansion);
			buff_Add(line, data->inputLine + i + 2);
		}
		else if (line[i] == '$' && line[i + 1] == '$')
		{
			line[i] = '\0';
			lngString(getpid(), expansion, 10);
			buff_Add(line, expansion);
			buff_Add(line, data->inputLine + i + 2);
		}
		else if (line[i] == '$' && (line[i + 1] == ' ' || line[i + 1] == '\0'))
			continue;
		else if (line[i] == '$')
		{
			for (j = 1; line[i + j] && line[i + j] != ' '; j++)
				expansion[j - 1] = line[i + j];
			temp = environKey(expansion, data);
			line[i] = '\0', expansion[0] = '\0';
			buff_Add(expansion, line + i + j);
			temp ? buff_Add(line, temp) : 1;
			buff_Add(line, expansion);
		}
	if (!strComp(data->inputLine, line, 0))
	{
		free(data->inputLine);
		data->inputLine = strDuplicate(line);
	}
}

/* Function: expAlias
 * ------------------
 * Expands aliases in the input line of the ARWEAVE structure.
 *
 * Parameters:
 *   - data: Pointer to an ARWEAVE structure containing input line information.
 */

void expAlias(ARWEAVE *data)
{
	int i, j, was_expanded = 0;
	char line[BUFFER_SIZE] = {0}, expansion[BUFFER_SIZE] = {'\0'}, *temp;

	if (data->inputLine == NULL)
		return;

	buff_Add(line, data->inputLine);

	for (i = 0; line[i]; i++)
	{
		for (j = 0; line[i + j] && line[i + j] != ' '; j++)
			expansion[j] = line[i + j];
		expansion[j] = '\0';

		temp = getAlias(data, expansion);
		if (temp)
		{
			expansion[0] = '\0';
			buff_Add(expansion, line + i + j);
			line[i] = '\0';
			buff_Add(line, temp);
			line[strLength(line)] = '\0';
			buff_Add(line, expansion);
			was_expanded = 1;
		}
		break;
	}
	if (was_expanded)
	{
		free(data->inputLine);
		data->inputLine = strDuplicate(line);
	}
}

/* Function: buff_Add
 * -------------------
 * Appends a string to the end of a buffer.
 *
 * Parameters:
 *   - buffer: The buffer to which the string is appended.
 *   - str_to_add: The string to append to the buffer.
 *
 * Returns:
 *   - Returns the length of the buffer after the addition.
 */

int buff_Add(char *buffer, char *str_to_add)
{
	int length, i;

	length = strLength(buffer);
	for (i = 0; str_to_add[i]; i++)
	{
		buffer[length + i] = str_to_add[i];
	}
	buffer[length + i] = '\0';
	return (length + i);
}

