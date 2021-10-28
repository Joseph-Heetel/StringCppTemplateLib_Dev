#pragma once
#include <cstdint>
#include <new>
#include <cstdlib>
#include <corecrt_memory.h>
#include <string.h>
#include <cassert>
#include <string_view>

namespace spp
{
	/// @brief A String class combining functionality for efficiently refcounted string instances and readonly stringviews in one class
	class String
	{
	private:
		static const uint64_t FLAG_ISMANAGED = 0b1Ui64 << 63Ui64;
		static const uint64_t MASK_LENGTH = ~FLAG_ISMANAGED;

		uint64_t m_Code;
		void* m_Data;

		template<bool IsManaged>
		static uint64_t BuildCode(const uint64_t length);

		struct ManagedData
		{
			uint64_t RefCounter;
			char Data;

			ManagedData() : RefCounter(0), Data(0) {}
		};
		ManagedData& AccessManaged();
		const ManagedData& AccessManaged() const;
		void InitManaged(const uint64_t len, const char* source);
		void Register();
		void Unregister();
		void Clean();

		const char* AccessStringView() const;
	public:
		/// @brief Initializes as a stringview for zero length string
		String() : m_Code(0), m_Data("") {}
		/// @brief Initializes as a stringview
		/// @param data readonly persistent data the stringview refers to
		String(const char* data) : m_Code(BuildCode<false>(strlen(data))), m_Data(const_cast<char*>(data)) {}
		/// @brief Initializes as a stringview
		/// @param data readonly persistent data the stringview refers to
		/// @param len length of the string view
		String(const char* data, const uint64_t len) : m_Code(BuildCode<false>(len)), m_Data(const_cast<char*>(data)) {}

		~String();
		String(const String& other);
		String(const String&& other);
		String& operator=(const String& other);

		uint64_t Length() const;
		bool IsManaged() const;

		char* Data();
		const char* Data() const;

		char& operator[](const uint64_t index);
		const char& operator[](const uint64_t index) const;

		static String MakeManaged(const char* source);
		static String MakeManaged(const char* source, const uint64_t len);
		static String MakeManaged(const char value, const uint64_t repeat);
		static String MakeManaged(const std::string_view& strview);
		static String MakeView(const char* source);
		static String MakeView(const char* source, const uint64_t len);
		static String MakeView(const std::string_view& strview);
	};

	std::ostream& operator<<(std::ostream& left, const String& right);

#pragma region Managed Data

	inline String::ManagedData& String::AccessManaged()
	{
		return *(reinterpret_cast<ManagedData*>(m_Data));
	}
	inline const String::ManagedData& String::AccessManaged() const
	{
		return *(reinterpret_cast<const ManagedData*>(m_Data));
	}
	inline void String::InitManaged(uint64_t len, const char* source)
	{
		size_t size = sizeof(ManagedData) + static_cast<size_t>(len);
		void* rawData = operator new[](size, std::align_val_t{ alignof(ManagedData) });
		ManagedData* data = new (rawData) ManagedData();
		if (source)
		{
			memcpy(&(data->Data), source, len);
		}
		else
		{
			memset(&(data->Data), 0, len);
		}
		(&(data->Data))[len] = '\0';
		m_Code = BuildCode<true>(len);
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
		uint8_t* data = reinterpret_cast<uint8_t*>(m_Data);
		operator delete[](data, std::align_val_t{ alignof(ManagedData) });
	}

	inline const char* String::AccessStringView() const
	{
		return reinterpret_cast<const char*>(m_Data);
	}

#pragma endregion

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

	inline String::String(const String && other)
	{
		m_Data = other.m_Data;
		m_Code = other.m_Code;
		if (IsManaged())
		{
			Register();
		}
	}

	inline String& String::operator=(const String & other)
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
	}

	inline uint64_t String::Length() const
	{
		return m_Code & MASK_LENGTH;
	}
	inline bool String::IsManaged() const
	{
		return !(m_Code & FLAG_ISMANAGED);
	}
	inline char* String::Data()
	{
		if (IsManaged())
		{
			return &(AccessManaged().Data);
		}
		else
		{
			return nullptr;
		}
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
	inline char& String::operator[](const uint64_t index)
	{
#ifndef SPP_DISABLE_ASSERTS
		assert(index > 0 && index < Length() && "Index invalid!");
#endif
		return Data()[index];
	}
	inline const char& String::operator[](const uint64_t index) const
	{
#ifndef SPP_DISABLE_ASSERTS
		assert(index > 0 && index < Length() && "Index invalid!");
#endif
		return Data()[index];
	}
	inline String String::MakeManaged(const char* source)
	{
		uint64_t len = strlen(source);
		return MakeManaged(source, len);
	}
	inline String String::MakeManaged(const char* source, const uint64_t len)
	{
		String result;
		if (len)
		{
			result.InitManaged(len, source);
		}
		return result;
	}
	inline String String::MakeManaged(const char value, const uint64_t repeat)
	{
		String result;
		if (repeat)
		{
			result.InitManaged(repeat, nullptr);
			memset(result.Data(), (value << 24) | (value << 16) | (value << 8) | value, repeat);
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
	inline String String::MakeView(const char* source, const uint64_t len)
	{
		return String(source, len);
	}
	inline String String::MakeView(const std::string_view& strview)
	{
		return String(strview.data(), strview.length());
	}
	template<bool IsManaged>
	inline uint64_t String::BuildCode(const uint64_t length)
	{
		if (IsManaged)
		{
			return length | FLAG_ISMANAGED;
		}
		else
		{
			return length;
		}
	}

	std::ostream& spp::operator<<(std::ostream& left, const String& right)
	{
		left << right.Data();
		return left;
	}
}