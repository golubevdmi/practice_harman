#include <document.h>

// Specific symbol for table
std::string Table::tableSeparator = std::string();
// Check the string for compliance
bool Table::isElement(const std::string& str, const char* separator)
{
	return str.find(separator) != size_t(-1);
}


void Table::add(std::string str)
{
	m_buffer.push_back(str);
	m_cols = std::count(str.begin(), str.end(), '|') + 1;

	for (auto it_sub_begin = str.begin(); it_sub_begin != str.end(); )
	{
		auto it_sub_end = std::find(it_sub_begin, str.end(), '|');
		auto substr = str.substr(std::distance(str.begin(), it_sub_begin), std::distance(it_sub_begin, it_sub_end));
		
		Element* elem = nullptr;
		// Check on table element
		if (Table::isElement(substr, ";"))
		{
			elem = new Table();
			m_nTable++;
		}
		// Check on ascii element
		else if (AsciiArt::isElement(substr))
		{
			elem = new AsciiArt();
			m_nAscii++;
		}
		// Check on paragraph element
		else if (Paragraph::isElement(substr))
		{
			elem = new Paragraph();
			m_nParagraph++;
		}
		else
		{
			elem = new UndefElement();
			m_nUndef++;
		}
		if (elem != nullptr)
		{
			elem->add(substr);
			m_elements.push_back(std::shared_ptr<Element>(elem));
			elem = nullptr;
		}

		if (it_sub_end != str.end())
			it_sub_begin = it_sub_end + 1;
		else
			break;
	}
	++m_rows;
}


// Get string vector for display
std::vector<std::string> Table::getStringRows(size_t maxLength) const
{
	std::vector<std::string> strOutVec;
	std::string strOut = getStringShow(maxLength);
	if (maxLength)
	{
		auto colLength = maxLength / m_cols;
		for (size_t i = 0; i < m_rows; i++)
		{
			// Get row elements by string vector
			std::vector< std::vector<std::string> > colVec;
			for (size_t j = 0; j < m_cols; j++)
				colVec.push_back(m_elements[i * m_cols + j]->getStringRows(colLength));

			// Get max size
			size_t maxSize = 0;
			for (size_t j = 0; j < m_cols; j++)
				maxSize = std::max(maxSize, colVec[j].size());
			
			// Creating a vector of aligned lines
			for (size_t p = 0; p < maxSize; p++)
			{
				std::string row;
				for (size_t k = 0; k < m_cols; k++)
				{
					if (p >= colVec[k].size())				// fill empty column
						colVec[k].push_back(std::string().append(colLength, ' '));
					if (colVec[k][p].size() < colLength)	// fill 'space' void
						colVec[k][p].append(colLength - colVec[k][p].size(), ' ');
					// Add to row
					row.append(colVec[k][p]);
				}
				strOutVec.push_back(row);
			}
		}
	}
	else
		strOutVec.push_back(strOut);
	return strOutVec;
}
// Get string for display
std::string Table::getStringShow(size_t maxLength) const
{
	std::string strOut;
	if (0 && maxLength)
	{
		auto colLength = maxLength / m_cols;
		for (const auto& element : m_elements)
		{
			auto strElem = element->getStringShow(colLength);
			strOut += strElem;
		}
	}
	else	// Original
	{
		for (const auto& str : m_buffer)
			strOut += str + "\n";
	}
	return strOut;
}