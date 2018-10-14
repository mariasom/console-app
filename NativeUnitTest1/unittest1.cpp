#include "stdafx.h"
#include "CppUnitTest.h"
#include <string>

using namespace std;

#define CATCH_CONFIG_MAIN
#include "include/catch.hpp"
#include "..\counter.h"

TEST_CASE("jeden", " ")
{
	string test = "ahoj!";
	REQUIRE(counterl(test) == 5);

}

TEST_CASE("dva", " ")
{
	string test1 = "pocet slov je neznamy";
	REQUIRE(counterw(test1) == 4);

}

TEST_CASE("tri", " ")
{
	string test2 = "ratam \n riadky \n v subore";
	REQUIRE(counterr(test2) == 3);

}

TEST_CASE("templatetest1", " ")
{
	string test = "ahoj!";
	REQUIRE(spocitaj<_char>(test) == 5);

}

TEST_CASE("templatetest2", " ")
{
	string test = "pocet slov je neznamye";
	REQUIRE(spocitaj<_word>(test) == 4);

}

TEST_CASE("templatetest3", " ")
{
	string test = "ratam \n riadky \n v subore";
	REQUIRE(spocitaj<_line>(test) == 3);

}
