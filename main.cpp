// app.cpp : Defines the entry point for the console application.
//


#include <fstream>
#include <vector>
#include <sstream>
#include <regex>

#include "counter.h"


int main(int argc, char* argv[])
{
	string s, vstup;
	int l,g;

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

			cout << "pocet znakov: " << counterl(vstup) << endl;
		}
		if (argv[1] == (string)("-w"))
		{
			while (getline(cin, s))
			{
				if (s == "exit")
					break;

				vstup += " " + s;
			}

			cout << "Pocet slov je: " << counterw(vstup) << endl;
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
			cout << "pocet znakov v subore " << argv[2] << ": " << counterl(vstup) << endl;
		}
		if (argv[1] == (string)("-w"))
		{
			while (getline(subor, s)/* && !subor.eof()*/)
			{
				vstup += " " + s;
			}

			cout << "Pocet slov v subore" << argv[2] << " je: " << counterw(vstup) << endl;
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