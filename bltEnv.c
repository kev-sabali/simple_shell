#include "shell.h"
/* Function: cmdEnv
 * -----------------
 * Implements the 'env' command. If no arguments are provided, prints the environment variables.
 * If an argument in the form 'name=value' is provided, sets or updates the environment variable
 * and prints the updated environment.
 *
 * Parameters:
 *   - data: Pointer to an ARWEAVE structure containing tokens, command name, etc.
 *
 * Returns:
 *   - Always returns 0.
 */

int cmdEnv(ARWEAVE *data)
{
	int i;
	char cpname[50] = {'\0'};
	char *var_copy = NULL;


	if (data->tokens[1] == NULL)
		prntEnviron(data);
	else
	{
		for (i = 0; data->tokens[1][i]; i++)
		{
			if (data->tokens[1][i] == '=')
			{

				var_copy = strDuplicate(environKey(cpname, data));
				if (var_copy != NULL)
					environSet_key(cpname, data->tokens[1] + i + 1, data);


				prntEnviron(data);
				if (environKey(cpname, data) == NULL)
				{
					_puts(data->tokens[1]);
					_puts("\n");
				}
				else
				{
					environSet_key(cpname, var_copy, data);
					free(var_copy);
				}
				return (0);
			}
			cpname[i] = data->tokens[1][i];
		}
		errno = 2;
		perror(data->cmdName);
		errno = 127;
	}
	return (0);
}

/* Function: cmdSet_env
 * ---------------------
 * Implements the 'set_env' command. Sets or updates an environment variable with the provided value.
 *
 * Parameters:
 *   - data: Pointer to an ARWEAVE structure containing tokens, command name, etc.
 *
 * Returns:
 *   - Returns 0 on success, 5 if too many arguments are provided.
 */

int cmdSet_env(ARWEAVE *data)
{

	if (data->tokens[1] == NULL || data->tokens[2] == NULL)
		return (0);
	if (data->tokens[3] != NULL)
	{
		errno = E2BIG;
		perror(data->cmdName);
		return (5);
	}

	environSet_key(data->tokens[1], data->tokens[2], data);

	return (0);
}

/* Function: cmdUnset_env
 * -----------------------
 * Implements the 'unset_env' command. Removes an environment variable.
 *
 * Parameters:
 *   - data: Pointer to an ARWEAVE structure containing tokens, command name, etc.
 *
 * Returns:
 *   - Returns 0 on success, 5 if too many arguments are provided.
 */

int cmdUnset_env(ARWEAVE *data)
{

	if (data->tokens[1] == NULL)
		return (0);
	if (data->tokens[2] != NULL)
	{
		errno = E2BIG;
		perror(data->cmdName);
		return (5);
	}
	environRemove_key(data->tokens[1], data);

	return (0);
}

