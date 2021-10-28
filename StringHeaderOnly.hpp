#pragma once

/*

StringHeaderOnly.hpp Version 0.1.1

A single file library exposing a String class combining stringview und refcounted string object functionality into one.
Additionally providing a StringBuilder implementation and methods for writing and parsing built-in types.

Customization #defines:
	#define JOHEET_STRING_NOASSERT
		Disable asserts (not sure why you'd want to)

MIT License
Copyright 2021 Joseph Heetel (https://github.com/Joseph-Heetel)

	Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files
	(the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge,
	publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do
	so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
	FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
	CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include <cstdint>
#include <new>
#include <cstdlib>
#include <corecrt_memory.h>
#include <string.h>
#include <cassert>
#include <string_view>
#include <ostream>
#include <vector>
#include <type_traits>

namespace joheet
{
	using index_t = ptrdiff_t;

#ifndef JOHEET_ITERATE
#define JOHEET_ITERATE(type, obj, iter) for (type::Iterator iter(obj); iter; ++iter)
#endif

#ifndef min
#define min(a, b) ((a) < (b)) ? (a) : (b)
#endif

#ifndef max
#define max(a, b) ((a) > (b)) ? (a) : (b)
#endif

#ifndef abs
#define abs(a) ((a) < 0) ? (-(a)) : (a)
#endif

	class StringIterator;

	/// @brief A String class combining functionality for efficiently refcounted string instances and readonly stringviews in one class
	class String
	{
	private:
		static const size_t FLAG_ISMANAGED = size_t(0b1) << (sizeof(size_t) * 8 - 2);
		static const size_t MASK_FLAGS = size_t(0b1) << (sizeof(size_t) * 8 - 2);
		static const size_t MASK_LENGTH = ~MASK_FLAGS;

		size_t m_Code;
		void* m_Data;

		static size_t BuildCodeView(size_t length);
		static size_t BuildCodeManaged(size_t length);

		struct ManagedData
		{
			size_t RefCounter;
			char Data;

			ManagedData() : RefCounter(0), Data(0) {}
		};
		ManagedData& AccessManaged();
		const ManagedData& AccessManaged() const;
		void InitManaged(const size_t len);
		void Register();
		void Unregister();
		void Clean();

		const char* AccessStringView() const;
		void AssertMutable();
	public:
		/// @brief the iterator type used to iterate through the collection represented by this class
		using Iterator = StringIterator;

		/// @brief Initializes as a stringview for zero length string
		String() : m_Code(0), m_Data(const_cast<char*>("")) {}
		/// @brief Initializes as a stringview
		/// @param data readonly persistent data the stringview refers to
		String(const char* data) : m_Code(BuildCodeView(strlen(data))), m_Data(const_cast<char*>(data)) {}
		/// @brief Initializes as a stringview
		/// @param data readonly persistent data the stringview refers to
		/// @param len length of the string view
		String(const char* data, const size_t len) : m_Code(BuildCodeView(len)), m_Data(const_cast<char*>(data)) {}

		~String();
		String(const String& other);
		String(const String&& other) noexcept;
		String& operator=(const String& other);

		/// @brief Get the length of the character sequence, excluding a potential terminating null character
		size_t Length() const;
		/// @brief True if Length() == 0
		bool IsEmpty() const;
		/// @brief True if Length() > 0
		bool IsNotEmpty() const;
		/// @brief True if the character data is managed via ref counting
		bool IsManaged() const;

		/// @brief Exposes the raw character memory. The mutable version of this function may only be called on managed String objects! Use String::AsManaged() if you need write access.
		char* Data();
		/// @brief Exposes the raw character memory. The mutable version of this function may only be called on managed String objects! Use String::AsManaged() if you need write access.
		const char* Data() const;

		/// @brief Indexes the underlying character sequence. The mutable version of this function may only be called on managed String objects! Use String::AsManaged() if you need write access.
		char& operator[](const index_t index);
		/// @brief Indexes the underlying character sequence. The mutable version of this function may only be called on managed String objects! Use String::AsManaged() if you need write access.
		const char& operator[](const index_t index) const;

		/// @brief Copies a character sequence into this string instance. This function may only be called on managed String objects!
		/// @param str character sequence
		/// @param strlength maximum count of characters to copy
		/// @param offset write offset
		void Fill(const char* str, size_t strlength, size_t offset = 0);
		/// @brief Copies a character sequence into this string instance. This function may only be called on managed String objects!
		/// @param str 
		/// @param offset write offset
		void Fill(const String& str, size_t offset = 0);
		/// @brief Fills this string instance with a character value repeated
		void Fill(char value);

		/// @brief Gets a StringView String
		/// @param offset Start position of the returned string view relative to this string
		/// @param length Maximum length of the returned string view
		String SubString(size_t offset, size_t length = SIZE_MAX) const;
		/// @brief Splits this string
		/// @param splitchar character marking where sections begin and end
		/// @param out vector to write output to
		/// @param skipEmpty if true, empty sections are ignored
		void Split(char splitchar, std::vector<String>& out, bool skipEmpty = true) const;
		/// @brief Gets a string view with all whitespace characters front and end removed
		String Trimmed() const;

		/// @brief Returns true if Length() > 0
		operator bool() const;

		/// @brief Returns true if both strings are lexigraphically equal
		bool operator==(const String& right);
		/// @brief Returns true if both strings are lexigraphically nonequal
		bool operator!=(const String& right);
		/// @brief Returns the lexigraphical comparison between both strings
		static int32_t Compare(const String& left, const String& right);

		/// @brief Returns true for any whitespace character passed in: Tabulation, Line Feed, Vertical Tab, Form Feed, Carriage Return, Space, NewLine
		static bool IsWhitespace(char character);

		/// @brief Exposes this string instance as a view
		String AsView() const;
		/// @brief Exposes this string instance as a managed. Will allocate a ManagedData instance if necessary
		String AsManaged() const;
		/// @brief Exposes a ManagedData copy of the current instance
		String MakeCopy() const;

		/// @brief Makes a managed string object with non-initialized string data (terminating null character is set)
		/// @param length Mutable characters to reserve
		static String MakeManaged(size_t length);
		/// @brief Makes a managed copy of a null-terminated character sequence
		static String MakeManaged(const char* source);
		/// @brief Makes a managed copy of a character sequence up to a given length
		static String MakeManaged(const char* source, const size_t len);
		/// @brief Makes a managed copy filled with a repeated value
		static String MakeManaged(const char value, const size_t repeat);
		/// @brief Makes a managed copy of a string_view
		static String MakeManaged(const std::string_view& strview);
		/// @brief Initializes a view of a null-terminated character sequence
		static String MakeView(const char* source);
		/// @brief Initializes a view of a character sequence up to a given length
		static String MakeView(const char* source, const size_t len);
		/// @brief Initializes a view of a string_view
		static String MakeView(const std::string_view& strview);

	};

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
	String ToString(const void* ptr);
	/// @brief Stringifies value
	/// @param precision How many letters of the fractional section to include. Further precision is discarded, and the resulting string will may end early.
	template<>
	String ToString<double>(double value, int32_t precision);
	/// @brief Stringifies value
	/// @param truestr set this to customize the string representing 'true'
	/// @param falsestr set this to customize the string representing 'false'
	String ToString(bool value, const char* truestr = "true", const char* falsestr = "false");

	/// @brief Attempts parsing val to a fundamental type, using decimal number representation
	/// @param out output value
	/// @param inject value to return in out if parsing fails
	/// @return true if parsing succeeds
	bool TryParse(const String& val, int64_t& out, int64_t inject = 0);
	/// @brief Attempts parsing val to a fundamental type, using decimal number representation
	/// @param out output value
	/// @param inject value to return in out if parsing fails
	/// @return true if parsing succeeds
	bool TryParse(const String& val, uint64_t& out, uint64_t inject = 0);
	/// @brief Attempts parsing val to a fundamental type, using decimal number representation
	/// @param out output value
	/// @param inject value to return in out if parsing fails
	/// @return true if parsing succeeds
	bool TryParse(const String& val, double& out, double inject = 0);

	/// @brief Attempts parsing val to a fundamental type, using decimal number representation
	/// @param out output value
	/// @param inject value to return in out if parsing fails
	/// @return true if parsing succeeds
	template<typename TNum>
	bool TryParse(const String& val, TNum& out, TNum inject = 0);

	/// @brief Provides functionality to conveniently and efficiently chain strings together
	class StringBuilder
	{
	private:
		std::vector<String> m_Sections;
		size_t m_Length;

	public:
		StringBuilder() : m_Sections(), m_Length(0) {}

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
		void Append<const String&>(const String& str);
		template<>
		void Append<String>(String str);
		template<>
		void Append<char>(char c);

		/// @brief Construct a managed string containing all inputs chained
		String Build() const;
	};

	std::ostream& operator<<(std::ostream& left, const String& right);

	/// @brief Returns a string instance containing a single line ('\\n' delimiter) read from the stream
	String GetLine(std::istream& in);

#pragma region String Class
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
#ifndef JOHEET_STRING_NOASSERT
		assert(IsManaged() && "String objects in view mode are immutable!");
#endif
	}

#pragma endregion
#pragma region Initializing, Constructing, Destructing

	inline size_t String::BuildCodeView(size_t length)
	{
#ifndef JOHEET_STRING_NOASSERT
		assert(length < MASK_LENGTH && "Maximum size exceeded!");
#endif
		return length;
	}

	inline size_t String::BuildCodeManaged(size_t length)
	{
#ifndef JOHEET_STRING_NOASSERT
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
	inline bool joheet::String::IsEmpty() const
	{
		return !Length();
	}
	inline bool joheet::String::IsNotEmpty() const
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
	inline char& String::operator[](const index_t index)
	{
#ifndef JOHEET_STRING_NOASSERT
		assert(index >= 0 && index < static_cast<index_t>(Length()) && "Index invalid!");
#endif
		return Data()[index];
	}
	inline const char& String::operator[](const index_t index) const
	{
#ifndef JOHEET_STRING_NOASSERT
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
		strlength = min(strlength, availableLength);
		memcpy(data, str, strlength);
	}
	inline void String::Fill(const String& str, size_t offset)
	{
		Fill(str.Data(), str.Length());
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
		JOHEET_ITERATE(String, *this, iter)
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

	inline String joheet::String::Trimmed() const
	{
		const char* start = nullptr;
		const char* end = nullptr;
		JOHEET_ITERATE(String, *this, iter)
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

	inline bool joheet::String::IsWhitespace(const char c)
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
		const char* leftComp = left.Length() ? left.Data() : "";
		const char* rightComp = right.Length() ? right.Data() : "";
		return strcmp(leftComp, rightComp);
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
#pragma endregion
#pragma region ToString Methods

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

#pragma endregion
#pragma region TryParse Methods

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
				char c = in.Data()[index];

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
			char c = in.Data()[index];
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
			char c = fractionalstr.Data()[index];
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

#pragma endregion
#pragma region StringBuilder Class

	template<>
	inline void StringBuilder::Append(const char* cstr)
	{
		Append(String::MakeView(cstr));
	}
	template<>
	inline void joheet::StringBuilder::Append(const String& str)
	{
		if (str.Length())
		{
			m_Sections.push_back(str);
			m_Length += str.Length();
		}
	}

	template<>
	inline void StringBuilder::Append(String str)
	{
		if (str.Length())
		{
			m_Sections.push_back(str);
			m_Length += str.Length();
		}
	}

	template<>
	inline void StringBuilder::Append(char c)
	{
		Append(String::MakeManaged(c, 1));
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

	inline String StringBuilder::Build() const
	{
		String result = String::MakeManaged(Length());
		char* data = result.Data();
		for (int32_t index = 0; index < m_Sections.size(); index++)
		{
			memcpy(data, m_Sections[index].Data(), m_Sections[index].Length());
			data += m_Sections[index].Length();
		}
		return result;
	}

#pragma endregion
#pragma region Stream Interoperability

	std::ostream& joheet::operator<<(std::ostream& left, const String& right)
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

		size_t strsize = 64;
		size_t writeindex = 0;
		String out = String::MakeManaged('\0', strsize);

		for (; in.good() && !in.eof();)
		{
			char c;
			in.read(&c, 1);
			if (c == '\n')
			{
				break;
			}
			if (writeindex == strsize)
			{
				strsize <<= 1;
				String newout = String::MakeManaged('\0', strsize);
				newout.Fill(out.Data(), out.Length());
				out = newout;
			}
			out[writeindex] = c;
			writeindex++;
		}
		return out;
	}

#pragma endregion
}