#include "string_extensions.h"
// source of inspisation: https://www.geeksforgeeks.org/convert-floating-point-number-string/

static void reverse(char *str, int len)
{
    int i = 0, j = len - 1, temp;
    while (i < j)
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}

static int int_to_string(int number, char str[], int numberOfDigits)
{
    int i = 0;
    while (number)
    {
        str[i++] = (number % 10) + '0';
        number = number / 10;
    }

    // If number of digits required is more, then add 0s at the beginning
    while (i < numberOfDigits)
        str[i++] = '0';

    reverse(str, i);
    str[i] = '\0';
    return i;
}

// Converts a floating-point/double number to a string.
void float_to_string(float number, char *res, int afterpoint)
{
    // Extract the two part
    int ipart = (int)number;
    float fpart = number - (float)ipart;

    // convert integer part to string
    int i = int_to_string(ipart, res, 0);

    // check for display option after point
    if (afterpoint != 0)
    {
        res[i] = '.';
        fpart = fpart * pow(10, afterpoint);

        // convert float part to string
        int_to_string((int)fpart, res + i + 1, afterpoint);
    }
}