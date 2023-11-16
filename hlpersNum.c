#include "shell.h"

/* Function: lngString
 * --------------------
 * Converts a long integer to a string representation in the specified base.
 * Stores the result in the provided 'string' array.
 *
 * Parameters:
 *   - number: The long integer to be converted.
 *   - string: Array to store the resulting string.
 *   - base: The base for the conversion (e.g., 10 for decimal, 16 for hexadecimal).
 */

void lngString(long number, char *string, int base)
{
	int index = 0, inNegative = 0;
	long cociente = number;
	char letters[] = {"0123456789abcdef"};

	if (cociente == 0)
		string[index++] = '0';

	if (string[0] == '-')
		inNegative = 1;

	while (cociente)
	{
		if (cociente < 0)
			string[index++] = letters[-(cociente % base)];
		else
			string[index++] = letters[cociente % base];
		cociente /= base;
	}
	if (inNegative)
		string[index++] = '-';

	string[index] = '\0';
	strVerse(string);
}

/* Function: _atoi
 * ----------------
 * Converts a string representation of an integer to an actual integer.
 *
 * Parameters:
 *   - s: The string to be converted to an integer.
 *
 * Returns:
 *   The converted integer value.
 */

int _atoi(char *s)
{
	int sign = 1;
	unsigned int number = 0;

	while (!('0' <= *s && *s <= '9') && *s != '\0')
	{
		if (*s == '-')
			sign *= -1;
		if (*s == '+')
			sign *= +1;
		s++;
	}


	while ('0' <= *s && *s <= '9' && *s != '\0')
	{

		number = (number * 10) + (*s - '0');
		s++;
	}
	return (number * sign);
}

/* Function: countChar
 * --------------------
 * Counts the occurrences of a specific character in a string.
 *
 * Parameters:
 *   - string: The input string to be checked.
 *   - character: The character whose occurrences are counted.
 *
 * Returns:
 *   The count of occurrences of the specified character in the string.
 */

int countChar(char *string, char *character)
{
	int i = 0, counter = 0;

	for (; string[i]; i++)
	{
		if (string[i] == character[0])
			counter++;
	}
	return (counter);
}

