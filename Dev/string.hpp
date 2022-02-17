#pragma once
#include <string.h>
#include "basics.hpp"
#include <vector>
#include <string_view>

namespace jht {

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
		/// @brief Exposes the raw character memory. Identical to "Data() const". Use incase compiler is confused
		const char* ConstData() const;

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
		String SubString(size_t offset, size_t length = UINT32_MAX) const;
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
}