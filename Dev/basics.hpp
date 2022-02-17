#pragma once
#include <cstring>

#pragma region Compile target agnostic type definitions

#include <stdint.h>
using fp32_t = float;
using fp64_t = double;
using index_t = ptrdiff_t;

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
#pragma region min/max/abs

#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef max
#define max(a, b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef clamp
#define clamp(v, lower, upper) max(min((v),(upper)),(lower)) 
#endif

#ifndef abs
#define abs(v) (((v) < 0) ? (-(v)) : (v))
#endif

#pragma endregion
#pragma region Template Helper Methods

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
#pragma region Iteration

#ifndef JHT_ITERATE
#define JHT_ITERATE(type, obj, iter) for (type::Iterator iter(obj); iter; ++iter)
#endif

#pragma endregion

}