#pragma once
#include <ostream>
#include <istream>
#include "string.hpp"

namespace jht {
	inline std::ostream& operator<<(std::ostream& left, const String& right);

	/// @brief Returns a string instance containing a single line ('\\n' delimiter) read from the stream
	inline String GetLine(std::istream& in);

}