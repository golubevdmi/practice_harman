#include <document.h>

// Specific symbols for ignore
std::string Paragraph::ignoreSymbols = std::string();
// Check the string for compliance
bool Paragraph::isElement(const std::string& str)
{
	bool bSpace = std::count(str.begin(), str.end(), ' ') / (double)str.size() > 0.8;
	return !bSpace && str.size() > 1 && !Table::isElement(str, Table::tableSeparator.c_str()) && !AsciiArt::isElement(str);
}

// Get string vector for display
std::vector<std::string> Paragraph::getStringRows(size_t maxLength) const
{
	std::vector<std::string> strOutVec;
	std::string strOut = getStringShow(maxLength);
	if (maxLength)
	{
		for (auto it_sub_begin = strOut.begin(); it_sub_begin != strOut.end(); )
		{
			auto it_sub_end = std::find(it_sub_begin, strOut.end(), '\n');
			auto substr = strOut.substr(std::distance(strOut.begin(), it_sub_begin), std::distance(it_sub_begin, it_sub_end));

			strOutVec.push_back(substr);

			if (it_sub_end != strOut.end())
				it_sub_begin = it_sub_end + 1;
			else
				break;
		}
	}
	else
		strOutVec.push_back(strOut);
	return strOutVec;
}
// Get string for display
std::string Paragraph::getStringShow(size_t maxLength) const
{
	std::string strOut;
	for (auto str : m_buffer)
	{
		strOut.append(str);
		if (!maxLength)
			strOut.append("\n");
	}
	if (maxLength)
	{
		assert(maxLength > getMaxLengthWord());
		for (size_t currMax = maxLength; currMax < strOut.size(); currMax += maxLength)
		{
			auto number = strOut.find_last_of(' ', currMax);
			if (number != std::string::npos)
			{
				strOut.insert(number + 1, "\n");
				currMax = number + 2;			// ignored ' ' and '\n'
			}
		}
	}
	return strOut;
}

// Calculate number of words (excluding ignored characters from Paragraph::ignoreSymbols)
size_t Paragraph::getNumberOfWords() const
{
	size_t nWords = 0;
	if (!m_buffer.empty())
	{
		for (const auto& str : m_buffer)
		{
			size_t nLetter = 0;
			for (const auto& c : str)
			{
				bool isIgnore = false;
				for (const auto& ignoreC : Paragraph::ignoreSymbols)
					if (c == ignoreC)
					{
						isIgnore = true;
						break;
					}
				if (!isIgnore)
					nLetter++;
				else if (nLetter)
				{
					nWords++;
					nLetter = 0;
				}
			}
		}
	}
	return nWords;
}
size_t Paragraph::getMaxLengthWord() const
{
	size_t maxChar = 0;
	size_t nChar = 0;
	std::string strtmp;
	if (!m_buffer.empty())
	{
		for (const auto& str : m_buffer)
		{
			for(const auto& c : str)
			{
				if (c != ' ')
				{
					strtmp += c;
					nChar++;
				}
				else
				{
					if (nChar > maxChar)
						debug_message(("max char " + std::to_string(nChar) + ", " + strtmp).c_str());
					maxChar = std::max(maxChar, nChar);
					nChar = 0;
					strtmp.clear();
				}
			}
		}
	}
	return maxChar;
}