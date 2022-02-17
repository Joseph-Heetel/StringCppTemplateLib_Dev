#pragma once
#include "jhtstring.hpp"
#include <cassert>
#include <iostream>

namespace jht
{
	inline void RunTests_Converter()
	{
		std::cout << "RunTests_Converter\n";

		{ // Single character UTF8
			UTF8EncoderChar encoder_char;
			UTF8EncoderChar8t encoder_char8_t;
			UTF8DecoderChar decoder_char;
			UTF8DecoderChar8t decoder_char8_t;

			for (char32_t c = 0; c < 128; c++)
			{
				std::basic_string<char> encoded0(encoder_char.Encode(c));
				std::basic_string_view<char> viewencoded0(encoded0);
				char32_t decoded0 = decoder_char.Decode(viewencoded0);
				assert(encoded0.size() == 1);
				assert(decoded0 == c);
				std::basic_string<char8_t> encoded1(encoder_char8_t.Encode(c));
				std::basic_string_view<char8_t> viewencoded1(encoded1);
				char32_t decoded1 = decoder_char8_t.Decode(viewencoded1);
				assert(encoded1.size() == 1);
				assert(decoded1 == c);
			}

			for (char32_t c = 128; c < 0x800; c++)
			{
				std::basic_string<char> encoded0(encoder_char.Encode(c));
				std::basic_string_view<char> viewencoded0(encoded0);
				char32_t decoded0 = decoder_char.Decode(viewencoded0);
				assert(encoded0.size() == 2);
				assert(decoded0 == c);
				std::basic_string<char8_t> encoded1(encoder_char8_t.Encode(c));
				std::basic_string_view<char8_t> viewencoded1(encoded1);
				char32_t decoded1 = decoder_char8_t.Decode(viewencoded1);
				assert(encoded1.size() == 2);
				assert(decoded1 == c);
			}

			for (char32_t c = 0x800; c < 0x10000; c++)
			{
				std::basic_string<char> encoded0(encoder_char.Encode(c));
				std::basic_string_view<char> viewencoded0(encoded0);
				char32_t decoded0 = decoder_char.Decode(viewencoded0);
				assert(encoded0.size() == 3);
				assert(decoded0 == c);
				std::basic_string<char8_t> encoded1(encoder_char8_t.Encode(c));
				std::basic_string_view<char8_t> viewencoded1(encoded1);
				char32_t decoded1 = decoder_char8_t.Decode(viewencoded1);
				assert(encoded1.size() == 3);
				assert(decoded1 == c);
			}

			for (char32_t c = 0x10000; c < 0x110000; c++)
			{
				std::basic_string<char> encoded0(encoder_char.Encode(c));
				std::basic_string_view<char> viewencoded0(encoded0);
				char32_t decoded0 = decoder_char.Decode(viewencoded0);
				assert(encoded0.size() == 4);
				assert(decoded0 == c);
				std::basic_string<char8_t> encoded1(encoder_char8_t.Encode(c));
				std::basic_string_view<char8_t> viewencoded1(encoded1);
				char32_t decoded1 = decoder_char8_t.Decode(viewencoded1);
				assert(encoded1.size() == 4);
				assert(decoded1 == c);
			}
		}

		{ // Single character UTF16
			UTF16EncoderWChar encoderWchar;
			UTF16EncoderChar16t encoderChar16;
			UTF16DecoderWChar decoderWchar;
			UTF16DecoderChar16t decoderChar16;

			for (char32_t c = 0x0000; c < 0xD800; c++)
			{
				std::basic_string<wchar_t> encoded0(encoderWchar.Encode(c));
				std::basic_string_view<wchar_t> viewencoded0(encoded0.data(), 1);
				char32_t decoded0 = decoderWchar.Decode(viewencoded0);
				assert(encoded0.size() == 1);
				assert(decoded0 == c);
				std::basic_string<char16_t> encoded1(encoderChar16.Encode(c));
				std::basic_string_view<char16_t> viewencoded1(encoded1.data(), 1);
				char32_t decoded1 = decoderChar16.Decode(viewencoded1);
				assert(encoded1.size() == 1);
				assert(decoded1 == c);
			}
			for (char32_t c = 0xE000; c < 0x10000; c++)
			{
				std::basic_string<wchar_t> encoded0(encoderWchar.Encode(c));
				std::basic_string_view<wchar_t> viewencoded0(encoded0.data(), 1);
				char32_t decoded0 = decoderWchar.Decode(viewencoded0);
				assert(encoded0.size() == 1);
				assert(decoded0 == c);
				std::basic_string<char16_t> encoded1(encoderChar16.Encode(c));
				std::basic_string_view<char16_t> viewencoded1(encoded1.data(), 1);
				char32_t decoded1 = decoderChar16.Decode(viewencoded1);
				assert(encoded1.size() == 1);
				assert(decoded1 == c);
			}
			for (char32_t c = 0x10000; c < 0x110000; c++)
			{
				std::basic_string<wchar_t> encoded0(encoderWchar.Encode(c));
				std::basic_string_view<wchar_t> viewencoded0(encoded0.data(), 2);
				char32_t decoded0 = decoderWchar.Decode(viewencoded0);
				assert(encoded0.size() == 2);
				assert(decoded0 == c);
				std::basic_string<char16_t> encoded1(encoderChar16.Encode(c));
				std::basic_string_view<char16_t> viewencoded1(encoded1.data(), 2);
				char32_t decoded1 = decoderChar16.Decode(viewencoded1);
				assert(encoded1.size() == 2);
				assert(decoded1 == c);
			}
		}

		{ // Single character UTF16 (Flipped)
			UTF16FlipEncoderWChar encoderWchar;
			UTF16FlipEncoderChar16t encoderChar16;
			UTF16FlipDecoderWChar decoderWchar;
			UTF16FlipDecoderChar16t decoderChar16;

			for (char32_t c = 0x0000; c < 0xD800; c++)
			{
				std::basic_string<wchar_t> encoded0(encoderWchar.Encode(c));
				std::basic_string_view<wchar_t> viewencoded0(encoded0);
				char32_t decoded0 = decoderWchar.Decode(viewencoded0);
				assert(encoded0.size() == 1);
				assert(decoded0 == c);
				std::basic_string<char16_t> encoded1(encoderChar16.Encode(c));
				std::basic_string_view<char16_t> viewencoded1(encoded1);
				char32_t decoded1 = decoderChar16.Decode(viewencoded1);
				assert(encoded1.size() == 1);
				assert(decoded1 == c);
			}
			for (char32_t c = 0xE000; c < 0x10000; c++)
			{
				std::basic_string<wchar_t> encoded0(encoderWchar.Encode(c));
				std::basic_string_view<wchar_t> viewencoded0(encoded0);
				char32_t decoded0 = decoderWchar.Decode(viewencoded0);
				assert(encoded0.size() == 1);
				assert(decoded0 == c);
				std::basic_string<char16_t> encoded1(encoderChar16.Encode(c));
				std::basic_string_view<char16_t> viewencoded1(encoded1);
				char32_t decoded1 = decoderChar16.Decode(viewencoded1);
				assert(encoded1.size() == 1);
				assert(decoded1 == c);
			}
			for (char32_t c = 0x10000; c < 0x110000; c++)
			{
				std::basic_string<wchar_t> encoded0(encoderWchar.Encode(c));
				std::basic_string_view<wchar_t> viewencoded0(encoded0);
				char32_t decoded0 = decoderWchar.Decode(viewencoded0);
				assert(encoded0.size() == 2);
				assert(decoded0 == c);
				std::basic_string<char16_t> encoded1(encoderChar16.Encode(c));
				std::basic_string_view<char16_t> viewencoded1(encoded1);
				char32_t decoded1 = decoderChar16.Decode(viewencoded1);
				assert(encoded1.size() == 2);
				assert(decoded1 == c);
			}
		}

		{ // Multi character UTF8
			UTF8EncoderChar encoder_char;
			UTF8EncoderChar8t encoder_char8_t;
			UTF8DecoderChar decoder_char;
			UTF8DecoderChar8t decoder_char8_t;

			const char8_t* cstr0 = u8"﻿काचं शक्नोम्यत्तुम् । नोपहिनस्ति माम् ॥";
			const char8_t* cstr1 = u8" J'peux manger d'la vitre, ça m'fa pas mal.";
			const char8_t* cstr2 = u8"Puedo comer vidrio, no me hace daño.";
			const char8_t* cstr3 = u8"ᛁᚳ᛫ᛗᚨᚷ᛫ᚷᛚᚨᛋ᛫ᛖᚩᛏᚪᚾ᛫ᚩᚾᛞ᛫ᚻᛁᛏ᛫ᚾᛖ᛫ᚻᛖᚪᚱᛗᛁᚪᚧ᛫ᛗᛖ᛬";
			const char8_t* cstr4 = u8"᚛᚛ᚉᚑᚅᚔᚉᚉᚔᚋ ᚔᚈᚔ ᚍᚂᚐᚅᚑ ᚅᚔᚋᚌᚓᚅᚐ᚜";
			const char8_t* cstr5 = u8"⠊⠀⠉⠁⠝⠀⠑⠁⠞⠀⠛⠇⠁⠎⠎⠀⠁⠝⠙⠀⠊⠞⠀⠙⠕⠑⠎⠝⠞⠀⠓⠥⠗⠞⠀⠍⠑";
			const char8_t* cstr6 = u8"আমি কাঁচ খেতে পারি, তাতে আমার কোনো ক্ষতি হয় না।";
			const char8_t* cstr7 = u8"ကျွန်တော် ကျွန်မ မှန်စားနိုင်တယ်။ ၎င်းကြောင့် ထိခိုက်မှုမရှိပါ။";
			const char8_t* cstr8 = u8" 我能吞下玻璃而不傷身體。";
			const char8_t* cstr9 = u8"ᐊᓕᒍᖅ ᓂᕆᔭᕌᖓᒃᑯ ᓱᕋᙱᑦᑐᓐᓇᖅᑐᖓ";
			const char8_t* cstrs[] = {cstr0, cstr1, cstr2, cstr3, cstr4, cstr5, cstr6, cstr7, cstr8, cstr9};

			for (index_t i = 0; i < 10; i++)
			{
				std::u8string_view strv0(cstrs[i]);
				auto decoded0 = decoder_char8_t.DecodeMulti(strv0);
				auto encoded0 = encoder_char8_t.EncodeMulti(decoded0);
				assert(strv0.size() == encoded0.size());
				for (index_t i = 0; i < encoded0.size(); i++)
				{
					assert(encoded0[i] == strv0[i]);
				}
				std::string_view strv1(reinterpret_cast<const char*>(cstrs[i]));
				auto decoded1 = decoder_char.DecodeMulti(strv1);
				auto encoded1 = encoder_char.EncodeMulti(decoded1);
				assert(strv1.size() == encoded1.size());
				for (index_t i = 0; i < encoded1.size(); i++)
				{
					assert(encoded1[i] == strv1[i]);
				}
			}
		}

		{ // Multi character UTF16
			UTF16EncoderWChar encoder_wchar;
			UTF16EncoderChar16t encoder_char16_t;
			UTF16DecoderWChar decoder_wchar;
			UTF16DecoderChar16t decoder_char16_t;

			const wchar_t* cstr0 = L"﻿काचं शक्नोम्यत्तुम् । नोपहिनस्ति माम् ॥";
			const wchar_t* cstr1 = L" J'peux manger d'la vitre, ça m'fa pas mal.";
			const wchar_t* cstr2 = L"Puedo comer vidrio, no me hace daño.";
			const wchar_t* cstr3 = L"ᛁᚳ᛫ᛗᚨᚷ᛫ᚷᛚᚨᛋ᛫ᛖᚩᛏᚪᚾ᛫ᚩᚾᛞ᛫ᚻᛁᛏ᛫ᚾᛖ᛫ᚻᛖᚪᚱᛗᛁᚪᚧ᛫ᛗᛖ᛬";
			const wchar_t* cstr4 = L"᚛᚛ᚉᚑᚅᚔᚉᚉᚔᚋ ᚔᚈᚔ ᚍᚂᚐᚅᚑ ᚅᚔᚋᚌᚓᚅᚐ᚜";
			const wchar_t* cstr5 = L"⠊⠀⠉⠁⠝⠀⠑⠁⠞⠀⠛⠇⠁⠎⠎⠀⠁⠝⠙⠀⠊⠞⠀⠙⠕⠑⠎⠝⠞⠀⠓⠥⠗⠞⠀⠍⠑";
			const wchar_t* cstr6 = L"আমি কাঁচ খেতে পারি, তাতে আমার কোনো ক্ষতি হয় না।";
			const wchar_t* cstr7 = L"ကျွန်တော် ကျွန်မ မှန်စားနိုင်တယ်။ ၎င်းကြောင့် ထိခိုက်မှုမရှိပါ။";
			const wchar_t* cstr8 = L" 我能吞下玻璃而不傷身體。";
			const wchar_t* cstr9 = L"ᐊᓕᒍᖅ ᓂᕆᔭᕌᖓᒃᑯ ᓱᕋᙱᑦᑐᓐᓇᖅᑐᖓ";
			const wchar_t* cstrs[] = { cstr0, cstr1, cstr2, cstr3, cstr4, cstr5, cstr6, cstr7, cstr8, cstr9 };

			for (index_t i = 0; i < 10; i++)
			{
				std::wstring_view strv0(cstrs[i]);
				auto decoded0 = decoder_wchar.DecodeMulti(strv0);
				auto encoded0 = encoder_wchar.EncodeMulti(decoded0);
				assert(strv0.size() == encoded0.size());
				for (index_t j = 0; j < encoded0.size(); j++)
				{
					assert(encoded0[j] == strv0[j]);
				}
				std::u16string_view strv1(reinterpret_cast<const char16_t*>(cstrs[i]));
				auto decoded1 = decoder_char16_t.DecodeMulti(strv1);
				auto encoded1 = encoder_char16_t.EncodeMulti(decoded1);
				assert(strv1.size() == encoded1.size());
				for (index_t j = 0; j < encoded1.size(); j++)
				{
					assert(encoded1[j] == strv1[j]);
				}
			}
		}

		//{ // Multi character UTF16 (Flipped)
		//	UTF16FlipEncoderWChar encoder_wchar;
		//	UTF16FlipEncoderChar16t encoder_char16_t;
		//	UTF16FlipDecoderWChar decoder_wchar;
		//	UTF16FlipDecoderChar16t decoder_char16_t;

		//	const wchar_t* cstr0 = L"﻿काचं शक्नोम्यत्तुम् । नोपहिनस्ति माम् ॥";
		//	const wchar_t* cstr1 = L" J'peux manger d'la vitre, ça m'fa pas mal.";
		//	const wchar_t* cstr2 = L"Puedo comer vidrio, no me hace daño.";
		//	const wchar_t* cstr3 = L"ᛁᚳ᛫ᛗᚨᚷ᛫ᚷᛚᚨᛋ᛫ᛖᚩᛏᚪᚾ᛫ᚩᚾᛞ᛫ᚻᛁᛏ᛫ᚾᛖ᛫ᚻᛖᚪᚱᛗᛁᚪᚧ᛫ᛗᛖ᛬";
		//	const wchar_t* cstr4 = L"᚛᚛ᚉᚑᚅᚔᚉᚉᚔᚋ ᚔᚈᚔ ᚍᚂᚐᚅᚑ ᚅᚔᚋᚌᚓᚅᚐ᚜";
		//	const wchar_t* cstr5 = L"⠊⠀⠉⠁⠝⠀⠑⠁⠞⠀⠛⠇⠁⠎⠎⠀⠁⠝⠙⠀⠊⠞⠀⠙⠕⠑⠎⠝⠞⠀⠓⠥⠗⠞⠀⠍⠑";
		//	const wchar_t* cstr6 = L"আমি কাঁচ খেতে পারি, তাতে আমার কোনো ক্ষতি হয় না।";
		//	const wchar_t* cstr7 = L"ကျွန်တော် ကျွန်မ မှန်စားနိုင်တယ်။ ၎င်းကြောင့် ထိခိုက်မှုမရှိပါ။";
		//	const wchar_t* cstr8 = L" 我能吞下玻璃而不傷身體。";
		//	const wchar_t* cstr9 = L"ᐊᓕᒍᖅ ᓂᕆᔭᕌᖓᒃᑯ ᓱᕋᙱᑦᑐᓐᓇᖅᑐᖓ";
		//	const wchar_t* cstrs[] = { cstr0, cstr1, cstr2, cstr3, cstr4, cstr5, cstr6, cstr7, cstr8, cstr9 };

		//	for (index_t i = 0; i < 10; i++)
		//	{
		//		std::wstring_view strv0(cstrs[i]);
		//		auto decoded0 = decoder_wchar.DecodeMulti(strv0);
		//		auto encoded0 = encoder_wchar.EncodeMulti(decoded0);
		//		assert(strv0.size() == encoded0.size());
		//		for (index_t j = 0; j < encoded0.size(); j++)
		//		{
		//			assert(encoded0[j] == strv0[j]);
		//		}
		//		std::u16string_view strv1(reinterpret_cast<const char16_t*>(cstrs[i]));
		//		auto decoded1 = decoder_char16_t.DecodeMulti(strv1);
		//		auto encoded1 = encoder_char16_t.EncodeMulti(decoded1);
		//		assert(strv1.size() == encoded1.size());
		//		for (index_t j = 0; j < encoded1.size(); j++)
		//		{
		//			assert(encoded1[j] == strv1[j]);
		//		}
		//	}
		//}
	}
}