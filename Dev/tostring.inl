#pragma once
#include "tostring.hpp"
#include <cassert>
#include "stringbuilder.hpp"

namespace jht {
	template<typename TNum>
	String ToString(TNum value, int32_t arg)
	{
		if (std::is_integral<TNum>::value)
		{
			if (std::is_signed<TNum>::value)
			{
				return ToString(static_cast<int64_t>(value), arg);
			}
			else
			{
				return ToString(static_cast<uint64_t>(value), arg);
			}
		}
		if (std::is_arithmetic<TNum>::value)
		{
			return ToString(static_cast<double>(value), arg);
		}
		assert(0 && "No overload available for this type!");
		return String();
	}

	inline void PrintUint64(char* buffer, int32_t& index, uint64_t value, int32_t radius)
	{
		assert(radius > 1 && radius <= 16 && "radius may only be in range [2, 16]");

		const char* NUMLETTERS = "0123456789ABCDEF";

		if (value == 0)
		{
			buffer[index] = '0';
			index--;
		}
		while (value > 0)
		{
			int32_t letter = value % radius;
			value /= radius;
			char character = NUMLETTERS[letter];
			buffer[index] = character;
			index--;
		}
	}

	template<>
	inline String ToString(int64_t value, int32_t radius)
	{
		const int32_t BUFFERSIZE = 65;  // an unsigned 64 bit integer printed with radius 2 will produce 64 bit values, which is the maximum

		char    buffer[BUFFERSIZE] = {};
		int32_t index = BUFFERSIZE - 2;

		bool isneg = value < 0;
		value = abs(value);

		PrintUint64(buffer, index, static_cast<uint64_t>(value), radius);
		if (isneg)
		{
			buffer[index] = '-';
			index--;
		}
		return String::MakeManaged(buffer + index + 1);
	}

	template<>
	inline String ToString(uint64_t value, int32_t radius)
	{
		const int32_t BUFFERSIZE = 65;  // an unsigned 64 bit integer printed with radius 2 will produce 64 bit values, which is the maximum

		char    buffer[BUFFERSIZE] = {};
		int32_t index = BUFFERSIZE - 2;

		PrintUint64(buffer, index, value, radius);
		return String::MakeManaged(buffer + index + 1);
	}

	inline String ToString(const void* ptr)
	{
		static const int32_t BUFFERSIZE = 17;
		char                 buffer[BUFFERSIZE] = {};
		for (int32_t i = 0; i < BUFFERSIZE - 1; i++)
		{
			buffer[i] = '0';
		}
		uint64_t value = reinterpret_cast<uint64_t>(ptr);
		int32_t  index = BUFFERSIZE - 2;
		PrintUint64(buffer, index, value, 16);
		return String::MakeManaged(buffer, BUFFERSIZE - 1);
	}

	template<>
	inline String ToString(double value, int32_t precision)
	{
		const char* NUMLETTERS = "0123456789ABCDEF";
		bool isneg = value < 0;  // Keep track of the sign

		int64_t integral = static_cast<int64_t>(value);
		String integralstr = ToString(abs(integral), 10);

		static const int32_t BUFFERSIZE = 32;
		char                 buffer[BUFFERSIZE] = {};
		int32_t              index = 0;

		precision = min(BUFFERSIZE - 1, precision);

		double fract = abs(value - static_cast<double>(integral));  // isolate fractional value

		for (index = 0; index < precision && fract > 0; index++)
		{
			fract *= 10;
			int32_t letter = static_cast<int32_t>(fract);
			fract = fract - letter;
			buffer[index] = NUMLETTERS[letter];
		}
		String fractionalstr = String(buffer, index);

		StringBuilder builder;
		if (isneg)
		{
			builder << "-";
		}
		builder << integralstr;
		builder << ".";
		if (index == 0)
		{
			builder << "0";
		}
		else
		{
			builder << fractionalstr;
		}

		return builder.Build();
	}

	inline String ToString(bool value, const char* truestr, const char* falsestr)
	{
		return String(value ? truestr : falsestr);
	}

}