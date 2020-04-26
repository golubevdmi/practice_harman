#ifndef _STRINGMOD_H__
#define _STRINGMOD_H__

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstring>
#include <cassert>


#if defined(_WIN32) || defined(_WIN64)
	#ifdef stringmod_EXPORTS
		#define STRINGMOD_API __declspec(dllexport)
	#else
		#define STRINGMOD_API __declspec(dllimport)
	#endif
#else
	#define STRINGMOD_API
#endif


// Class mimicking string functionality
class STRINGMOD_API String
{
public:
	String();
	String(const char *pcszStr);
	String(const String& str);
	~String();

	bool empty() const;
	char* c_str();
	const char* c_str() const;

	operator const char*() const;
	String& operator=(const String& str);
private:
	char *m_pszStr;
};

#endif // !_STRINGMOD_H__
