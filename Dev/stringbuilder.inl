#pragma once
#include "stringbuilder.hpp"

namespace jht {
	template<>
	inline void StringBuilder::Append(const char* cstr)
	{
		Append(String::MakeView(cstr));
	}
	template<>
	inline void StringBuilder::Append(String str)
	{
		Append<const String&>(str);
	}
	template<>
	inline void jht::StringBuilder::Append(const String& str)
	{
		bool pushOnTempBuff = str.Length() < SINGLETHRESHHOLD;
		bool flushTempBuff = true;
		if (pushOnTempBuff) {
			flushTempBuff = str.Length() + m_BufferIndex > BUFFERSIZE;
		}

		if (flushTempBuff) {
			FlushBuffer();
		}

		m_Length += str.Length();

		if (pushOnTempBuff) {
			m_Buffer.Fill(str, m_BufferIndex);
			m_BufferIndex += str.Length();
		}
		else {
			m_Sections.push_back(str);
		}
	}

	template<>
	inline void StringBuilder::Append(char c)
	{
		if (m_BufferIndex >= BUFFERSIZE) {
			FlushBuffer();
		}
		m_Buffer[m_BufferIndex] = c;
		m_BufferIndex++;
		m_Length++;
	}

	template<typename T>
	inline void StringBuilder::Append(T value)
	{
		Append(ToString(value));
	}

	template<typename T>
	inline void StringBuilder::AppendLine(T value)
	{
		Append(value);
		Append("\n");
	}
	template<typename T>
	inline StringBuilder& StringBuilder::operator<<(T value)
	{
		Append(value);
		return *this;
	}

	inline void StringBuilder::FlushBuffer()
	{
		if (m_BufferIndex > 0) {
			m_Sections.push_back(String::MakeManaged(m_Buffer.Data(), m_BufferIndex));
			m_BufferIndex = 0;
		}
	}

	inline String StringBuilder::Build()
	{
		if (m_Length == 0) {
			return String();
		}
		FlushBuffer();
		String result = String::MakeManaged(Length());
		char* data = result.Data();
		for (int32_t index = 0; index < m_Sections.size(); index++)
		{
			memcpy(data, m_Sections[index].Data(), m_Sections[index].Length());
			data += m_Sections[index].Length();
		}
		return result;
	}
}