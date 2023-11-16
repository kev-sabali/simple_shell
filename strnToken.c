#include "shell.h"

/* Function: _strtok
 * -----------------
 * Tokenizes a string using the specified delimiter. This function is a reentrant
 * version of strtok.
 *
 * Parameters:
 *   - line:  String to be tokenized (or NULL to continue tokenizing the last string).
 *   - delim: Delimiter characters.
 *
 * Returns:
 *   - A pointer to the next token in the string, or NULL if no more tokens are found.
 *
 * Note:
 *   - The function uses a static variable 'str' to keep track of the current position in the string.
 *   - Subsequent calls with NULL as the 'line' parameter continue tokenizing the same string.
 */
char *_strtok(char *line, char *delim)
{
	int j;
	static char *str;
	char *copystr;

	if (line != NULL)
		str = line;
	for (; *str != '\0'; str++)
	{
		for (j = 0; delim[j] != '\0'; j++)
		{
			if (*str == delim[j])
			break;
		}
		if (delim[j] == '\0')
			break;
	}
	copystr = str;
	if (*copystr == '\0')
		return (NULL);
	for (; *str != '\0'; str++)
	{
		for (j = 0; delim[j] != '\0'; j++)
		{
			if (*str == delim[j])
			{
				*str = '\0';
				str++;
				return (copystr);
			}
		}
	}
	return (copystr);
}

