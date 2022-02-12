#pragma once
#include <cstring>

#pragma region Compile target agnostic type definitions

#include <stdint.h>
using fp32_t = float;
using fp64_t = double;

#pragma endregion

namespace jht
{
#pragma region Function Pointer Templates

	template<typename TRet>
	using FuncPtr0 = TRet(*)();
	template<typename TRet, typename TIn0>
	using FuncPtr1 = TRet(*)(TIn0);
	template<typename TRet, typename TIn0, typename TIn1>
	using FuncPtr2 = TRet(*)(TIn0, TIn1);
	template<typename TRet, typename TIn0, typename TIn1, typename TIn2>
	using FuncPtr3 = TRet(*)(TIn0, TIn1, TIn2);
	template<typename TRet, typename TIn0, typename TIn1, typename TIn2, typename TIn3>
	using FuncPtr4 = TRet(*)(TIn0, TIn1, TIn2, TIn3);
	template<typename TRet, typename TIn0, typename TIn1, typename TIn2, typename TIn3, typename TIn4>
	using FuncPtr5 = TRet(*)(TIn0, TIn1, TIn2, TIn3, TIn4);

#pragma endregion
#pragma region Template Helper Methods

	template<typename T>
	constexpr inline T MIN(const T in1, const T in2)
	{
		if (in1 < in2) return in1;
		return in2;
	}

	template<typename T>
	constexpr inline T MAX(const T in1, const T in2)
	{
		if (in1 > in2) return in1;
		return in2;
	}

	template<typename T>
	constexpr inline T ABS(const T in)
	{
		if (in < (T)0)
		{
			return -in;
		}
		return in;
	}

	constexpr inline int8_t		MIN(const int8_t in1, const int8_t in2) { return MIN<int8_t>(in1, in2); }
	constexpr inline int16_t	MIN(const int16_t in1, const int16_t in2) { return MIN<int16_t>(in1, in2); }
	constexpr inline int32_t	MIN(const int32_t in1, const int32_t in2) { return MIN<int32_t>(in1, in2); }
	constexpr inline int64_t	MIN(const int64_t in1, const int64_t in2) { return MIN<int64_t>(in1, in2); }
	constexpr inline uint8_t	MIN(const uint8_t in1, const uint8_t in2) { return MIN<uint8_t>(in1, in2); }
	constexpr inline uint16_t	MIN(const uint16_t in1, const uint16_t in2) { return MIN<uint16_t>(in1, in2); }
	constexpr inline uint32_t	MIN(const uint32_t in1, const uint32_t in2) { return MIN<uint32_t>(in1, in2); }
	constexpr inline uint64_t	MIN(const uint64_t in1, const uint64_t in2) { return MIN<uint64_t>(in1, in2); }
	constexpr inline fp32_t		MIN(const fp32_t in1, const fp32_t in2) { return MIN<fp32_t>(in1, in2); }
	constexpr inline fp64_t		MIN(const fp64_t in1, const fp64_t in2) { return MIN<fp64_t>(in1, in2); }
	constexpr inline int8_t		MAX(const int8_t in1, const int8_t in2) { return MAX<int8_t>(in1, in2); }
	constexpr inline int16_t	MAX(const int16_t in1, const int16_t in2) { return MAX<int16_t>(in1, in2); }
	constexpr inline int32_t	MAX(const int32_t in1, const int32_t in2) { return MAX<int32_t>(in1, in2); }
	constexpr inline int64_t	MAX(const int64_t in1, const int64_t in2) { return MAX<int64_t>(in1, in2); }
	constexpr inline uint8_t	MAX(const uint8_t in1, const uint8_t in2) { return MAX<uint8_t>(in1, in2); }
	constexpr inline uint16_t	MAX(const uint16_t in1, const uint16_t in2) { return MAX<uint16_t>(in1, in2); }
	constexpr inline uint32_t	MAX(const uint32_t in1, const uint32_t in2) { return MAX<uint32_t>(in1, in2); }
	constexpr inline uint64_t	MAX(const uint64_t in1, const uint64_t in2) { return MAX<uint64_t>(in1, in2); }
	constexpr inline fp32_t		MAX(const fp32_t in1, const fp32_t in2) { return MAX<fp32_t>(in1, in2); }
	constexpr inline fp64_t		MAX(const fp64_t in1, const fp64_t in2) { return MAX<fp64_t>(in1, in2); }
	constexpr inline int64_t	ABS(const int64_t in) { return ABS<int64_t>(in); }
	constexpr inline int32_t	ABS(const int32_t in) { return ABS<int32_t>(in); }
	constexpr inline int16_t	ABS(const int16_t in) { return ABS<int16_t>(in); }
	constexpr inline int8_t		ABS(const int8_t in) { return ABS<int8_t>(in); }
	constexpr inline fp64_t		ABS(const fp64_t in) { return ABS<fp64_t>(in); }
	constexpr inline fp32_t		ABS(const fp32_t in) { return ABS<fp32_t>(in); }

	template<typename T>
	void ArrCopy(T* dest, const T* source, size_t count)
	{
		count *= sizeof(T);
		std::memcpy(dest, source, count);
	}
	template<typename T>
	void ArrNull(T* dest, size_t count)
	{
		count *= sizeof(T);
		std::memset(dest, 0, count);
	}

#pragma endregion
#pragma region Type Forward Declarations

	template<typename T>
	class IndexEnumerator;
	template<typename T>
	class ConstIndexEnumerator;
	template<typename T>
	class IContainer;
	template<typename T>
	class IArrayContainer;
	template<typename T>
	class Vector;
	template<typename T, int32_t A>
	class AVector;
	template<typename T>
	class DynamicBuffer;


#pragma endregion
	class Error
	{
	public:
		const char8_t* const Message;
		const char8_t* const File;
		const uint32_t Line;

	public:

		inline explicit Error(
			const char8_t* message
		) : Message(message), File(nullptr), Line(0)
		{}

		inline Error(
			const char8_t* message,
			const char8_t* file,
			uint32_t line
		) : Message(message), File(file), Line(line)
		{}

		static void  Throw(
			const char8_t* message
		)
		{
			throw Error(message);
		}

		static void Throw(
			const char8_t* message,
			const char8_t* file,
			uint32_t line
		)
		{
			throw Error(message, file, line);
		}
	};

}

#define __JHT_WIDEN_HIDDEN(x) u8 ## x
#define JHT_U8STR(x) __JHT_WIDEN_HIDDEN(x)

#define JHT_FILEU8 JHT_U8STR(__FILE__)

// Macro for assertion including line number and file name
#define JHT_ASSERT(expr, message) if (!(expr)) ::jht::Error::Throw(message, JHT_FILEU8, __LINE__)

// Macro for throwing an exception including line number and file name
#define JHT_THROW(message) ::jht::Error::Throw(message, TECT_FILEW, __LINE__)