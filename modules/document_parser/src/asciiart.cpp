#include <document.h>

// Specific symbols for ascii arts
std::string AsciiArt::asciiArtSymbols = std::string();
// Check the string for compliance
bool AsciiArt::isElement(const std::string& str)
{
	if (!str.empty())
	{
		size_t sum = 0;
		if ((double)std::count(str.begin(), str.end(), ' ') / (double)str.size() > 0.95)
			return false;
		for (const auto& c : AsciiArt::asciiArtSymbols)
			sum += std::count(str.begin(), str.end(), c);
		if ((double)sum / (double)str.size() > 0.8)
			return true;
	}
	return false;
}