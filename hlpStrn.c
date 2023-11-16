#include "shell.h"

/* Function: strLength
 * --------------------
 * Calculates the length of a string.
 *
 * Parameters:
 *   - string: The input string.
 *
 * Returns:
 *   The length of the string.
 */
int strLength(char *string)
{
	int length = 0;

	if (string == NULL)
		return (0);

	while (string[length++] != '\0')
	{
	}
	return (--length);
}



char *strDuplicate(char *string)
{
	char *result;
	int length, i;

	if (string == NULL)
		return (NULL);

	length = strLength(string) + 1;

	result = malloc(sizeof(char) * length);

	if (result == NULL)
	{
		errno = ENOMEM;
		perror("Error");
		return (NULL);
	}
	for (i = 0; i < length ; i++)
	{
		result[i] = string[i];
	}

	return (result);
}

/* Function: strComp
 * ------------------
 * Compares two strings.
 *
 * Parameters:
 *   - string1: The first string.
 *   - string2: The second string.
 *   - number: The number of characters to compare (0 for full comparison).
 *
 * Returns:
 *   1 if the strings are equal, 0 otherwise.
 */
int strComp(char *string1, char *string2, int number)
{
	int iterator;

	if (string1 == NULL && string2 == NULL)
		return (1);

	if (string1 == NULL || string2 == NULL)
		return (0);

	if (number == 0)
	{
		if (strLength(string1) != strLength(string2))
			return (0);
		for (iterator = 0; string1[iterator]; iterator++)
		{
			if (string1[iterator] != string2[iterator])
				return (0);
		}
		return (1);
	}
	else
	{
		for (iterator = 0; iterator < number ; iterator++)
		{
			if (string1[iterator] != string2[iterator])
			return (0);
		}
		return (1);
	}
}

/* Function: strConcat
 * --------------------
 * Concatenates two strings.
 *
 * Parameters:
 *   - string1: The first string.
 *   - string2: The second string.
 *
 * Returns:
 *   A pointer to the concatenated string.
 */
char *strConcat(char *string1, char *string2)
{
	char *result;
	int length1 = 0, length2 = 0;

	if (string1 == NULL)
		string1 = "";
	length1 = strLength(string1);

	if (string2 == NULL)
		string2 = "";
	length2 = strLength(string2);

	result = malloc(sizeof(char) * (length1 + length2 + 1));
	if (result == NULL)
	{
		errno = ENOMEM;
		perror("Error");
		return (NULL);
	}


	for (length1 = 0; string1[length1] != '\0'; length1++)
		result[length1] = string1[length1];
	free(string1);


	for (length2 = 0; string2[length2] != '\0'; length2++)
	{
		result[length1] = string2[length2];
		length1++;
	}

	result[length1] = '\0';
	return (result);
}


/* Function: strVerse
 * -------------------
 * Reverses the characters in a string.
 *
 * Parameters:
 *   - string: The string to be reversed.
 */
void strVerse(char *string)
{

	int i = 0, length = strLength(string) - 1;
	char hold;

	while (i < length)
	{
		hold = string[i];
		string[i++] = string[length];
		string[length--] = hold;
	}
}

