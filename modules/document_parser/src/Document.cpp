#include <document.h>


// Default constructor
Document::Document(std::string filename)
{
	if (!filename.empty())
		if (!init(filename))
			throw;
}

// Get information from document
bool Document::init(std::string filename)
{
	std::ifstream in(filename, std::ios::in);
	if (in.is_open())
	{
		if (!m_elements.empty())
			m_elements.clear();
		if (!m_buffer.empty())
			m_buffer.clear();

		while (!in.eof())
		{
			std::string line;
			std::getline(in, line);
			m_buffer.push_back(std::string(line));
		}
		in.close();
		return true;
	}
	return false;
}

// Parse a document with elements
bool Document::parse()
{
	if (!m_buffer.empty())
	{
		Element* elem = nullptr;
		for (auto it = m_buffer.begin(); it != m_buffer.end(); ++it)
		{
			// Check on table element
			if (Table::isElement(*it, Table::tableSeparator.c_str()))
			{
				if (elem != nullptr && typeid(*elem) != typeid(Table))
				{
					m_elements.push_back(std::shared_ptr<Element>(elem));
					elem = nullptr;
				}
				if (elem == nullptr)
				{
					elem = new Table();
					m_nTable++;
				}
				elem->add(*it);
			}
			// Check on ascii element
			else if (AsciiArt::isElement(*it))
			{
				if (elem != nullptr && typeid(*elem) != typeid(AsciiArt))
				{
					m_elements.push_back(std::shared_ptr<Element>(elem));
					elem = nullptr;
				}
				if (elem == nullptr)
				{
					elem = new AsciiArt();
					m_nAscii++;
				}
				elem->add(*it);
			}
			// Check on paragraph element
			else if (Paragraph::isElement(*it))
			{
				if (elem != nullptr && typeid(*elem) != typeid(Paragraph))
				{
					m_elements.push_back(std::shared_ptr<Element>(elem));
					elem = nullptr;
				}
				if (elem == nullptr)
				{
					elem = new Paragraph();
					m_nParagraph++;
				}
				elem->add(*it);
			}
			else
			{
				if (elem != nullptr && typeid(*elem) != typeid(UndefElement))
				{
					m_elements.push_back(std::shared_ptr<Element>(elem));
					elem = nullptr;
				}
				if (elem == nullptr)
				{
					elem = new UndefElement();
					m_nUndef++;
				}
				elem->add(*it);
			}
		}
		if (elem != nullptr)
		{
			m_elements.push_back(std::shared_ptr<Element>(elem));
			elem = nullptr;
		}
		return true;
	}
	return false;
}


void Document::show(size_t maxLength) const
{
	assert(maxLength > 0);
	for (const auto& element : m_elements)
		element->show(maxLength);
}

// Original string vector with document
std::vector<std::string> Document::getDocStringVec() const
{
	return m_buffer;
}
// Original string with document
std::string Document::getDocString() const
{
	std::string strOut;
	for (const auto& str : m_buffer)
		strOut.append(str);
	return strOut;
}