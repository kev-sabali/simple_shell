#include "shell.h"

/* Function: main
 * ---------------
 * The main entry point of the shell.
 *
 * Parameters:
 *   - argc: Number of command-line arguments.
 *   - argv: Array of command-line arguments.
 *   - env:  Array of environment variables.
 *
 * Returns:
 *   0 on successful execution.
 */
int main(int argc, char *argv[], char *env[])
{
	ARWEAVE data_struct = {NULL}, *data = &data_struct;
	char *prompt = "";

	iniData(data, argc, argv, env);

	signal(SIGINT, hnldControl);

	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO) && argc == 1)
	{
		errno = 2;/*???????*/
		prompt = PROMPT_MSG;
	}
	errno = 0;
	loopInfinite(prompt, data);
	return (0);
}


/* Function: hnldControl
 * ---------------------
 * Signal handler for SIGINT (Ctrl+C).
 *
 * Parameters:
 *   - opr: Signal number (unused).
 */
void hnldControl(int opr UNUSED)
{
	_puts("\n");
	_puts(PROMPT_MSG);
}


/* Function: iniData
 * ------------------
 * Initializes the data structure with default values.
 *
 * Parameters:
 *   - data: Pointer to the ARWEAVE structure.
 *   - argc: Number of command-line arguments.
 *   - argv: Array of command-line arguments.
 *   - env:  Array of environment variables.
 */
void iniData(ARWEAVE *data, int argc, char *argv[], char **env)
{
	int i = 0;

	data->prgName = argv[0];
	data->inputLine = NULL;
	data->cmdName = NULL;
	data->xCounter = 0;

	if (argc == 1)
		data->fileDesc = STDIN_FILENO;
	else
	{
		data->fileDesc = open(argv[1], O_RDONLY);
		if (data->fileDesc == -1)
		{
			_putchar(data->prgName);
			_putchar(": 0: Can't open ");
			_putchar(argv[1]);
			_putchar("\n");
			exit(127);
		}
	}
	data->tokens = NULL;
	data->env = malloc(sizeof(char *) * 50);
	if (env)
	{
		for (; env[i]; i++)
		{
			data->env[i] = strDuplicate(env[i]);
		}
	}
	data->env[i] = NULL;
	env = data->env;

	data->aliasList = malloc(sizeof(char *) * 20);
	for (i = 0; i < 20; i++)
	{
		data->aliasList[i] = NULL;
	}
}

/* Function: loopInfinite
 * -----------------------
 * The main loop for the shell, processing user input.
 *
 * Parameters:
 *   - prompt: The shell prompt.
 *   - data:   Pointer to the ARWEAVE structure.
 */
void loopInfinite(char *prompt, ARWEAVE *data)
{
	int error_code = 0, string_len = 0;

	while (++(data->xCounter))
	{
		_puts(prompt);
		error_code = string_len = _getline(data);

		if (error_code == EOF)
		{
			freeData_all(data);
			exit(errno);
		}
		if (string_len >= 1)
		{
			expAlias(data);
			expVar(data);
			tokenise(data);
			if (data->tokens[0])
			{
				error_code = _exe(data);
				if (error_code != 0)
					_printErr(error_code, data);
			}
			freeData(data);
		}
	}
}

