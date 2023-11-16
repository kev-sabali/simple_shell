#include "shell.h"
/* Function: prntAlias
 * ---------------------
 * Prints aliases from the aliasList in the ARWEAVE structure. If 'alias' is provided,
 * prints only the matching aliases. Each alias is printed in the format 'alias=value'.
 *
 * Parameters:
 *   - data: Pointer to an ARWEAVE structure containing aliasList.
 *   - alias: The specific alias to print (optional, can be NULL to print all aliases).
 *
 * Returns:
 *   - Always returns 0.
 */

int prntAlias(ARWEAVE *data, char *alias)
{
	int i, j, alias_length;
	char buffer[250] = {'\0'};

	if (data->aliasList)
	{
		alias_length = strLength(alias);
		for (i = 0; data->aliasList[i]; i++)
		{
			if (!alias || (strComp(data->aliasList[i], alias, alias_length)
				&&	data->aliasList[i][alias_length] == '='))
			{
				for (j = 0; data->aliasList[i][j]; j++)
				{
					buffer[j] = data->aliasList[i][j];
					if (data->aliasList[i][j] == '=')
						break;
				}
				buffer[j + 1] = '\0';
				buff_Add(buffer, "'");
				buff_Add(buffer, data->aliasList[i] + j + 1);
				buff_Add(buffer, "'\n");
				_puts(buffer);
			}
		}
	}

	return (0);
}

/* Function: getAlias
 * -------------------
 * Retrieves the value associated with a given alias from the aliasList in the ARWEAVE structure.
 *
 * Parameters:
 *   - data: Pointer to an ARWEAVE structure containing aliasList.
 *   - name: The alias for which to retrieve the value.
 *
 * Returns:
 *   - Returns the value associated with the provided alias or NULL if not found.
 */

char *getAlias(ARWEAVE *data, char *name)
{
	int i, alias_length;


	if (name == NULL || data->aliasList == NULL)
		return (NULL);

	alias_length = strLength(name);

	for (i = 0; data->aliasList[i]; i++)
	{
		if (strComp(name, data->aliasList[i], alias_length) &&
			data->aliasList[i][alias_length] == '=')
		{
			return (data->aliasList[i] + alias_length + 1);
		}
	}

	return (NULL);

}

/* Function: setAlias
 * -------------------
 * Sets or updates an alias in the aliasList of the ARWEAVE structure.
 *
 * Parameters:
 *   - alias_string: The string containing the alias and its value (e.g., "alias=value").
 *   - data: Pointer to an ARWEAVE structure containing aliasList.
 *
 * Returns:
 *   - Returns 0 on success, 1 if alias_string or aliasList is NULL.
 */

int setAlias(char *alias_string, ARWEAVE *data)
{
	int i, j;
	char buffer[250] = {'0'}, *temp = NULL;


	if (alias_string == NULL ||  data->aliasList == NULL)
		return (1);

	for (i = 0; alias_string[i]; i++)
		if (alias_string[i] != '=')
			buffer[i] = alias_string[i];
		else
		{
			temp = getAlias(data, alias_string + i + 1);
			break;
		}

	for (j = 0; data->aliasList[j]; j++)
		if (strComp(buffer, data->aliasList[j], i) &&
			data->aliasList[j][i] == '=')
		{
			free(data->aliasList[j]);
			break;
		}


	if (temp)
	{
		buff_Add(buffer, "=");
		buff_Add(buffer, temp);
		data->aliasList[j] = strDuplicate(buffer);
	}
	else
		data->aliasList[j] = strDuplicate(alias_string);
	return (0);
}

