#pragma once
#include "streams.hpp"
#include "stringbuilder.hpp"

namespace jht {
	std::ostream& jht::operator<<(std::ostream& left, const String& right)
	{
		left.write(right.Data(), right.Length());
		return left;
	}

	String GetLine(std::istream& in)
	{
		if (in.bad() || in.eof())
		{
			return String();
		}

		const size_t strsize = 64;
		size_t writeindex = 0;
		StringBuilder builder;
		String buffer = String::MakeManaged('\0', strsize + 1);

		bool encounteredEnd = false;
		while (true) {
			if (!in.good() || in.eof()) {
				builder.Append(buffer);
			}
			while (true)
			{

				char c;
				in.read(&c, 1);
				if (c == '\n')
				{
					break;
				}
				if (writeindex == strsize)
				{
					builder.Append(buffer.MakeCopy());
					buffer.Fill('\0');
					writeindex = 0;
				}
				buffer.Data()[writeindex] = c;
				writeindex++;
			}
		}
		return buffer;
	}
}