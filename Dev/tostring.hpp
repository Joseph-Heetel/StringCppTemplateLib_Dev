#pragma once
#include "string.hpp"

namespace jht {
	/// @brief Stringifies value
	/// @param arg for integers this is the radius, for floats this is the precision
	template<typename TNum>
	String ToString(TNum value, int32_t arg = 10);

	/// @brief Stringifies value
	/// @param radius Which numeric system to use: 2 = binary, 8 = octal, 10 = decimal, 16 = hexadecimal
	template<>
	String ToString<int64_t>(int64_t value, int32_t radius);
	/// @brief Stringifies value
	/// @param radius Which numeric system to use: 2 = binary, 8 = octal, 10 = decimal, 16 = hexadecimal
	template<>
	String ToString<uint64_t>(uint64_t value, int32_t radius);
	/// @brief Stringifies a pointer (interpreting it as a hexadecimal unsigned integer)
	inline String ToString(const void* ptr);
	/// @brief Stringifies value
	/// @param precision How many letters of the fractional section to include. Further precision is discarded, and the resulting string will may end early.
	template<>
	String ToString<double>(double value, int32_t precision);
	/// @brief Stringifies value
	/// @param truestr set this to customize the string representing 'true'
	/// @param falsestr set this to customize the string representing 'false'
	inline String ToString(bool value, const char* truestr = "true", const char* falsestr = "false");

}
