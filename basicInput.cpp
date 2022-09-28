#include <iostream>
#include <fstream>
#include <xlocale>
#include <codecvt>

#include <string>
#include <xstring>


using std::cout;
using std::wcout;
using std::wstring;

int func2()
{

	std::locale loc(std::locale::classic(), new std::codecvt_utf8<wchar_t>);




	std::wifstream inFile;
	std::wofstream outFile;

	wstring test;
	wstring::size_type foundIdx;

	inFile.open("wideText2.txt");
	outFile.open("wideText3.txt");

	if (inFile.is_open() && outFile.is_open())
	{
		inFile.imbue(loc);
			std::getline(inFile, test);
			
			foundIdx = test.find(L'ا');
			test.insert(test.begin() + 1 + foundIdx, L'\x652');

		outFile.imbue(loc);
			outFile << test;
		


		inFile.close();
		outFile.close();

	}
	else
		throw std::exception("error");




	return 0;
}