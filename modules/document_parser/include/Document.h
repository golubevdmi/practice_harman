#ifndef _DOCUMENT_PARSER_H__
#define _DOCUMENT_PARSER_H__

#include <iostream>
#include <vector>
#include <cstring>
#include <cassert>
#include <fstream>
#include <string>
#include <typeinfo>
#include <typeindex>
#include <cmath>
#include <algorithm>


#if defined(_WIN32) || defined(_WIN64)
	#ifdef document_parser_EXPORTS
		#define DOCUMENT_API __declspec(dllexport)
	#else
		#define DOCUMENT_API __declspec(dllimport)
	#endif
#else
	#define DOCUMENT_API
#endif

#if 0
	#define debug_message(message) printf("--> %s\n", message)
#else
	#define debug_message(message)
#endif


class Element;
class Table;
class Paragraph;
class AsciiArt;

// Abstract class document structure (for class document, table)
class DOCUMENT_API DocumentBase
{
public:
	virtual void show_tables(size_t maxLength = 0) const;
	virtual void show_ascii(size_t maxLength = 0) const;
	virtual void show_paragraph(size_t maxLength = 0) const;
	virtual void show_undef(size_t maxLength = 0) const;

	virtual size_t size() const;
	virtual size_t size_ascii() const;
	virtual size_t size_table() const;
	virtual size_t size_paragraph() const;
	virtual size_t size_undef() const;
	virtual size_t size_words() const;

protected:
	std::vector< std::shared_ptr<Element> > m_elements;
	size_t m_nAscii = 0, m_nTable = 0, m_nParagraph = 0, m_nUndef = 0;

	void show(const std::type_info& type, size_t maxLength = 0) const;
};


class DOCUMENT_API Document : public DocumentBase
{
public:
	Document(std::string filename = std::string());

	bool init(std::string filename);
	bool parse();
	void show(size_t maxLength = 0) const;

	std::vector<std::string> getDocStringVec() const;
	std::string getDocString() const;
private:
	std::vector<std::string> m_buffer;
};

// Abstract class document element
class DOCUMENT_API Element
{
public:
	virtual void add(std::string str);
	virtual void show(size_t maxLength = 0) const;

	virtual std::vector<std::string> getStringRows(size_t maxLength = 0) const;
	virtual std::string getStringShow(size_t maxLength = 0) const;
	virtual size_t getBufferSize() const { return m_buffer.size(); }

protected:
	std::vector<std::string> m_buffer;

};

// Table element
class DOCUMENT_API Table : public Element, public DocumentBase
{
public:
	static std::string tableSeparator;
	static bool isElement(const std::string& str, const char* separator);

	void add(std::string str) override;

	void setSeparator(std::string separator) { m_separator = separator; }
	virtual std::vector<std::string> getStringRows(size_t maxLength = 0) const;
	std::string getStringShow(size_t maxLength = 0) const override;

private:
	size_t m_cols, m_rows;
	std::string m_separator;
};

// Ascii element
class DOCUMENT_API AsciiArt : public Element
{
public:
	static std::string asciiArtSymbols;
	static bool isElement(const std::string& str);
};

// Paragraph element
class DOCUMENT_API Paragraph : public Element
{
public:
	static std::string ignoreSymbols;
	static bool isElement(const std::string& str);

	std::vector<std::string> getStringRows(size_t maxLength = 0) const override;
	std::string getStringShow(size_t maxLength) const;
	size_t getNumberOfWords() const;
	size_t getMaxLengthWord() const;
};

// Undefine element
class DOCUMENT_API UndefElement : public Element
{};


#endif // !_DOCUMENT_PARSER_H__