#include "shell.h"
/* Function: cmdExit
 * ------------------
 * Implements the 'exit' command. Exits the shell with the specified exit code.
 *
 * Parameters:
 *   - data: Pointer to an ARWEAVE structure containing tokens, command name, etc.
 *
 * Returns:
 *   - Does not return, as it exits the program with the specified exit code.
 */


int cmdExit(ARWEAVE *data)
{
	int i;

	if (data->tokens[1] != NULL)
	{
		for (i = 0; data->tokens[1][i]; i++)
			if ((data->tokens[1][i] < '0' || data->tokens[1][i] > '9')
				&& data->tokens[1][i] != '+')
			{
				errno = 2;
				return (2);
			}
		errno = _atoi(data->tokens[1]);
	}
	freeData_all(data);
	exit(errno);
}
/* Function: cmdCd
 * ----------------
 * Implements the 'cd' command. Changes the current working directory.
 *
 * Parameters:
 *   - data: Pointer to an ARWEAVE structure containing tokens, command name, etc.
 *
 * Returns:
 *   - Returns 0 on success, 3 if changing directory fails.
 */

int cmdCd(ARWEAVE *data)
{
	char *dir_home = environKey("HOME", data), *dir_old = NULL;
	char old_dir[128] = {0};
	int error_code = 0;

	if (data->tokens[1])
	{
		if (strComp(data->tokens[1], "-", 0))
		{
			dir_old = environKey("OLDPWD", data);
			if (dir_old)
				error_code = setWork_dir(data, dir_old);
			_puts(environKey("PWD", data));
			_puts("\n");

			return (error_code);
		}
		else
		{
			return (setWork_dir(data, data->tokens[1]));
		}
	}
	else
	{
		if (!dir_home)
			dir_home = getcwd(old_dir, 128);

		return (setWork_dir(data, dir_home));
	}
	return (0);
}

/* Function: setWork_dir
 * ----------------------
 * Sets the current working directory and updates environment variables.
 *
 * Parameters:
 *   - data: Pointer to an ARWEAVE structure containing tokens, command name, etc.
 *   - new_dir: The directory to change to.
 *
 * Returns:
 *   - Returns 0 on success, 3 if changing directory fails.
 */

int setWork_dir(ARWEAVE *data, char *new_dir)
{
	char old_dir[128] = {0};
	int err_code = 0;

	getcwd(old_dir, 128);

	if (!strComp(old_dir, new_dir, 0))
	{
		err_code = chdir(new_dir);
		if (err_code == -1)
		{
			errno = 2;
			return (3);
		}
		environSet_key("PWD", new_dir, data);
	}
	environSet_key("OLDPWD", old_dir, data);
	return (0);
}

/* Function: cmdHelp
 * -------------------
 * Implements the 'help' command. Displays help messages for different commands.
 *
 * Parameters:
 *   - data: Pointer to an ARWEAVE structure containing tokens, command name, etc.
 *
 * Returns:
 *   - Returns 1 on success, 0 if the provided command is not recognized.
 */

int cmdHelp(ARWEAVE *data)
{
	int i, length = 0;
	char *mensajes[6] = {NULL};

	mensajes[0] = HELP_MSG;

	/* validate args */
	if (data->tokens[1] == NULL)
	{
		_puts(mensajes[0] + 6);
		return (1);
	}
	if (data->tokens[2] != NULL)
	{
		errno = E2BIG;
		perror(data->cmdName);
		return (5);
	}
	mensajes[1] = HELP_EXIT_MSG;
	mensajes[2] = HELP_ENV_MSG;
	mensajes[3] = HELP_SETENV_MSG;
	mensajes[4] = HELP_UNSETENV_MSG;
	mensajes[5] = HELP_CD_MSG;

	for (i = 0; mensajes[i]; i++)
	{

		length = strLength(data->tokens[1]);
		if (strComp(data->tokens[1], mensajes[i], length))
		{
			_puts(mensajes[i] + length + 1);
			return (1);
		}
	}

	errno = EINVAL;
	perror(data->cmdName);
	return (0);
}

/* Function: cmdAlias
 * --------------------
 * Implements the 'alias' command. Displays or sets aliases.
 *
 * Parameters:
 *   - data: Pointer to an ARWEAVE structure containing tokens, command name, etc.
 *
 * Returns:
 *   - Returns 0 on success.
 */

int cmdAlias(ARWEAVE *data)
{
	int i = 0;


	if (data->tokens[1] == NULL)
		return (prntAlias(data, NULL));

	while (data->tokens[++i])
	{
		if (countChar(data->tokens[i], "="))
			setAlias(data->tokens[i], data);
		else
			prntAlias(data, data->tokens[i]);
	}

	return (0);
}

