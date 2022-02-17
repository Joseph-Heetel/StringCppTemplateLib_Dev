// Testing.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "jhtstring.hpp"
#include <string>
#include <fcntl.h>
#include <io.h>
#include "test_string.hpp"
#include "test_stringiter.hpp"
#include "test_tostringparse.hpp"
#include "test_converter.hpp"

int main()
{
	//_setmode(_fileno(stdout), _O_U16TEXT);
	jht::RunTests_String();
	jht::RunTests_StringIterator();
	jht::RunTests_ToStringParse();
	jht::RunTests_Converter();

	std::cout << "ALL TESTS PASSED";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
