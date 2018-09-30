// app.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <regex>

using namespace std;

int main(int argc, char* argv[])
{
	string s, vstup;
	int l;

	if (argc == 1 || argc > 3)
		return 0;
	else if (argc == 2)
	{
		if (argv[1] == (string)("-c"))
		{
			while (getline(cin, s))
			{
				if (s == "exit")
					break;
				vstup += s;
			}
			cout << "pocet znakov: " << vstup.size() << endl;
		}
		if (argv[1] == (string)("-w"))
		{
			int pocet = 0;
			while (getline(cin, s))
			{
				if (s == "exit")
					break;

				vstup += " " + s;
			}

			l = (int)vstup.size();

			for (int i = 0; i < l - 1; i++)
			{
				if (vstup[i] == ' ' && vstup[i + 1] != ' ')
					pocet++;
			}

			cout << "Pocet slov je: " << pocet << endl;
		}
		if (argv[1] == (string)("-l"))
		{
			int riadky = 0;
			while (getline(cin, s))
			{
				if (s == "exit")
					break;
				riadky++;
			}
			cout << "pocet riadkov: " << riadky << endl;
		}
	}
	else if (argc == 3)
	{
		fstream subor;

		subor.open(argv[2], fstream::in);

		if (!subor.is_open())
		{
			cout << "Subor sa nepodarilo otvorit" << endl;
			return 0;
		}

		if (argv[1] == (string)("-c"))
		{
			while (getline(subor, s)/* && !subor.eof()*/)
			{
				vstup += s;
			}
			cout << "pocet znakov v subore " << argv[2] << ": " << vstup.size() << endl;
		}
		if (argv[1] == (string)("-w"))
		{
			int pocet = 0;
			while (getline(subor, s)/* && !subor.eof()*/)
			{
				vstup += " " + s;
			}

			l = (int)vstup.size();

			for (int i = 0; i < l - 1; i++)
			{
				if (vstup[i] == ' ' && vstup[i + 1] != ' ')
					pocet++;
			}

			cout << "Pocet slov v subore" << argv[2] << " je: " << pocet << endl;
		}
		if (argv[1] == (string)("-l"))
		{
			int riadky = 0;
			while (getline(subor, s)/* && !subor.eof()*/)
			{
				riadky++;
			}
			cout << "pocet riadkov v subore " << argv[2] << ": " << riadky << endl;
		}
		subor.close();
	}

	system("pause");
	return 0;
}