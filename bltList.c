#include "shell.h"

/*
 * Function: builtins_list
 * -----------------------
 * Searches for a built-in command in the list of available built-ins and executes it.
 *
 * data: A pointer to the ARWEAVE struct containing program data.
 *
 * Returns:
 *    0: Successful execution of the built-in command.
 *   -1: The specified command is not a built-in.
 *   [Specific Return Values]: Result of the executed built-in command.
 */
int cmdList(ARWEAVE *data)
{
	int iterator;
	command options[] = {
		{"exit", cmdExit},
		{"help", cmdHelp},
		{"cd", cmdCd},
		{"alias", cmdAlias},
		{"env", cmdEnv},
		{"setenv", cmdSet_env},
		{"unsetenv",cmdUnset_env},
		{NULL, NULL}
	};

	for (iterator = 0; options[iterator].builtin != NULL; iterator++)
	{

		if (strComp(options[iterator].builtin, data->cmdName, 0))
		{

			return (options[iterator].function(data));
		}

	}
	return (-1);
}

