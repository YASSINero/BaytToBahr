#include <algorithm>
#include <fstream>
#include <codecvt>
#include <string>
#include <xlocale>

#include <iostream>

#include "Horoof.h"


using std::wstring;
//====================================================================================

const wchar_t abajad[] = { harakat::getTFatha(),harakat::getTDamma(),harakat::getTKasra() };
const wchar_t harakat[] = { harakat::getFatha(), harakat::getDamma() ,harakat::getKasra() ,harakat::getShadda() , harakat::getSokoon()};

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

bool isWayon(const wchar_t harf/*, const size_t off, const wstring& wstr*/) // wayon as in واي
{
		if (harf == harf::getAlif() || harf == harf::getWaw() || harf == harf::getYa2() || harf == harf::getShortAlif())
			return true;

	return false;
}


int main()
{
	std::locale loc(std::locale::classic(), new std::codecvt_utf8<wchar_t>);

	std::wofstream fout;	fout.imbue(loc);
	std::wifstream fin;		fin.imbue(loc);

	wstring sin, verse = L"ويَأْتِي الفِطْرُ بَعْدَ الصَّوْمِ عيداً";

	fout.open("foutFile.txt");						//	OUTPUT SECTION
	fout << verse;											//	<<<<<<<<<<<<<<
	fout.close();											//	xxxxxxxxxxxxxx


	fin.open("foutFile.txt");								//	INPUT SECTION
	std::getline(fin, sin);
	fin >> sin;														//	>>>>>>>>>>>>>


	fout.open("foutFile.txt", std::ios_base::app);	//	<<<<<<<<<<<<<
	fout << '\n' << sin;											//	<<<<<<<<<<<<<


	std::getline(fin, sin);									//	>>>>>>>>>>>>>

	for (size_t i = 0; i<sin.size(); i++)
	{
		//if(sin[i] == L'ة' && i+1<sin.size() && (i+1 == sin.size()-1 || sin[i+1] == L' '))
		if (sin[i] == L'ة')	// tiedTa2 process isn't perfect
		{
			if(i == sin.size()-1)
			{
				sin.replace(i, 1, 1, L'ه');
				sin.insert(sin.begin() + 1 + i, harakat::getSokoon());
			}
			if (i + 1 < sin.size()-1 && sin[i + 1] == harakat::getTFatha())
			{
				sin.replace(i, 1, 1, L'ت');
				sin.replace(i+1, 1, 1, harakat::getFatha());
				sin.insert(sin.begin() + 2 + i, L'ن');
				sin.insert(sin.begin() + 3 + i, harakat::getSokoon());
			}
		}
		

		switch (sin[i])
		{
		case harf::getShortAlif():	//short alif

				// own's letter harf process
			if (i == sin.size() - 1 /* || i == sin.size() - 2 */)
				sin.insert(sin.begin() + 1 + i, harakat::getSokoon());
			else
			{
				/*
				if (sin[i - 1] != L'?' && isHaraka(sin[i - 1]))
				{
					sin.replace(i-1, 1, 1, L'?');
				}	// haraka correction (soukoun insertion below)
				*/
				if (sin[i - 1] != harakat::getFatha() && !isHaraka(sin[i - 1]))
				{
					if (i + 1 < sin.size() && !isTanween(sin[i + 1]))
					{
						sin.insert(sin.begin() + i, harakat::getFatha());
						i++;	// this fixes the 112 bug
					}
				}		// giving fatha to preceeding harf

				if (i + 1 < sin.size() - 1 && isTanween(sin[i + 1]))
				{
					sin.replace(i, 1, 1, harakat::getFatha());
					sin.replace(i + 1, 1, 1, L'ن');
					sin.insert(sin.begin() + 2 + i, harakat::getSokoon());
				}	// tanween materialization if not in the end
				else if (i + 1 == sin.size() - 1 && isTanween(sin[i + 1]))
				{
					sin.replace(i + 1, 1, 1, harakat::getSokoon());
				}	// replacing last tanween with soukoun
				else	
				{
					sin.insert(sin.begin() + 1 + i, harakat::getSokoon());
					// inserting soukoun
				}
				//AFTER INSERTING A wchar the wstring gets longer by 1
					// i.e. sin.size() => sin.size()+1
						// => the current i in the iteration is off by one

			}	// END own's letter harf process
		break;



		case harf::getAlif():	// Alif

			if (i == sin.size() - 1 || sin[i + 1] == L' ') //check last letter is alif
			{
				sin.insert(sin.begin() + 1 + i, harakat::getSokoon());
				if (!isHaraka(sin[i - 1]))
				{
					if (sin[i - 1] == harf::getWaw() && sin[i - 2] != L' ')	//check waw's surroundings
					{
						sin.insert(sin.begin() + i, harakat::getSokoon());	//=> it's waw ljam3
						i++;
					}
					else
					{
						sin.insert(sin.begin() + i, harakat::getFatha());
						i++;
					}
				}
			}
			else
			{
				/*if(i == sin.size() - 1 || sin[i+1] == L' ')	//check alif in the end 
				
					if(sin[i-1] == harf::getWaw() && sin[i - 2] != L' ')	//check waw preceding it
					{
						sin.insert(sin.begin() + i, harakat::getSokoon());	//=> it's waw ljam3
						i++;
					}*/
				
				if(sin[i-1] != L' ' && !isTanween(sin[i+1]) && !isHaraka(sin[i - 1]))
				{
					sin.insert(sin.begin() + i, harakat::getFatha());
					i++;
				}

				if (i+1 == sin.size()-1 && isTanween(sin[i + 1]))
				{
					sin.replace(i + 1, 1, 1, harakat::getSokoon());
					sin.insert(sin.begin() + i, harakat::getFatha());
					i++;
				}	// replacing last tanween with soukoun
				else if(i + 1 < sin.size() - 1 && isTanween(sin[i + 1]))
				{
					sin.replace(i, 1, 1, harakat::getFatha());
					sin.replace(i + 1, 1, 1, L'ن');
					sin.insert(sin.begin() + 2 + i, harakat::getSokoon());
				}	// tanween materialization if not in the end
				else
				{
					// then alif is followed by a harf
					//if(sin[])
					sin.insert(sin.begin() + 1 + i, harakat::getSokoon());
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
		case harf::getYa2(): case harf::getWaw():	//Todo: Add Qafia support for tanwin in the end
			
			if (i == 0)
				continue; //Continue because arabic doesnt start with a saak'in

			if(sin[i+1] == harf::getShortAlif())
			{
				if(!isTanween(sin[i+2]))
					sin.insert(sin.begin() + 1 + i, harakat::getFatha());
			}	// WTF?? maybe sin[i+1] == alif ?

			if (i + 1 < sin.size() && !isHaraka(sin.at(i + 1)) && !isHaraka(sin[i - 1]) && sin.at(i - 1) != L' ')
					{
						sin.insert(sin.begin() + 1 + i, harakat::getSokoon());

						sin.insert(sin.begin() + i, sin[i] == harf::getWaw() ? harakat::getDamma() : harakat::getKasra());
						i++;
					}	// giving haraka to preceeding harf according to cond.
			else if (i == sin.size() - 1)
					{
						sin.insert(sin.begin() + 1 + i, harakat::getSokoon());
						if(!isHaraka(sin[i - 1]))
						{
							sin.insert(sin.begin() + i, sin[i] == harf::getWaw() ? harakat::getDamma() : harakat::getKasra());
							i++;
						}
					}
			
			break;

		case harakat::getTKasra(): case harakat::getTDamma():
			if (i == sin.size() - 1)
			{
				// this line replaces tanween by haraka
				sin.replace(i, 1, 1, sin[i] == harakat::getTKasra() ? harakat::getKasra() : harakat::getDamma());
				if (sin[i - 1] == L'ة')	
				{	//Ta2 changes if not in the end
					sin.replace(i - 1, 1, 1, L'ت');	//by ta2 mabsouta
				}
				sin.insert(sin.begin() + 1 + i, sin[i] == harakat::getKasra() ? harf::getYa2() : harf::getWaw());
				sin.insert(sin.begin() + 2 + i, harakat::getSokoon());
			}
			else
			{
				sin.replace(i, 1, 1, sin[i] == harakat::getTKasra() ? harakat::getKasra() : harakat::getDamma());
				sin.insert(sin.begin() + 1 + i, L'ن');
				sin.insert(sin.begin() + 2 + i, harakat::getSokoon());
			}
			break;

		case harakat::getShadda():
			if(isWayon(sin[i+1]))	// ابّان
			{
				sin.replace(i, 1, 1, harakat::getSokoon());
				sin.insert(sin.begin() + 1 + i, sin[i-1]);
				switch (sin[i+2])
				{
				case harf::getYa2(): case harf::getWaw():
					sin.insert(sin.begin() + 2 + i, sin[i + 2] == harf::getWaw() ? harakat::getDamma() : harakat::getKasra());
					break;
				case harf::getAlif(): case harf::getShortAlif():
					sin.insert(sin.begin() + 2 + i, harakat::getFatha());
					break;
				default: ;
				}
			}
			else
			{
				sin.replace(i, 1, 1, harakat::getSokoon());
				sin.insert(sin.begin() + 1 + i, sin[i - 1]);
			}
			break;

			default: ;
		}
	}

	fout << '\n' << sin;							//	<<<<<<<<<<<<<





	fout.close();									//	xxxxxxxxxxxxx
	return 0;
}