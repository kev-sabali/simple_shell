#include "shell.h"

/* Function: _exe
 * ---------------
 * Executes the command specified in the ARWEAVE structure.
 *
 * Parameters:
 *   - data: Pointer to an ARWEAVE structure containing command information.
 *
 * Returns:
 *   - Returns 0 on success, or an error code if execution encounters issues.
 */
int _exe(ARWEAVE *data)
{
	int retval = 0, status;
	pid_t pidd;

	retval = cmdList(data);
	if (retval != -1)
		return (retval);

	retval = identifyProgram(data);
	if (retval)
	{
		return (retval);
	}
	else
	{
		pidd = fork();
		if (pidd == -1)
		{
			perror(data->cmdName);
			exit(EXIT_FAILURE);
		}
		if (pidd == 0)
		{
			retval = execve(data->tokens[0], data->tokens, data->env);
			if (retval == -1)
				perror(data->cmdName), exit(EXIT_FAILURE);
		}
		else
		{
			wait(&status);
			if (WIFEXITED(status))
				errno = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				errno = 128 + WTERMSIG(status);
		}
	}
	return (0);
}

