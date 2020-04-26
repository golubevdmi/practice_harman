#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstring>
#include <cassert>


// Replacing substring with another substring
void strreplace(char* pszResult, const char* pcszSubstr, const char* pcszStr2);

int main(int argc, char* argv)
{
	printf("String replacement function example. \n\n");

	char str[1000] = "--ABCDCDCDCD++++ABABCDCD------";
	char substr[] = "ABCD";
	char substr2[] = "0AB";

	printf("Src: %s\n", str);
	printf("Substr: %s\n", substr);
	printf("Substr2: %s\n", substr2);
	strreplace(str, substr, substr2);
	printf("Output: %s\n", str);
	return 0;
}

void strreplace(char* pszResult, const char* pcszSubstr, const char* pcszStr2)
{
	if (pszResult == NULL || pcszSubstr == NULL || pcszStr2 == NULL)
		return;

	size_t lenghtSrc = strlen(pszResult);
	size_t lenghtSub = strlen(pcszSubstr);
	size_t lenghtSub2 = strlen(pcszStr2);
	ptrdiff_t i = lenghtSub2 - lenghtSub;

	char *pTmp = pszResult;
	for (; pTmp = strstr(pTmp, pcszSubstr); pTmp += lenghtSub2)
	{
		if (lenghtSub < lenghtSub2)
		{
			memmove(pTmp + i, pTmp, lenghtSrc - (size_t)(pTmp - pszResult));
			lenghtSrc += i;
		}
		else if (lenghtSub > lenghtSub2)
		{
			memcpy(pTmp, pTmp - i, lenghtSrc - (size_t)(pTmp - pszResult) + i);
			lenghtSrc -= i;
		}
		strncpy(pTmp, pcszStr2, lenghtSub2);
	}
}