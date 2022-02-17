#pragma once
#include "string.hpp"

namespace jht {

	/// @brief Class exposing functionaly to iterate through the characters of a String object
	class StringIterator
	{
	private:
		String m_Str;
		index_t m_Index;

	public:
		StringIterator() : m_Str(), m_Index() {}
		StringIterator(const String& str) : m_Str(str), m_Index() {}

		/// @brief Exposes the current character
		char operator*() const { return m_Str[m_Index]; }
		/// @brief Exposes the current character
		char Current() const { return m_Str[m_Index]; }

		/// @brief Advances the iterator to the next character
		void operator++() { m_Index++; }
		/// @brief Returns true if the index represented by this iterator is valid
		operator bool() const { return static_cast<size_t>(m_Index) < m_Str.Length(); }

		/// @brief Exposes a pointer to the currently read character
		const char* Ptr() const { return m_Str.Data() + m_Index; }

		/// @brief Exposes the current index represented by the iterator
		index_t& Index() { return m_Index; }
		/// @brief Exposes the current index represented by the iterator
		index_t Index() const { return m_Index; }
	};
}