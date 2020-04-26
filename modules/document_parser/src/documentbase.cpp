#include <Document.h>


// Display specific item
void DocumentBase::show(const std::type_info& type, size_t maxLength) const
{
	assert(maxLength > 0);
	for (const auto& element : m_elements)
		if (typeid(*element) == type)
			element->show(maxLength);
}
// Display specific item (tables)
void DocumentBase::show_tables(size_t maxLength) const
{
	show(typeid(Table), maxLength);
	for (const auto& element : m_elements)
		if (typeid(*element) == typeid(Table))
			std::static_pointer_cast<Table>(element)->show_tables(maxLength);
}
// Display specific item (ascii arts)
void DocumentBase::show_ascii(size_t maxLength) const
{
	show(typeid(AsciiArt), maxLength);
	for (const auto& element : m_elements)
		if (typeid(*element) == typeid(Table))
			std::static_pointer_cast<Table>(element)->show_ascii(maxLength);
}
// Display specific item (paragraphs)
void DocumentBase::show_paragraph(size_t maxLength) const
{
	show(typeid(Paragraph), maxLength);
	for (const auto& element : m_elements)
		if (typeid(*element) == typeid(Table))
			std::static_pointer_cast<Table>(element)->show_paragraph(maxLength);
}
// Display specific item (undef)
void DocumentBase::show_undef(size_t maxLength) const
{
	show(typeid(UndefElement), maxLength);
	for (const auto& element : m_elements)
		if (typeid(*element) == typeid(Table))
			std::static_pointer_cast<Table>(element)->show_undef(maxLength);
}

size_t DocumentBase::size() const
{
	size_t nElements = 0;
	for (const auto& element : m_elements)
		if (typeid(*element) == typeid(Table))
			nElements += std::static_pointer_cast<Table>(element)->size();
	return nElements + m_elements.size();
}
size_t DocumentBase::size_ascii() const
{
	size_t nAscii = 0;
	for (const auto& element : m_elements)
		if (typeid(*element) == typeid(Table))
			nAscii += std::static_pointer_cast<Table>(element)->size_ascii();
	return nAscii + m_nAscii;
}
size_t DocumentBase::size_table() const
{
	size_t nTable = 0;
	for (const auto& element : m_elements)
		if (typeid(*element) == typeid(Table))
			nTable += std::static_pointer_cast<Table>(element)->size_table();
	return nTable + m_nTable;
}
size_t DocumentBase::size_paragraph() const
{
	size_t nParagraph = 0;
	for (const auto& element : m_elements)
		if (typeid(*element) == typeid(Table))
			nParagraph += std::static_pointer_cast<Table>(element)->size_paragraph();
	return nParagraph + m_nParagraph;
}
size_t DocumentBase::size_undef() const
{
	size_t nUndef = 0;
	for (const auto& element : m_elements)
		if (typeid(*element) == typeid(Table))
			nUndef += std::static_pointer_cast<Table>(element)->size_undef();
	return nUndef + m_nUndef;
}
// Words in all elements
size_t DocumentBase::size_words() const
{
	size_t nWords = 0;
	for (const auto& element : m_elements)
	{
		if (typeid(*element) == typeid(Paragraph))
			nWords += std::static_pointer_cast<Paragraph>(element)->getNumberOfWords();
		else if (typeid(*element) == typeid(Table))
			nWords += std::static_pointer_cast<Table>(element)->size_words();
	}
	return nWords;
}