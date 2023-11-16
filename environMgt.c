#include "shell.h"

/* Function: environKey
 * ---------------------
 * Retrieves the value associated with a given key from the environment variables.
 *
 * Parameters:
 *   - key: The key for which to retrieve the value.
 *   - data: Pointer to an ARWEAVE structure containing environment variables.
 *
 * Returns:
 *   - Returns the value associated with the provided key or NULL if not found.
 */

char *environKey(char *key, ARWEAVE *data)
{
	int i, key_length = 0;

	if (key == NULL || data->env == NULL)
		return (NULL);


	key_length = strLength(key);

	for (i = 0; data->env[i]; i++)
	{
		if (strComp(key, data->env[i], key_length) &&
		 data->env[i][key_length] == '=')
		{
			return (data->env[i] + key_length + 1);
		}
	}

	return (NULL);
}

/* Function: environSet_key
 * -------------------------
 * Sets or updates an environment variable with the provided key and value.
 *
 * Parameters:
 *   - key: The key for the environment variable.
 *   - value: The value to set for the environment variable.
 *   - data: Pointer to an ARWEAVE structure containing environment variables.
 *
 * Returns:
 *   - Returns 0 on success, 1 if key, value, or environment variables are NULL.
 */

int environSet_key(char *key, char *value, ARWEAVE *data)
{
	int i, key_length = 0, is_new_key = 1;


	if (key == NULL || value == NULL || data->env == NULL)
		return (1);

	key_length = strLength(key);

	for (i = 0; data->env[i]; i++)
	{
		if (strComp(key, data->env[i], key_length) &&
		 data->env[i][key_length] == '=')
		{
			is_new_key = 0;
			free(data->env[i]);
			break;
		}
	}

	data->env[i] = strConcat(strDuplicate(key), "=");
	data->env[i] = strConcat(data->env[i], value);

	if (is_new_key)
	{

		data->env[i + 1] = NULL;
	}
	return (0);
}

/* Function: environRemove_key
 * ----------------------------
 * Removes an environment variable with the provided key from the environment variables.
 *
 * Parameters:
 *   - key: The key for the environment variable to remove.
 *   - data: Pointer to an ARWEAVE structure containing environment variables.
 *
 * Returns:
 *   - Returns 1 on success (if the key is found and removed), 0 if the key is not found.
 */

int environRemove_key(char *key, ARWEAVE *data)
{
	int i, key_length = 0;


	if (key == NULL || data->env == NULL)
		return (0);


	key_length = strLength(key);

	for (i = 0; data->env[i]; i++)
	{
		if (strComp(key, data->env[i], key_length) &&
		 data->env[i][key_length] == '=')
		{
			free(data->env[i]);


			i++;
			for (; data->env[i]; i++)
			{
				data->env[i - 1] = data->env[i];
			}

			data->env[i - 1] = NULL;
			return (1);
		}
	}
	return (0);
}

/* Function: prntEnviron
 * -----------------------
 * Prints the environment variables to the standard output.
 *
 * Parameters:
 *   - data: Pointer to an ARWEAVE structure containing environment variables.
 */

void prntEnviron(ARWEAVE *data)
{
	int j;

	for (j = 0; data->env[j]; j++)
	{
		_puts(data->env[j]);
		_puts("\n");
	}
}

