#include "shell.h"

/* Function: _puts
 * ----------------
 * Writes a string to the standard output.
 *
 * Parameters:
 *   - string: The string to be written.
 *
 * Returns:
 *   The number of characters written.
 */
int _puts(char *string)
{
	return (write(STDOUT_FILENO, string, strLength(string)));
}

/* Function: _putchar
 * -------------------
 * Writes a string to the standard error output.
 *
 * Parameters:
 *   - string: The string to be written.
 *
 * Returns:
 *   The number of characters written.
 */
int _putchar(char *string)
{
	return (write(STDERR_FILENO, string, strLength(string)));
}

/* Function: _printErr
 * ---------------------
 * Prints error messages based on the error code and the data structure.
 *
 * Parameters:
 *   - errorcode: The error code indicating the type of error.
 *   - data: The data structure containing relevant information.
 *
 * Returns:
 *   Always returns 0.
 */
int _printErr(int errorcode, ARWEAVE *data)
{
	char n_as_string[10] = {'\0'};

	lngString((long) data->xCounter, n_as_string, 10);

	if (errorcode == 2 || errorcode == 3)
	{
		_putchar(data->prgName);
		_putchar(": ");
		_putchar(n_as_string);
		_putchar(": ");
		_putchar(data->tokens[0]);
		if (errorcode == 2)
			_putchar(": Illegal number: ");
		else
			_putchar(": can't cd to ");
		_putchar(data->tokens[1]);
		_putchar("\n");
	}
	else if (errorcode == 127)
	{
		_putchar(data->prgName);
		_putchar(": ");
		_putchar(n_as_string);
		_putchar(": ");
		_putchar(data->cmdName);
		_putchar(": not found\n");
	}
	else if (errorcode == 126)
	{
		_putchar(data->prgName);
		_putchar(": ");
		_putchar(n_as_string);
		_putchar(": ");
		_putchar(data->cmdName);
		_putchar(": Permission denied\n");
	}
	return (0);
}

