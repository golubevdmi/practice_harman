#include <stringmod.h>


String::String() : m_pszStr(NULL) {}

String::String(const char *pcszStr) :
	m_pszStr((char *)std::calloc(1 + std::strlen(pcszStr), sizeof(char)))
{
	if (!m_pszStr)
		throw std::bad_alloc();
	std::strcpy(m_pszStr, pcszStr);
}

String::String(const String& str)
{
	if (!str.empty())
	{
		// Memory allocation
		m_pszStr = (char *)std::calloc((1 + std::strlen(str)), sizeof(char));
		if (!m_pszStr)
			throw std::bad_alloc();
		std::strcpy(m_pszStr, str.m_pszStr);
	}
	else
	{
		m_pszStr = NULL;
	}
}

String::~String()
{
	// Free memory
	if (m_pszStr)
		free(m_pszStr);
	m_pszStr = NULL;
}


bool String::empty() const
{
	return !(bool)m_pszStr;
}


char* String::c_str()
{
	return m_pszStr;
}
const char* String::c_str() const
{
	return m_pszStr;
}


String::operator const char *() const
{
	return m_pszStr;
}


String& String::operator=(const String& str)
{
	if (this != &str)
	{
		if (!str.empty())
		{
			size_t szSrc = 0, szDst = 0;
			if (m_pszStr)
				szSrc = std::strlen(m_pszStr);
			if (str.m_pszStr)
				szDst = std::strlen(str.m_pszStr);

			if (szSrc != szDst)
			{
				// Free memory
				if (m_pszStr)
					free(m_pszStr);
				// Memory allocation
				m_pszStr = (char *)std::calloc((1 + szDst), sizeof(char));
				if (!m_pszStr)
					throw std::bad_alloc();
			}
			std::strcpy(m_pszStr, str.m_pszStr);
		}
		else
		{
			if (m_pszStr)
				free(m_pszStr);
			m_pszStr = NULL;
		}
	}
	return *this;
}