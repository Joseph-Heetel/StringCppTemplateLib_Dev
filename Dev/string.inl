#pragma once
#include "string.hpp"
#include <cassert>
#include "stringiterator.hpp"

namespace jht {

#pragma region Managed Data

	inline String::ManagedData& String::AccessManaged()
	{
		return *(reinterpret_cast<ManagedData*>(m_Data));
	}
	inline const String::ManagedData& String::AccessManaged() const
	{
		return *(reinterpret_cast<const ManagedData*>(m_Data));
	}
	inline void String::InitManaged(size_t len)
	{
		size_t size = sizeof(ManagedData) + static_cast<size_t>(len);
		void* rawData = operator new[](size, std::align_val_t{ alignof(ManagedData) });
		ManagedData* data = new (rawData) ManagedData();
		(&(data->Data))[len] = '\0';
		m_Data = data;
		m_Code = BuildCodeManaged(len);
		Register();
	}

	inline void String::Register()
	{
		AccessManaged().RefCounter++;
	}

	inline void String::Unregister()
	{
		ManagedData& data = AccessManaged();
		data.RefCounter--;
		if (data.RefCounter == 0)
		{
			Clean();
		}
	}

	inline void String::Clean()
	{
		void* data = reinterpret_cast<uint8_t*>(m_Data);
		operator delete[](data, std::align_val_t{ alignof(ManagedData) });
	}

	inline const char* String::AccessStringView() const
	{
		return reinterpret_cast<const char*>(m_Data);
	}

	inline void String::AssertMutable()
	{
#ifndef JHT_STRING_NOASSERT
		assert(IsManaged() && "String objects in view mode are immutable!");
#endif
	}

#pragma endregion
#pragma region Initializing, Constructing, Destructing

	inline size_t String::BuildCodeView(size_t length)
	{
#ifndef JHT_STRING_NOASSERT
		assert(length < MASK_LENGTH && "Maximum size exceeded!");
#endif
		return length;
	}

	inline size_t String::BuildCodeManaged(size_t length)
	{
#ifndef JHT_STRING_NOASSERT
		assert(length < MASK_LENGTH && "Maximum size exceeded!");
#endif
		return FLAG_ISMANAGED | length;
	}

	inline String::~String()
	{
		if (IsManaged())
		{
			Unregister();
		}
	}

	inline String::String(const String& other)
	{
		m_Data = other.m_Data;
		m_Code = other.m_Code;
		if (IsManaged())
		{
			Register();
		}
	}

	inline String::String(const String&& other) noexcept
	{
		m_Data = other.m_Data;
		m_Code = other.m_Code;
		if (IsManaged())
		{
			Register();
		}
	}

	inline String& String::operator=(const String& other)
	{
		if (IsManaged())
		{
			Unregister();
		}
		m_Data = other.m_Data;
		m_Code = other.m_Code;
		if (IsManaged())
		{
			Register();
		}
		return *this;
	}

#pragma endregion
#pragma region Data access and manipulation

	inline size_t String::Length() const
	{
		return m_Code & MASK_LENGTH;
	}
	inline bool jht::String::IsEmpty() const
	{
		return !Length();
	}
	inline bool jht::String::IsNotEmpty() const
	{
		return Length();
	}
	inline bool String::IsManaged() const
	{
		return (m_Code & FLAG_ISMANAGED);
	}
	inline char* String::Data()
	{
		AssertMutable();

		return &(AccessManaged().Data);
	}
	inline const char* String::Data() const
	{
		if (IsManaged())
		{
			return &(AccessManaged().Data);
		}
		else
		{
			return AccessStringView();
		}
	}	
	inline const char* String::ConstData() const
	{
		if (IsManaged())
		{
			return &(AccessManaged().Data);
		}
		else
		{
			return AccessStringView();
		}
	}

	inline char& String::operator[](const index_t index)
	{
#ifndef JHT_STRING_NOASSERT
		assert(index >= 0 && index < static_cast<index_t>(Length()) && "Index invalid!");
#endif
		return Data()[index];
	}
	inline const char& String::operator[](const index_t index) const
	{
#ifndef JHT_STRING_NOASSERT
		assert(index >= 0 && index < static_cast<index_t>(Length()) && "Index invalid!");
#endif
		return Data()[index];
	}
	inline void String::Fill(const char* str, size_t strlength, size_t offset)
	{
		AssertMutable();
		char* data = Data() + offset;
		index_t availableLength = static_cast<index_t>(Length()) - static_cast<index_t>(offset);
		if (availableLength <= 0)
		{
			return;
		}
		strlength = min(strlength, static_cast<size_t>(availableLength));
		memcpy(data, str, strlength);
	}
	inline void String::Fill(const String& str, size_t offset)
	{
		Fill(str.Data(), str.Length(), offset);
	}
	inline void String::Fill(char value)
	{
		AssertMutable();
		uint32_t byteValue = static_cast<uint32_t>(value);
		memset(Data(), (byteValue << 24) | (byteValue << 16) | (byteValue << 8) | byteValue, Length());
	}

	inline String String::SubString(size_t offset, size_t length) const
	{
		if (offset >= Length())
		{
			return String();
		}
		if (offset + length >= Length())
		{
			return MakeView(Data() + offset, Length() - offset);
		}
		return MakeView(Data() + offset, length);
	}

	inline void String::Split(char splitchar, std::vector<String>& out, bool skipEmpty) const
	{
		if (IsEmpty())
		{
			return;
		}
		const char* start = Data();
		JHT_ITERATE(String, *this, iter)
		{
			if (*iter == splitchar)
			{
				const char* current = iter.Ptr();
				if (current - start >= 0)
				{
					String section = MakeView(start, current - start);
					if (!skipEmpty || section.IsNotEmpty())
					{
						out.push_back(section);
					}
				}
				start = current + 1;
			}
		}
		const char* current = Data() + Length();
		if (current - start >= 0)
		{
			String section = MakeView(start, current - start);
			if (!skipEmpty || section.IsNotEmpty())
			{
				out.push_back(section);
			}
		}
	}

	inline String jht::String::Trimmed() const
	{
		const char* start = nullptr;
		const char* end = nullptr;
		JHT_ITERATE(String, *this, iter)
		{
			bool whitespace = IsWhitespace(*iter);
			if (!start)
			{
				if (!whitespace)
				{
					start = iter.Ptr();
				}
			}
			else if (!end)
			{
				if (whitespace)
				{
					end = iter.Ptr();
					break;
				}
			}
		}
		if (!start)
		{
			return String();
		}
		if (!end)
		{
			end = Data() + Length();
		}
		return String::MakeView(start, end - start);
	}

	inline String::operator bool() const
	{
		return Length();
	}

	inline bool String::operator==(const String& right)
	{
		return Compare(*this, right) == 0;
	}

	inline bool String::operator!=(const String& right)
	{
		return Compare(*this, right) != 0;
	}

	inline bool jht::String::IsWhitespace(const char c)
	{
		static const char* WHITESPACECHAR = "\u0009\u000A\u000B\u000C\u000D\u0020";
		return c == WHITESPACECHAR[0] ||
			c == WHITESPACECHAR[1] ||
			c == WHITESPACECHAR[2] ||
			c == WHITESPACECHAR[3] ||
			c == WHITESPACECHAR[4] ||
			c == WHITESPACECHAR[5];
	}
	inline int32_t String::Compare(const String& left, const String& right)
	{
		const char* leftComp = left.Data();
		const char* rightComp = right.Data();
		index_t leftLength = static_cast<index_t>(left.Length());
		index_t rightLength = static_cast<index_t>(right.Length());
		if (leftComp == rightComp)
		{
			return static_cast<int32_t>(clamp(leftLength - rightLength, -1, 1));
		}
		index_t i = 0;
		for (; i < leftLength && i < rightLength; i++)
		{
			if (leftComp[i] == rightComp[i])
			{
				continue;
			}
			return clamp(static_cast<int32_t>(leftComp[i]) - static_cast<int32_t>(rightComp[i]), -1, 1);
		}
		return static_cast<int32_t>(clamp(leftLength - rightLength, -1, 1));
	}

	inline String String::AsView() const
	{
		return MakeView(Data(), Length());
	}

	inline String String::AsManaged() const
	{
		if (IsManaged())
		{
			return *this;
		}
		else
		{
			return MakeManaged(Data(), Length());
		}
	}

	inline String String::MakeCopy() const
	{
		return MakeManaged(Data(), Length());
	}

	//template<typename TChar>
	//bool StringCharProducer<TChar>::GetNext(TChar& item)
	//{
	//	if (!m_Iterator)
	//	{
	//		return false;
	//	}
	//	item = static_cast<TChar>(m_Iterator.Current());
	//	++m_Iterator;
	//	return true;
	//}

#pragma endregion
#pragma region Maker methods

	inline String String::MakeManaged(size_t length)
	{
		String result;
		if (length)
		{
			result.InitManaged(length);
		}
		return result;
	}

	inline String String::MakeManaged(const char* source)
	{
		size_t len = strlen(source);
		return MakeManaged(source, len);
	}
	inline String String::MakeManaged(const char* source, const size_t len)
	{
		String result;
		if (len)
		{
			result.InitManaged(len);
			result.Fill(source, len);
		}
		return result;
	}
	inline String String::MakeManaged(const char value, const size_t repeat)
	{
		String result;
		if (repeat)
		{
			result.InitManaged(repeat);
			result.Fill(value);
		}
		return result;
	}
	inline String String::MakeManaged(const std::string_view& strview)
	{
		return MakeManaged(strview.data(), strview.length());
	}
	inline String String::MakeView(const char* source)
	{
		return String(source);
	}
	inline String String::MakeView(const char* source, const size_t len)
	{
		return String(source, len);
	}
	inline String String::MakeView(const std::string_view& strview)
	{
		return String(strview.data(), strview.length());
	}

#pragma endregion
}