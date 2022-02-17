#pragma once
#include <string_view>
#include <sstream>

namespace jht
{

	template<typename TChar>
	class EncoderBase
	{
	public:
		using TIn = char32_t;
		using TOut = TChar;
		virtual std::basic_string_view<TChar> Encode(char32_t input) = 0;
		virtual std::basic_string<TChar> EncodeMulti(std::basic_string_view<char32_t> input)
		{
			std::basic_stringstream<TChar> output;
			for (char32_t c : input)
			{
				output << Encode(c);
			}
			return output.str();
		}
	};

	template<typename TChar>
	class DecoderBase
	{
	public:
		using TIn = TChar;
		using TOut = char32_t;
		virtual char32_t Decode(std::basic_string_view<TChar>& input) = 0;
		virtual std::basic_string<char32_t> DecodeMulti(std::basic_string_view<TChar> input)
		{
			std::basic_stringstream<char32_t> output;
			for (std::basic_string_view<TChar> strview(input); strview.size() > 0;)
			{
				output << Decode(strview);
			}
			return output.str();
		}
	};

#pragma region UTF8

	template<typename TChar>
	class UTF8Encoder : public EncoderBase<TChar>
	{
	protected:
		static const size_t BUFFERSIZE = 4;
		TChar m_Buffer[BUFFERSIZE];
	public:
		virtual std::basic_string_view<TChar> Encode(char32_t input) override;
	};


	template<typename TChar>
	class UTF8Decoder : public DecoderBase<TChar>
	{
	public:
		virtual char32_t Decode(std::basic_string_view<TChar>& input) override;
	};

	using UTF8EncoderChar = UTF8Encoder<char>;
	using UTF8EncoderChar8t = UTF8Encoder<char8_t>;

	using UTF8DecoderChar = UTF8Decoder<char>;
	using UTF8DecoderChar8t = UTF8Decoder<char8_t>;

#pragma endregion
#pragma region UTF16

	enum class Endianness
	{
		Little,
		Big
	};

	template<typename TChar, bool flipBytes>
	class UTF16Encoder : public EncoderBase<TChar>
	{
	protected:
		static const size_t BUFFERSIZE = 2;
		TChar m_Buffer[BUFFERSIZE];
	public:
		virtual std::basic_string_view<TChar> Encode(char32_t input) override;
	};

	template<typename TChar, bool flipBytes>
	class UTF16Decoder : public DecoderBase<TChar>
	{
	public:
		virtual char32_t Decode(std::basic_string_view<TChar>& input) override;
	};

	using UTF16EncoderWChar = UTF16Encoder<wchar_t, false>;
	using UTF16FlipEncoderWChar = UTF16Encoder<wchar_t, true>;
	using UTF16EncoderChar16t = UTF16Encoder<char16_t, false>;
	using UTF16FlipEncoderChar16t = UTF16Encoder<char16_t, true>;
	using UTF16DecoderWChar = UTF16Decoder<wchar_t, false>;
	using UTF16FlipDecoderWChar = UTF16Decoder<wchar_t, true>;
	using UTF16DecoderChar16t = UTF16Decoder<char16_t, false>;
	using UTF16FlipDecoderChar16t = UTF16Decoder<char16_t, true>;

#pragma endregion
}