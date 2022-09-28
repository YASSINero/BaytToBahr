#include <algorithm>
#include <fstream>
#include <codecvt>
#include <string>
#include <xlocale>

#include <iostream>


using std::wstring;
//====================================================================================

const wchar_t abajad[] = {L'ً',L'ٌ',L'ٍ'};
const wchar_t harakat[] = {L'َ', L'ُ',L'ِ',L'ّ', L'ْ'};

bool isTanween(const wchar_t harf)
{
	return std::any_of(std::begin(abajad), std::end(abajad), [harf](const wchar_t h){return h == harf;});
	/*for (const auto c : abajad)
	{
		if(harf == c)
			return true;
	}
	 false;*/
}

bool isHaraka(const wchar_t harf)
{
	return std::any_of(std::begin(harakat), std::end(harakat), [harf](const wchar_t h) {return h == harf; });
	/*for (const auto c : harakat)
	{
		if (harf == c)
			return true;
	}
	return false;*/
}

bool isWayon(const wchar_t harf/*, const size_t off, const wstring& wstr*/) // wayon as in ????
{
		if (harf == L'ا' || harf == L'و' || harf == L'ي' || harf == L'ى')
			return true;

	return false;
}


int main()
{
	std::locale loc(std::locale::classic(), new std::codecvt_utf8<wchar_t>);

	std::wofstream fout;	fout.imbue(loc);
	std::wifstream fin;		fin.imbue(loc);

	wstring sin, verse = L"وبعده يا أيها الجواد";

	fout.open("foutFile.txt");						//	OUTPUT SECTION
	fout << verse;									//	<<<<<<<<<<<<<<
	fout.close();									//	xxxxxxxxxxxxxx


	fin.open("foutFile.txt");						//	INPUT SECTION
	std::getline(fin, sin);
	fin >> sin;										//	>>>>>>>>>>>>>


	fout.open("foutFile.txt", std::ios_base::app);	//	<<<<<<<<<<<<<
	fout << '\n' << sin;							//	<<<<<<<<<<<<<


	std::getline(fin, sin);							//	>>>>>>>>>>>>>
	//sin.replace(sin.begin(), sin.end(), L'?', L"??");

	for (size_t i = 0; i<sin.size(); i++)
	{
		switch (sin[i])
		{
		case L'ى':

				// own's letter harf process
			if (i == sin.size() - 1 /* || i == sin.size() - 2 */)
				sin.insert(sin.begin() + 1 + i, L'ْ');
			else
			{
				/*
				if (sin[i - 1] != L'?' && isHaraka(sin[i - 1]))
				{
					sin.replace(i-1, 1, 1, L'?');
				}	// haraka correction (soukoun insertion below)
				*/
				if (sin[i - 1] != L'َ' && !isHaraka(sin[i - 1]))
				{
					if (i + 1 < sin.size() && !isTanween(sin[i + 1]))
					{
						sin.insert(sin.begin() + i, L'َ');
						i++;	// this fixes the 112 bug
					}
				}		// giving fatha to preceeding harf

				if (i + 1 < sin.size() - 1 && isTanween(sin[i + 1]))
				{
					sin.replace(i, 1, 1, L'َ');
					sin.replace(i + 1, 1, 1, L'ن');
					sin.insert(sin.begin() + 2 + i, L'ْ');
				}	// tanween materialization if not in the end
				else if (i + 1 == sin.size() - 1 && isTanween(sin[i + 1]))
				{
					sin.replace(i + 1, 1, 1, L'ْ');
				}	// replacing last tanween with soukoun
				else	
				{
					sin.insert(sin.begin() + 1 + i, L'ْ');
					// inserting soukoun
				}
				//AFTER INSERTING A wchar the wstring gets longer by 1
					// i.e. sin.size() => sin.size()+1
						// => the current i in the iteration is off by one

			}	// END own's letter harf process
		break;



		case L'ا':

			if (i == sin.size() - 1)
			{
				sin.insert(sin.begin() + 1 + i, L'ْ');
				if (!isHaraka(sin[i - 1]))
				{
					sin.insert(sin.begin() + i, L'َ');
				}
			}
			else
			{
				if(sin[i-1] != L' ' && !isTanween(sin[i+1]) && !isHaraka(sin[i - 1]))
				{
					sin.insert(sin.begin() + i, L'َ');
					i++;
				}

				if (i+1 == sin.size()-1 && isTanween(sin[i + 1]))
				{
					sin.replace(i + 1, 1, 1, L'ْ');
					sin.insert(sin.begin() + i, L'َ');
				}	// replacing last tanween with soukoun
				else if(i + 1 < sin.size() - 1 && isTanween(sin[i + 1]))
				{
					sin.replace(i, 1, 1, L'َ');
					sin.replace(i + 1, 1, 1, L'ن');
					sin.insert(sin.begin() + 2 + i, L'ْ');
				}	// tanween materialization if not in the end
				else
				{
					// then alif is followed by a harf
					sin.insert(sin.begin() + 1 + i, L'ْ');
					//sin.insert(sin.begin() + i, L'?');
				}

				/*
				
				for (const auto c : harakat)
				{
					if (i > 0 && sin[i + 1] != c)
					{
						sin.insert(sin.begin() + 1 + i, L'?');
						break;
					}
				}*/
				
			}
		break;
		case L'ي': case L'و':	//Todo: work on decompressing chadda
			
			if (i == 0)
				continue; //Continue because arabic doesnt start with a saak'in

			if(sin[i+1] == L'ا')
			{
				sin.insert(sin.begin() + 1 + i, L'َ');
			}	// WTF?? maybe sin[i+1] == alif ?

			if (i + 1 < sin.size() && !isHaraka(sin.at(i + 1)) && !isHaraka(sin[i - 1]) && sin.at(i - 1) != L' ')
					{
						sin.insert(sin.begin() + 1 + i, L'ْ');

						sin.insert(sin.begin() + i, sin[i] == L'و' ? L'ُ' : L'ِ');

					}	// giving haraka to preceeding harf according to cond.
			else if (i == sin.size() - 1)
					{
						sin.insert(sin.begin() + 1 + i, L'ْ');
						if(!isHaraka(sin[i - 1]))
						{
							sin.insert(sin.begin() + i, sin[i] == L'و' ? L'ُ' : L'ِ');
						}
					}
			
			break;

		case L'ٍ': case L'ٌ':
			if (i == sin.size() - 1)
			{
				sin.replace(i, 1, 1, sin[i] == L'ٍ' ? L'ِ' : L'ُ');
				sin.insert(sin.begin() + 1 + i, L'ْ');
			}
			else
			{
				sin.replace(i, 1, 1, sin[i] == L'ٍ' ? L'ِ' : L'ُ');
				sin.insert(sin.begin() + i, L'ن');	 i++;
				sin.insert(sin.begin() + 1 + i, L'ْ');
			}
			break;

			default: ;
		}
	}

	fout << '\n' << sin;							//	<<<<<<<<<<<<<





	fout.close();									//	xxxxxxxxxxxxx
	return 0;
}