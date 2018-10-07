#include "stdafx.h"
#include "CppUnitTest.h"
#include <string>

using namespace std;

#define CATCH_CONFIG_MAIN
#include "include/catch.hpp"
#include "C:\Users\Meri\Desktop\MPM\ing\1.semester\tvorba_soft_syst\test\counter.h"

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