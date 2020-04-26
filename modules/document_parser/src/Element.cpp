#include <document.h>


// Virtual method for adding information to an element
void Element::add(std::string str)
{
	m_buffer.push_back(str);
}

// Specific display item
void Element::show(size_t maxLength) const
{
	std::string strOut;
	auto strOutVec = getStringRows(maxLength);
	for (const auto& str : strOutVec)
	{
		std::cout << str << std::endl;
	}
}

// Get string vector for display
std::vector<std::string> Element::getStringRows(size_t maxLength) const
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
std::string Element::getStringShow(size_t maxLength) const
{
	std::string strOut;
	for (auto str : m_buffer)
	{
		strOut.append(str);
		if (maxLength > str.size())
			strOut.append("\n");
	}
	if (maxLength)
		for (size_t currMax = maxLength; currMax < strOut.size(); currMax += maxLength)
			strOut.insert(currMax - 1, "\n");
	return strOut;
}