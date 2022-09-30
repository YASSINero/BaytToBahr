#pragma once
#ifndef HOROOF_H
#define HOROOF_H


class harf
{

public:

	static constexpr auto getAlif()
	{
		return L'ا';
	}

	static constexpr auto getShortAlif()
	{
		return L'ى';
	}
	static constexpr auto getWaw()
	{
		return L'و';
	}
	static constexpr auto getYa2()
	{
		return L'ي';
	}
};

class harakat
{
public:
	static constexpr auto getFatha()
	{
		return L'َ';
	}

	static constexpr auto getDamma()
	{
		return L'ُ';
	}
	static constexpr auto getKasra()
	{
		return L'ِ';
	}
	static constexpr auto getShadda()
	{
		return L'ّ';
	}
	static constexpr auto getSokoon()
	{
		return L'ْ';
	}

	//======TANWEEN======

	static constexpr auto getTFatha()
	{
		return L'ً';
	}

	static constexpr auto getTDamma()
	{
		return L'ٌ';
	}
	static constexpr auto getTKasra()
	{
		return L'ٍ';
	}
};

#endif
