#pragma once
#include "tryparse.hpp"

namespace jht
{
	inline bool TryParse(const String& val, int64_t& out, int64_t inject)
	{
		out = inject;

		const String in = val.Trimmed();
		if (in.IsEmpty())
		{
			return false;
		}

		int32_t radius = 10;

		// try find a sign if any

		int32_t index = 0;
		int32_t lengthcached = static_cast<int32_t>(in.Length());

		bool isneg = false;
		bool foundNumStart = false;

		for (index = 0; index < lengthcached; index++)
		{
			char c = in[index];

			if (c == '-')
			{
				isneg = true;
				index++;
				break;
			}
			else if (c >= '0' || c <= ('0' + radius))
			{
				foundNumStart = true;
				break;
			}
			else
			{
				return false;
			}
		}

		// Find the first number

		if (!foundNumStart)
		{
			for (; index < lengthcached; index++)
			{
				char c = in.ConstData()[index];

				if (String::IsWhitespace(c))
				{
					continue;
				}
				if (c >= '0' || c <= ('0' + radius))
				{
					foundNumStart = true;
					break;
				}
				else
				{
					return false;
				}
			}
		}

		// parse the number

		uint64_t uintval = 0;

		if (TryParse(in.SubString(index), uintval))
		{
			out = static_cast<int64_t>(uintval);
			if (isneg)
			{
				out = -out;
			}
			return true;
		}

		return false;
	}

	bool TryParse(const String& val, uint64_t& out, uint64_t inject)
	{
		out = inject;

		const String in = val.Trimmed();
		if (in.IsEmpty())
		{
			return false;
		}

		int32_t lengthcached = static_cast<int32_t>(in.Length());

		int32_t radius = 10;

		uint64_t parse = 0;
		for (int32_t index = 0; index < lengthcached; index++)
		{
			char c = in.ConstData()[index];
			if (c < '0' || c >('0' + radius))
			{
				return false;
			}
			parse *= 10;
			parse += static_cast<uint64_t>(c) - static_cast<uint64_t>('0');
		}
		out = parse;
		return true;
	}

	bool TryParse(const String& val, double& out, double inject)
	{
		out = inject;

		const String in = val.Trimmed();
		if (in.IsEmpty())
		{
			return false;
		}

		String integralstr;
		String fractionalstr;

		std::vector<String> sections;
		in.Split('.', sections, true);

		if (sections.size() == 2)
		{
			integralstr = sections[0];
			fractionalstr = sections[1];
		}
		else if (sections.size() == 1)
		{
			integralstr = sections[0];
		}
		else
		{
			return false;
		}

		double integralresult = 0;

		int64_t intval = 0;

		if (TryParse(integralstr, intval))
		{
			integralresult = static_cast<double>(intval);
		}
		else
		{
			return false;
		}

		if (fractionalstr.IsEmpty())
		{
			out = integralresult;
			return true;
		}

		double fractionalresult = 0;
		int32_t radius = 10;

		for (int32_t index = static_cast<int32_t>(fractionalstr.Length() - 1); index >= 0; index--)
		{
			char c = fractionalstr.ConstData()[index];
			if (c < '0' || c >('0' + radius))
			{
				return false;
			}
#pragma warning (disable: 26451) // Arithmetic overflow warning: c is guaranteed to be at least '0' and at most '9'
			fractionalresult += static_cast<double>(c - '0');
#pragma warning (default: 26451)
			fractionalresult /= 10.0;
		}

		if (integralresult < 0)
		{
			out = integralresult - fractionalresult;
		}
		else
		{
			out = integralresult + fractionalresult;
		}
		return true;
	}

	bool TryParse(const String& val, bool& out, bool inject)
	{
		if (val.SubString(0, 5) == "false")
		{
			out = false;
			return true;
		}
		else if (val.SubString(0, 4) == "true")
		{
			out = true;
			return true;
		}
		out = inject;
		return false;
	}

	template<typename TNum>
	bool TryParse(const String& val, TNum& out, TNum inject)
	{
		bool result;
		if (std::is_floating_point<TNum>::value)
		{
			double temp = 0;
			result = TryParse(val, temp, static_cast<double>(inject));
			out = static_cast<TNum>(temp);
		}
		else if (std::is_unsigned<TNum>::value)
		{
			uint64_t temp = 0;
			result = TryParse(val, temp, static_cast<uint64_t>(inject));
			out = static_cast<TNum>(temp);
		}
		else
		{
			int64_t temp = 0;
			result = TryParse(val, temp, static_cast<int64_t>(inject));
			out = static_cast<TNum>(temp);
		}
		return result;
	}


}