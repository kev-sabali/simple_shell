#include "shell.h"

/* Function: freeData
 * -------------------
 * Frees the memory allocated for the ARWEAVE structure's tokens, inputLine, and cmdName.
 * Resets the corresponding pointers to NULL.
 *
 * Parameters:
 *   - data: Pointer to an ARWEAVE structure.
 */

void freeData(ARWEAVE *data)
{
	if (data->tokens)
		freeArray_p(data->tokens);
	if (data->inputLine)
		free(data->inputLine);
	if (data->cmdName)
		free(data->cmdName);

	data->inputLine = NULL;
	data->cmdName = NULL;
	data->tokens = NULL;
}

/* Function: freeData_all
 * -----------------------
 * Frees the memory allocated for the ARWEAVE structure's tokens, inputLine, cmdName,
 * env, and aliasList. Also, closes the file descriptor if it's not the default (0).
 *
 * Parameters:
 *   - data: Pointer to an ARWEAVE structure.
 */

void freeData_all(ARWEAVE *data)
{
	if (data->fileDesc != 0)
	{
		if (close(data->fileDesc))
			perror(data->prgName);
	}
	freeData(data);
	freeArray_p(data->env);
	freeArray_p(data->aliasList);
}

/* Function: freeArray_p
 * ----------------------
 * Frees the memory allocated for a dynamically allocated string array.
 * Resets the array pointer to NULL.
 *
 * Parameters:
 *   - array: Pointer to a dynamically allocated string array.
 */

void freeArray_p(char **array)
{
	int i;

	if (array != NULL)
	{
		for (i = 0; array[i]; i++)
			free(array[i]);

		free(array);
		array = NULL;
	}
}

