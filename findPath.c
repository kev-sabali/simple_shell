#include "shell.h"

/* Function: check_file
 * ---------------------
 * Checks if the given file at the specified full path exists and is executable.
 *
 * Parameters:
 *   - full_path: The full path of the file to check.
 *
 * Returns:
 *   - Returns 0 if the file exists and is executable, 126 if it is not executable,
 *     or 127 if the file does not exist.
 */
int check_file(char *full_path);

/* Function: identifyProgram
 * -------------------------
 * Identifies the program associated with the command name in the ARWEAVE structure.
 * Searches directories in the PATH environment variable for the executable file.
 *
 * Parameters:
 *   - data: Pointer to an ARWEAVE structure containing command information.
 *
 * Returns:
 *   - Returns 0 if the program is found and executable, 126 if not executable,
 *     127 if the program is not found, or an error code if there are issues.
 */

int identifyProgram(ARWEAVE *data)
{
	int i = 0, ret_code = 0;
	char **directories;

	if (!data->cmdName)
		return (2);


	if (data->cmdName[0] == '/' || data->cmdName[0] == '.')
		return (check_file(data->cmdName));

	free(data->tokens[0]);
	data->tokens[0] = strConcat(strDuplicate("/"), data->cmdName);
	if (!data->tokens[0])
		return (2);

	directories = tokenPath(data);

	if (!directories || !directories[0])
	{
		errno = 127;
		return (127);
	}
	for (i = 0; directories[i]; i++)
	{
		directories[i] = strConcat(directories[i], data->tokens[0]);
		ret_code = check_file(directories[i]);
		if (ret_code == 0 || ret_code == 126)
		{
			errno = 0;
			free(data->tokens[0]);
			data->tokens[0] = strDuplicate(directories[i]);
			freeArray_p(directories);
			return (ret_code);
		}
	}
	free(data->tokens[0]);
	data->tokens[0] = NULL;
	freeArray_p(directories);
	return (ret_code);
}

/* Function: tokenPath
 * --------------------
 * Tokenizes the PATH environment variable into an array of directory paths.
 *
 * Parameters:
 *   - data: Pointer to an ARWEAVE structure containing environment information.
 *
 * Returns:
 *   - Returns an array of directory paths from the PATH environment variable.
 */

char **tokenPath(ARWEAVE *data)
{
	int i = 0;
	int counter_directories = 2;
	char **tokens = NULL;
	char *PATH;


	PATH = environKey("PATH", data);
	if ((PATH == NULL) || PATH[0] == '\0')
	{
		return (NULL);
	}

	PATH = strDuplicate(PATH);


	for (i = 0; PATH[i]; i++)
	{
		if (PATH[i] == ':')
			counter_directories++;
	}


	tokens = malloc(sizeof(char *) * counter_directories);


	i = 0;
	tokens[i] = strDuplicate(_strtok(PATH, ":"));
	while (tokens[i++])
	{
		tokens[i] = strDuplicate(_strtok(NULL, ":"));
	}

	free(PATH);
	PATH = NULL;
	return (tokens);

}

/* Function: check_file
 * ---------------------
 * Checks if the given file at the specified full path exists and is executable.
 *
 * Parameters:
 *   - full_path: The full path of the file to check.
 *
 * Returns:
 *   - Returns 0 if the file exists and is executable, 126 if it is not executable,
 *     or 127 if the file does not exist.
 */

int check_file(char *full_path)
{
	struct stat sb;

	if (stat(full_path, &sb) != -1)
	{
		if (S_ISDIR(sb.st_mode) ||  access(full_path, X_OK))
		{
			errno = 126;
			return (126);
		}
		return (0);
	}

	errno = 127;
	return (127);
}

