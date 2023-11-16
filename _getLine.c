#include "shell.h"

/* Function: _getline
 * -------------------
 * Reads a line from a file descriptor, tokenizes it based on the ';' delimiter,
 * and checks for logical operators ('&&' and '||') to split the commands.
 * The first command is returned, and subsequent commands are stored in a static array.
 *
 * Parameters:
 *   - data: Pointer to an ARWEAVE structure containing file descriptor and input line.
 *
 * Returns:
 *   - Length of the input line (excluding null terminator) or -1 on EOF.
 */
int _getline(ARWEAVE *data)
{
	char buff[BUFFER_SIZE] = {'\0'};
	static char *array_commands[10] = {NULL};
	static char array_operators[10] = {'\0'};
	ssize_t bytes_read, i = 0;

/* Check if the array_commands is empty or if there's a logical operator that
     * should affect the next command's execution. If true, read from the file descriptor
     * and tokenize the input.
    */

	if (!array_commands[0] || (array_operators[0] == '&' && errno != 0) ||
		(array_operators[0] == '|' && errno == 0))
	{

		for (i = 0; array_commands[i]; i++)
		{
			free(array_commands[i]);
			array_commands[i] = NULL;
		}


		bytes_read = read(data->fileDesc, &buff, BUFFER_SIZE - 1);
		if (bytes_read == 0)
			return (-1);


		i = 0;
		do {
			array_commands[i] = strDuplicate(_strtok(i ? NULL : buff, "\n;"));

			i = checkLogic(array_commands, i, array_operators);
		} while (array_commands[i++]);
	}

	data->inputLine = array_commands[0];
	for (i = 0; array_commands[i]; i++)
	{
		array_commands[i] = array_commands[i + 1];
		array_operators[i] = array_operators[i + 1];
	}

	return (strLength(data->inputLine));
}

/* Function: checkLogic
 * ---------------------
 * Checks for logical operators ('&&' and '||') in the given command,
 * and splits the command accordingly. Updates the array_commands and array_operators.
 *
 * Parameters:
 *   - array_commands: Array of strings representing commands.
 *   - i: Index of the current command in array_commands.
 *   - array_operators: Array of characters representing logical operators.
 *
 * Returns:
 *   - Updated index (i) to process the next command.
 */

int checkLogic(char *array_commands[], int i, char array_operators[])
{
	char *temp = NULL;
	int j;

	for (j = 0; array_commands[i] != NULL  && array_commands[i][j]; j++)
	{
		if (array_commands[i][j] == '&' && array_commands[i][j + 1] == '&')
		{

			temp = array_commands[i];
			array_commands[i][j] = '\0';
			array_commands[i] = strDuplicate(array_commands[i]);
			array_commands[i + 1] = strDuplicate(temp + j + 2);
			i++;
			array_operators[i] = '&';
			free(temp);
			j = 0;
		}
		if (array_commands[i][j] == '|' && array_commands[i][j + 1] == '|')
		{

			temp = array_commands[i];
			array_commands[i][j] = '\0';
			array_commands[i] = strDuplicate(array_commands[i]);
			array_commands[i + 1] = strDuplicate(temp + j + 2);
			i++;
			array_operators[i] = '|';
			free(temp);
			j = 0;
		}
	}
	return (i);
}

