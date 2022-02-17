#pragma once
#include <vector>
#include "string.hpp"

namespace jht {
	/// @brief Provides functionality to conveniently and efficiently chain strings together
	class StringBuilder
	{
	private:
		static const size_t BUFFERSIZE = 128;
		static const size_t SINGLETHRESHHOLD = BUFFERSIZE / 4;

		std::vector<String> m_Sections;
		size_t m_Length;
		String m_Buffer;
		size_t m_BufferIndex;

		void FlushBuffer();
	public:
		StringBuilder() : m_Sections(), m_Length(0), m_Buffer(String::MakeManaged('\0', BUFFERSIZE)), m_BufferIndex() {}

		/// @brief Combined length of all string sections currently stored
		size_t Length() const { return m_Length; }

		/// @brief Append any value
		template<typename T>
		void Append(T value);
		/// @brief Append any value. Followed by a newline
		template<typename T>
		void AppendLine(T value);
		/// @brief Append any value
		template<typename T>
		StringBuilder& operator<<(T value);

		template<>
		void Append<const char*>(const char* cstr);
		template<>
		void Append<>(String str);
		template<>
		void Append<const String&>(const String& str);
		template<>
		void Append<char>(char c);

		/// @brief Construct a managed string containing all inputs chained
		String Build();
	};
}