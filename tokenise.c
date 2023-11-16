#include "shell.h"

/* Function: tokenise
 * -------------------
 * Tokenizes the input line in the ARWEAVE structure based on the specified delimiters.
 * Allocates memory for an array of tokens and updates the 'tokens' and 'cmdName' fields in the ARWEAVE structure.
 *
 * Parameters:
 *   - data: Pointer to the ARWEAVE structure containing information about the program state.
 *
 * Note:
 *   - The function uses space and tab characters (' ' and '\t') as delimiters.
 *   - The 'tokens' array is terminated with a NULL pointer.
 *   - Memory is allocated for each token and the cmdName field.
 */
void tokenise(ARWEAVE *data)
{
	char *delimiter = " \t";
	int i, j, counter = 2, length;

	length = strLength(data->inputLine);
	if (length)
	{
		if (data->inputLine[length - 1] == '\n')
			data->inputLine[length - 1] = '\0';
	}

	for (i = 0; data->inputLine[i]; i++)
	{
		for (j = 0; delimiter[j]; j++)
		{
			if (data->inputLine[i] == delimiter[j])
				counter++;
		}
	}

	data->tokens = malloc(counter * sizeof(char *));
	if (data->tokens == NULL)
	{
		perror(data->prgName);
		exit(errno);
	}
	i = 0;
	data->tokens[i] = strDuplicate(_strtok(data->inputLine, delimiter));
	data->cmdName = strDuplicate(data->tokens[0]);
	while (data->tokens[i++])
	{
		data->tokens[i] = strDuplicate(_strtok(NULL, delimiter));
	}
}

