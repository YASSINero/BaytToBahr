#include <iostream>
#include <fcntl.h>
#include <io.h>
#include <fstream>
#include <stdio.h>
#include <xlocale>
#include <codecvt>

#include <string>


using std::cout;
using std::wstring;
using std::ios;


int func()
{
	std::locale loc(std::locale::classic(), new std::codecvt_utf8<wchar_t>);


	const wstring kalima = L"ابابااب", jomla = L"اَحْمَدٌ", harakat = L"بَبُبِبْبًبٌبٍبّ";

	std::wifstream inFile;
	wstring test;


	FILE* fPtr;
	std::wofstream outFile;



	outFile.open("wideText2.txt");
	fopen_s(&fPtr, "wideText.txt", "w+");


	_setmode(_fileno(fPtr), _O_U8TEXT);
	fwprintf(fPtr, L"%s\n", kalima.c_str());



	outFile.imbue(loc);
	outFile << kalima << '\n';
	outFile << L"kalima size: " << kalima.size() << '\n';
	outFile << L"kalima front: " << kalima.front() 
		 << L" === kalima back: " << kalima.back() << '\n';

for(size_t i = 0; i<kalima.size(); i++)
	outFile << L"kalima at(" << i << L"):" << kalima.at(i) << '\n';


/*========seperation=======*/	outFile << "\n\n\n";


	outFile << jomla << '\n';
	outFile << L"jomla size: " << jomla.size() << '\n';
	outFile << L"jomla front: " << jomla.front()
		 << L" === jomla back: " << jomla.back() << '\n';

for (size_t i = 0; i < jomla.size(); i++)
	outFile << L"jomla at(" << i << L"):" << jomla.at(i) << '\n';

	if (jomla.at(1) == L'َ')
		outFile << L"true\n";
	else
		outFile << L"false\n";


/*========seperation=======*/	outFile << "\n\n\n";


	outFile << harakat << '\n';
	outFile << L"harakat size: " << harakat.size() << '\n';
	outFile << L"harakat front: " << harakat.front()
		 << L" === harakat back: " << harakat.back() << '\n';

	for (size_t i = 0; i < harakat.size(); i++)
		outFile << L"harakat at(" << i << L"):" << harakat.at(i) << '\n';

	if (harakat.at(1) == L'َ')
		outFile << L"true\n";
	else
		outFile << L"false\n";
	

	
	//std::getline(inFile, test);


	;fclose(fPtr);
	outFile.close();



	return 0;
}