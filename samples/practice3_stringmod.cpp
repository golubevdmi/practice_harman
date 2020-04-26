#include <stringmod.h>


int main(int argc, const char* argv)
{
	printf("String Class and its methods examples. \n\n");

	// Is empty method
	String str0;
	printf("Is empty str0: %d\n", str0.empty());
	
	String str1 = "Helen";
	String str2("Victor");

	// c.str() method
	printf("str1.c_str(): %s \n", str1.c_str());
	// const char*
	printf("static_cast<const char*>(str2): %s \n", static_cast<const char*>(str2));
	// Copy Constructor
	String str3(str2);
	printf("str3 (str2): %s \n", str3);

	// operator =
	str1 = str2;
	printf("\nstr1 = str2 \n");
	printf("str1: %s \n", str1);
	printf("str2: %s \n", str2);
	return 0;
}