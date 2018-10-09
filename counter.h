#pragma once

#include <iostream>
#include <string>

using namespace std;

typedef struct _char : public string {
	_char(string str) : string(str) {};

	int spocitaj()
	{
		return size();
	};
} ;

typedef struct _word : public string{
	_word(string str) : string(str) {};

	int spocitaj() {
		int pocet = 1;
		if (at(0) == ' ')
			pocet = 0;

		int l = size();

		for (int i = 0; i < l - 1; i++)
		{
			if (at(i) == ' ' && at(i + 1) != ' ')
				pocet++;
		}

		return pocet;
	}
};

typedef struct _line : public string {

	_line(string str) : string(str) {};

	int spocitaj()
	{
		int pocet = 1;
		int l = size();

		for (int i = 0; i < l - 1; i++)
		{
			if (at(i) == '\n')
				pocet++;
		}
		return pocet;
	}
};

template <typename T>
size_t spocitaj(string str)
{
	T result(str);
	return result.spocitaj();
}

int counterl(string vstup)
{
	return vstup.size();
}

int counterw(string vstup)
{
	int pocet = 1;
	if (vstup[0] == ' ')
		pocet = 0;

	int l = (int)vstup.size();

	for (int i = 0; i < l - 1; i++)
	{
		if (vstup[i] == ' ' && vstup[i + 1] != ' ')
			pocet++;
	}

	return pocet;
}

int counterr(string vstup)
{
	int pocet = 1;
	int l = (int)vstup.size();

	for (int i = 0; i < l - 1; i++)
	{
		if (vstup[i] == '\n')
			pocet++;
	}
	return pocet;
}