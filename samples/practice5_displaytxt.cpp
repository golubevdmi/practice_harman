#include <iostream>

#include <document.h>

// For parsing elements
std::string asciiArtSymbols = " <>:.,*-+=`~@#%^&()!?[]{}'\"/|\\_";
std::string ignoreSymbols = " \n\t;:.,*+=`~@#%^&()!?[]{}'\"/|\\";
std::string tableSeparator = "|";

// File
std::string path = "D:\\Dropbox\\Workspace\\Harman\\Automotive_Internship\\Practice\\practice_harman\\data\\ascii_art\\";
std::string file_name = "words32_letters112.txt";

// Console rows length
const size_t maxLength = 90;

int main(int argc, const char* argv[])
{
	AsciiArt::asciiArtSymbols = asciiArtSymbols;
	Paragraph::ignoreSymbols = ignoreSymbols;
	Table::tableSeparator = tableSeparator;

	Document doctxt;
	if (doctxt.init(path + file_name))
	{
		doctxt.parse();
		std::cout
			<< "File: " << file_name << "\n"
			<< "size: " << doctxt.size() << "\n"
			<< "size words: " << doctxt.size_words() << "\n"
			<< "size table: " << doctxt.size_table() << "\n"
			<< "size ascii: " << doctxt.size_ascii() << "\n"
			<< "size paragraph: " << doctxt.size_paragraph() << "\n"
			<< "size undefine elements: " << doctxt.size_undef() << "\n"
			<< std::endl;

		doctxt.show(maxLength);
		//doctxt.show_tables(maxLength);
		//doctxt.show_ascii(maxLength);
		//doctxt.show_paragraph(maxLength);
		//doctxt.show_undef(maxLength);
		return 0;
	}
	return -1;
}