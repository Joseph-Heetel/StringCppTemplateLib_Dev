#pragma once
#include "converter.hpp"
#include <string_view>

namespace jht
{
	namespace utf8
	{
		const uint8_t MASK_MULTIWORD = 0b10000000;
		const uint8_t MASK_MULTI2 = 0b11100000;
		const uint8_t MASK_MULTI3 = 0b11110000;
		const uint8_t MASK_MULTI4 = 0b11111000;

		const uint8_t FLAG_MULTI2 = 0b11000000;
		const uint8_t FLAG_MULTI3 = 0b11100000;
		const uint8_t FLAG_MULTI4 = 0b11110000;

		const uint8_t DATA_MULTI2 = 0b00011111;
		const uint8_t DATA_MULTI3 = 0b00001111;
		const uint8_t DATA_MULTI4 = 0b00000111;

		const uint8_t MASK_CONT = 0b11000000;
		const uint8_t FLAG_CONT = 0b10000000;
		const uint8_t DATA_CONT = 0b00111111;


		template<typename TChar, int32_t LENGTH>
		inline void EncodeContinuations(char32_t& code, TChar* output)
		{
			for (int32_t i = 0; i < LENGTH; i++)
			{
				output[LENGTH - i] = FLAG_CONT | (DATA_CONT & code);
				code = code >> 6;
			}
		}

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

		template<typename TChar, int32_t LENGTH>
		inline bool ReadMultiWord(std::basic_string_view<TChar>& input, char32_t& output)
		{
			for (int32_t cont = 1; cont < LENGTH; cont++)
			{
				TChar contCode = 0;
				if (!TryGetNextCodeUnit<TChar>(input, contCode))
				{
					return false;
				}
				if ((contCode & MASK_CONT) != FLAG_CONT)
				{
					return false;
				}
				output = output << 6;
				output |= (contCode & DATA_CONT);
			}
			return true;
		}
	}

#pragma region Encoding

	template<typename TChar>
	inline std::basic_string_view<TChar> UTF8Encoder<TChar>::Encode(char32_t input)
	{
		using namespace utf8;

		memset(m_Buffer, 0, sizeof(TChar) * BUFFERSIZE);

		if (input < (0b1 << 7))
		{
			m_Buffer[0] = static_cast<TChar>(input);
			return std::basic_string_view<TChar>(m_Buffer, 1);
		}
		else if (input < (0b1 << 11))
		{
			EncodeContinuations<TChar, 1>(input, m_Buffer);
			m_Buffer[0] = FLAG_MULTI2 | static_cast<TChar>(input);
			return std::basic_string_view<TChar>(m_Buffer, 2);
		}
		else if (input < (0b1 << 16))
		{
			EncodeContinuations<TChar, 2>(input, m_Buffer);
			m_Buffer[0] = FLAG_MULTI3 | static_cast<TChar>(input);
			return std::basic_string_view<TChar>(m_Buffer, 3);
		}
		else if (input < (0b1 << 21))
		{
			EncodeContinuations<TChar, 3>(input, m_Buffer);
			m_Buffer[0] = FLAG_MULTI4 | static_cast<TChar>(input);
			return std::basic_string_view<TChar>(m_Buffer, 4);
		}
		return reinterpret_cast<const TChar*>(u8"\uFFFD");
	}

#pragma endregion
#pragma region Decoding

	template<typename TChar>
	inline char32_t jht::UTF8Decoder<TChar>::Decode(std::basic_string_view<TChar>& input)
	{
		using namespace utf8;

		TChar initcode = 0;
		if (!TryGetNextCodeUnit(input, initcode))
		{
			return 0;
		}

		if (initcode & MASK_MULTIWORD)
		{
			// Multiword code
			bool multi2 = ((initcode & MASK_MULTI2) == FLAG_MULTI2);
			bool multi3 = ((initcode & MASK_MULTI3) == FLAG_MULTI3);
			bool multi4 = ((initcode & MASK_MULTI4) == FLAG_MULTI4);

			char32_t output = 0;
			bool result = false;
			if (multi2)
			{
				output = initcode & DATA_MULTI2;
				result = ReadMultiWord<TChar, 2>(input, output);
			}
			else if (multi3)
			{
				output = initcode & DATA_MULTI3;
				result = ReadMultiWord<TChar, 3>(input, output);
			}
			else if (multi4)
			{
				output = initcode & DATA_MULTI4;
				result = ReadMultiWord<TChar, 4>(input, output);
			}
			if (!result)
			{
				return U'\uFFFD';
			}
			return output;
		}
		else
		{
			return static_cast<char32_t>(initcode);
		}
	}

#pragma endregion
}