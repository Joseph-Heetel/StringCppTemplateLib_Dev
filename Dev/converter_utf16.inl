#pragma once
#include "converter.hpp"
#include <string_view>

namespace jht
{
	namespace utf16
	{
		const uint16_t MASK_SURROGATE = 0xFC00;
		const uint16_t TEST_SURROGATE = 0xD800;
		const uint16_t FLAG_HIGHSURROGATE = 0xD800;
		const uint16_t FLAG_LOWSURROGATE = 0xDC00; 
		const uint32_t MULTIWORDRANGE = 0x10000;
	
		template<typename TChar>
		inline bool TryGetNextCodeUnit(std::basic_string_view<TChar>& input, TChar& output)
		{
			if (input.length() == 0)
			{
				return false;
			}
			output = input[0];
			input = std::basic_string_view<TChar>(input.data() + 1, input.length() - 1);
			return true;
		}

		template<typename TChar, bool flipBytes>
		inline TChar ReorderBytes(TChar in)
		{
			if (flipBytes)
			{
				return (in >> 8) | ((in & 0xFF) << 8);
			}
			return in;
		}
	}

#pragma region Encoding

	template<typename TChar, bool flipBytes>
	inline std::basic_string_view<TChar> jht::UTF16Encoder<TChar, flipBytes>::Encode(char32_t code)
	{
		memset(m_Buffer, 0, sizeof(TChar) * BUFFERSIZE);
		using namespace utf16;

		bool isMultiword = code >= MULTIWORDRANGE;

		if (isMultiword)
		{
			TChar high = static_cast<TChar>(0xD800 + ((code - 0x10000) / 0x400));
			TChar low = static_cast<TChar>(0xDC00 + (code % 0x400));
			m_Buffer[0] = ReorderBytes<TChar, flipBytes>(high);
			m_Buffer[1] = ReorderBytes<TChar, flipBytes>(low);
			return std::basic_string_view<TChar>(m_Buffer, 2);
		}
		else
		{
			m_Buffer[0] = ReorderBytes<TChar, flipBytes>(static_cast<TChar>(code));
			return std::basic_string_view<TChar>(m_Buffer, 1);
		}
	}

#pragma endregion
#pragma region Decoding

	template<typename TChar, bool flipBytes>
	inline char32_t jht::UTF16Decoder<TChar, flipBytes>::Decode(std::basic_string_view<TChar>& input)
	{
		using namespace utf16;

		TChar code = 0;
		if (!TryGetNextCodeUnit<TChar>(input, code))
		{
			return 0;
		}

		code = ReorderBytes<TChar, flipBytes>(code);

		bool isSurrogate = ((code & MASK_SURROGATE) == TEST_SURROGATE);

		if (!isSurrogate)
		{
			return code;
		}
		else
		{
			TChar lowSurrogateCode = 0;
			if (!TryGetNextCodeUnit(input, lowSurrogateCode))
			{
				return true;
			}

			lowSurrogateCode = ReorderBytes<TChar, flipBytes>(lowSurrogateCode);

			bool isSurrogate2 = !((lowSurrogateCode & MASK_SURROGATE) == TEST_SURROGATE);

			if (!isSurrogate2)
			{
				return code;
			}

			return (code - 0xD800) * 0x400 + (lowSurrogateCode - 0xDC00) + 0x10000;
		}
	}

#pragma endregion
}