#include <Aplicacion.h>

size_t strlen(const char *s)
{
	size_t n = 0;
	while(s[n] != '\0')
		n++;
	return n;
}

int strcmp(const char *s1, const char *s2)
{
	int i=0;
	
	while(s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
		i++;

	return s1[i] - s2[i];
}

int strncmp(const char *s1, const char *s2, size_t n)
{
	size_t i=0;

	while(i < n && s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
		i++;
	
	if(i==n)
		i--;

	return s1[i] - s2[i];
}

char *strcat(char *dest, const char *src)
{
	size_t i, dest_len = 0;

	while(dest[dest_len] != '\0')
		dest_len++;

	for(i=0; src[i] != '\0'; i++)
		dest[dest_len + i] = src[i];

	dest[dest_len + i] = '\0';

	return dest;
}

char *strncat(char *dest, const char *src, size_t n)
{
	size_t i, dest_len = 0;

	while(dest[dest_len] != '\0')
		dest_len++;

	for(i=0; i<n && src[i] != '\0'; i++)
		dest[dest_len + i] = src[i];

	dest[dest_len + i] = '\0';

	return dest;
}

char *strcpy(char *dest, const char *src)
{
	size_t i;

	for(i=0; src[i] != '\0'; i++)
		dest[i] = src[i];
	dest[i] = '\0';

	return dest;
}

char *strncpy(char *dest, const char *src, size_t n)
{
	size_t i;

	for(i=0; i<n && src[i] != '\0'; i++)
		dest[i] = src[i];
	for(; i<n; i++)
		dest[i] = '\0';

	return dest;
}

char *index(const char *s, int c)
{
	size_t i;

	for(i=0; s[i] != '\0'; i++)
		if(s[i] == c)
			return (char*) (s+i);
	
	return NULL;
}

char *rindex(const char *s, int c)
{
	int i;
	int s_len = (int) strlen(s);

	for( i = s_len ; i >= 0 ; i-- )
		if(s[i] == c)
			return (char*) (s+i);
	
	return NULL;
}

// Stores s[from..to) in sub.
// The caller is responsible for memory allocation.
void extract_substr(char const *s, char *sub, size_t from, size_t to)
{
    size_t sublen = to - from;
    strncpy(sub, s + from, sublen);
    sub[sublen] = '\0';
}


// reverses a string 'str' of length 'len'
void reverse(char *str, int len)
{
    int i=0, j=len-1, temp;
    while (i<j)
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++; j--;
    }
}

 // Converts a given integer x to string str[].  d is the number
 // of digits required in output. If d is more than the number
 // of digits in x, then 0s are added at the beginning.
int intToStr(int x, char str[], int d)
{
    int i = 0;
    while (x)
    {
        str[i++] = (x%10) + '0';
        x = x/10;
    }

    // If number of digits required is more, then
    // add 0s at the beginning
    while (i < d)
        str[i++] = '0';

    reverse(str, i);
    str[i] = '\0';
    return i;
}

// Converts a floating point number to string.
void ftoa(float n, char *res, int afterpoint)
{
    // Extract integer part
    int ipart = (int)n;

    // Extract floating part
    float fpart = n - (float)ipart;

    // convert integer part to string
    int i = intToStr(ipart, res, 0);

    // check for display option after point
    if (afterpoint != 0)
    {
        res[i] = '.';  // add dot

        // Get the value of fraction part upto given no.
        // of points after dot. The third parameter is needed
        // to handle cases like 233.007
        fpart = fpart * ipow(10, afterpoint);

        intToStr((int)fpart, res + i + 1, afterpoint);
    }
}
