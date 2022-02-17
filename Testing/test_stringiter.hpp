#pragma once
#include "jhtstring.hpp"
#include <cassert>
#include <iostream>

namespace jht
{
	inline void RunTests_StringIterator()
	{
		std::cout << "RunTests_StringIterator\n";

		{ // Basic function
			const String str0;
			const String str1 = "0123";
			String str2 = str1.MakeCopy();

			bool test = true;
			JHT_ITERATE(jht::String, str0, iter)
			{
				test = false;
			}
			assert(test);

			int index = 0;
			JHT_ITERATE(String, str1, iter)
			{
				assert(iter.Index() == index);
				assert(*iter == str1[index]);
				assert(iter.Current() == str1[index]);
				assert(iter.Ptr()[0] == str1[index]);
				index++;
			}
		}
	}
}