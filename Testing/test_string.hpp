#pragma once
#include "jhtstring.hpp"
#include <cassert>
#include <iostream>

namespace jht
{
	inline void RunTests_String()
	{
		std::cout << "RunTests_String\n";

		{ // Constructors
			String str0;
			assert(!str0.IsManaged());
			assert(str0.Length() == 0);
			assert(strcmp(str0.ConstData(), "") == 0);

			String str1 = "a const char* cstr";
			assert(!str1.IsManaged());
			assert(str1.Length() == strlen("a const char* cstr"));
			assert(strcmp(str1.ConstData(), "a const char* cstr") == 0);

			String str2("a const char* cstr", 10);
			assert(!str2.IsManaged());
			assert(str2.Length() == 10);
			assert(str2 == String("a const ch"));
		}

		{ // Static constructors
			std::string_view strview("a const char* cstr");

			String str0 = String::MakeView(strview);
			assert(!str0.IsManaged());
			assert(str0.Length() == strview.length());
			assert(strcmp(str0.ConstData(), "a const char* cstr") == 0);

			String str1 = String::MakeView("a const char* cstr");
			assert(!str1.IsManaged());
			assert(str1.Length() == strlen("a const char* cstr"));
			assert(strcmp(str1.ConstData(), "a const char* cstr") == 0);

			String str2 = String::MakeView("a const char* cstr", 10);
			assert(!str2.IsManaged());
			assert(str2.Length() == 10);
			assert(str2 == String("a const ch"));

			String str3 = String::MakeManaged('a', 14);
			assert(str3.IsManaged());
			assert(str3.Length() == 14);
			for (int i = 0; i < 14; i++)
			{
				assert(str3[i] == 'a');
			}

			String str4 = String::MakeManaged("a const char* cstr");
			assert(str4.IsManaged());
			assert(str4.Length() == strlen("a const char* cstr"));
			assert(strcmp(str4.ConstData(), "a const char* cstr") == 0);

			String str5 = String::MakeManaged("a const char* cstr", 10);
			assert(str5.IsManaged());
			assert(str5.Length() == 10);
			assert(str5 == String("a const ch"));

			String str6 = String::MakeManaged(64);
			assert(str6.IsManaged());
			assert(str6.Length() == 64);

			String str7 = String::MakeManaged(strview);
			assert(str7.IsManaged());
			assert(str7.Length() == strview.length());
			assert(strcmp(str7.ConstData(), strview.data()) == 0);
		}

		{ // Copying
			String str0;
			{
				String str1 = String::MakeView("a const char* cstr");
				str0 = str1.MakeCopy();
			}
			assert(str0.IsManaged());
		}

		{ // Comparison
			String str0 = "a";
			String str1 = "b";
			String str2 = "c";
			String str3 = "ab";
			String str4 = "bc";
			String str5 = "cd";

			assert(str0 == str0);
			assert(str1 == str1);
			assert(str2 == str2);
			assert(str3 == str3);
			assert(str4 == str4);
			assert(str5 == str5);
			assert(str0 != str1);
			assert(String::Compare(str0, str1) == clamp(strcmp(str0.ConstData(), str1.ConstData()), -1, 1));
			assert(String::Compare(str1, str2) == clamp(strcmp(str1.ConstData(), str2.ConstData()), -1, 1));
			assert(String::Compare(str2, str3) == clamp(strcmp(str2.ConstData(), str3.ConstData()), -1, 1));
			assert(String::Compare(str3, str4) == clamp(strcmp(str3.ConstData(), str4.ConstData()), -1, 1));
			assert(String::Compare(str4, str5) == clamp(strcmp(str4.ConstData(), str5.ConstData()), -1, 1));
			assert(String::Compare(str5, str0) == clamp(strcmp(str5.ConstData(), str0.ConstData()), -1, 1));
			assert(String::Compare(str0, str3) == clamp(strcmp(str0.ConstData(), str3.ConstData()), -1, 1));
			assert(String::Compare(str1, str4) == clamp(strcmp(str1.ConstData(), str4.ConstData()), -1, 1));
			assert(String::Compare(str2, str5) == clamp(strcmp(str2.ConstData(), str5.ConstData()), -1, 1));
			assert(String::Compare(str3, str0) == clamp(strcmp(str3.ConstData(), str0.ConstData()), -1, 1));
			assert(String::Compare(str4, str1) == clamp(strcmp(str4.ConstData(), str1.ConstData()), -1, 1));
			assert(String::Compare(str5, str2) == clamp(strcmp(str5.ConstData(), str2.ConstData()), -1, 1));
			//assert(String::Compare(str0, str1) == strcmp(str0.ConstData(), str1.ConstData()));
			//assert(String::Compare(str1, str2) == strcmp(str1.ConstData(), str2.ConstData()));
			//assert(String::Compare(str2, str3) == strcmp(str2.ConstData(), str3.ConstData()));
			//assert(String::Compare(str3, str4) == strcmp(str3.ConstData(), str4.ConstData()));
			//assert(String::Compare(str4, str5) == strcmp(str4.ConstData(), str5.ConstData()));
			//assert(String::Compare(str5, str0) == strcmp(str5.ConstData(), str0.ConstData()));
			//assert(String::Compare(str0, str3) == strcmp(str0.ConstData(), str3.ConstData()));
			//assert(String::Compare(str1, str4) == strcmp(str1.ConstData(), str4.ConstData()));
			//assert(String::Compare(str2, str5) == strcmp(str2.ConstData(), str5.ConstData()));
			//assert(String::Compare(str3, str0) == strcmp(str3.ConstData(), str0.ConstData()));
			//assert(String::Compare(str4, str1) == strcmp(str4.ConstData(), str1.ConstData()));
			//assert(String::Compare(str5, str2) == strcmp(str5.ConstData(), str2.ConstData()));
		}

		{ // Fill operation
			String templ = "__________";
			String entry = "01234";

			String str0 = templ.MakeCopy();
			str0.Fill('+');
			JHT_ITERATE(jht::String, str0, iter)
			{
				assert(*iter == '+');
			}

			String str1 = templ.MakeCopy();
			str1.Fill(entry);
			assert(str1 == "01234_____");

			String str2 = templ.MakeCopy();
			str2.Fill(entry, 3);
			assert(str2 == "___01234__");
		}

		{ // Split
			String templ0 = "I \nlike\n Ns \t\n  Ps";
			String templ1 = "\n\n";
			String templ2 = "\n .\n\n";

			std::vector<String> testvector;
			templ0.Split('\n', testvector);
			assert(testvector.size() == 4);
			assert(testvector[0] == "I ");
			assert(testvector[1] == "like");
			assert(testvector[2] == " Ns \t");
			assert(testvector[3] == "  Ps");

			testvector.clear();
			templ1.Split('\n', testvector);
			assert(testvector.size() == 0);

			testvector.clear();
			templ2.Split('\n', testvector);
			assert(testvector.size() == 1);
			assert(testvector[0] == " .");
		}

		{ // SubString
			String templ = "0123456789";

			String str0 = templ.SubString(0);
			assert(str0 == templ);

			String str1 = templ.SubString(1, 3);
			assert(str1 == "123");

			String str2 = templ.SubString(0, 9);
			assert(str2 == "012345678" && str2 != templ);

			String str3 = templ.SubString(9);
			assert(str3 == "9");

			String str4 = templ.SubString(95);
			assert(str4.IsEmpty());

			String str5 = templ.SubString(0, 0);
			assert(str5.IsEmpty());
		}

		{ // Trimming
			String templ0 = "\u0009\u000A\u000B\u000C\u000D\u0020.\u0009\u000A\u000B\u000C\u000D\u0020";
			String templ1 = "\u0009\u000A\u000B\u000C\u000D\u0020";
			String templ2;

			String str0 = templ0.Trimmed();
			assert(str0 == ".");

			String str1 = templ1.Trimmed();
			assert(str1.IsEmpty());

			String str2 = templ2.Trimmed();
			assert(str2.IsEmpty());
		}
	}
}