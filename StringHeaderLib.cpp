// StringHeaderLib.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "JHTString.hpp"
#include <string>
#include <fcntl.h>
#include <io.h>

class Tester : public jht::Consumer<char32_t>
{
public:
	void PushItem(const char32_t& item)
	{
		std::cout << jht::ToString(static_cast<uint32_t>(item), 16) << std::endl;
	}
};

int main()
{
	_setmode(_fileno(stdout), _O_U16TEXT);

	jht::String test = "Hello there";
	jht::String test2 = test.AsManaged();
	jht::String test3 = test.SubString(4, 3);
	jht::StringBuilder builder;
	builder.AppendLine(69);
	builder.AppendLine(-420);
	builder.AppendLine(-69.420);

	std::wcout << L"kekd\n";

	static const char* unicode = reinterpret_cast<const char*>(u8"ờ");

	jht::StringCharProducer<char> prod(jht::String::MakeView(unicode));
	jht::DecoderUTF8<jht::StringCharProducer<char>> decoder(prod);

	using vectorconsumer = jht::StandardContainerConsumer<std::vector<wchar_t>>;
	std::vector<wchar_t> output;
	vectorconsumer consumer(output);
	jht::TranscodeUTF8toUTF16 < jht::StringCharProducer<char> , vectorconsumer, true, jht::InvalidCodeHandling::Throw > (prod, consumer);
	output.push_back(L'\0');
	const wchar_t* umm = output.data();
	std::wcout << output.data();
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
