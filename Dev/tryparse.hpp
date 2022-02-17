#pragma once
#include "string.hpp"

namespace jht {
	/// @brief Attempts parsing val to a fundamental type, using decimal number representation
	/// @param out output value
	/// @param inject value to return in out if parsing fails
	/// @return true if parsing succeeds
	inline bool TryParse(const String& val, int64_t& out, int64_t inject = 0);
	/// @brief Attempts parsing val to a fundamental type, using decimal number representation
	/// @param out output value
	/// @param inject value to return in out if parsing fails
	/// @return true if parsing succeeds
	inline bool TryParse(const String& val, uint64_t& out, uint64_t inject = 0);
	/// @brief Attempts parsing val to a fundamental type, using decimal number representation
	/// @param out output value
	/// @param inject value to return in out if parsing fails
	/// @return true if parsing succeeds
	inline bool TryParse(const String& val, double& out, double inject = 0);
	/// @brief Attempts parsing val to a fundamental type, using decimal number representation
	/// @param out output value
	/// @param inject value to return in out if parsing fails
	/// @return true if parsing succeeds
	inline bool TryParse(const String& val, bool& out, bool inject = false);

	/// @brief Attempts parsing val to a fundamental type, using decimal number representation
	/// @param out output value
	/// @param inject value to return in out if parsing fails
	/// @return true if parsing succeeds
	template<typename TNum>
	bool TryParse(const String& val, TNum& out, TNum inject = 0);

}