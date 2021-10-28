// StringHeaderLib.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "StringHeaderOnly.hpp"
#include <string>

int main()
{
	joheet::String test = "Hello there";
	joheet::String test2 = test.AsManaged();
	joheet::String test3 = test.SubString(4, 3);
	joheet::StringBuilder builder;
	builder.Append("\n");
	builder << test << "\n" << test2 << "\n" << joheet::String::MakeManaged('-', 15) << "\n" << 69.420;
	std::cout << builder.Build() << "\n";
	JOHEET_ITERATE(joheet::String, builder.Build(), iterator)
	{
		std::cout << *iterator;
	}
	std::cout << "\n\n";
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
