#pragma once

#include <iostream>
#include <string>

using namespace std;

int counterl(string vstup)
{
	return vstup.size();
}

int counterw(string vstup)
{
	int pocet = 0;
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
	//dooncit
	return 0;
}