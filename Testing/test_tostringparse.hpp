#pragma once
#include "jhtstring.hpp"

namespace jht
{
	template<typename T>
	inline void RoundTrip(T value)
	{
		String str = ToString(value);
		T output = T();
		assert(TryParse(str, output));
		assert(value == output);
	}

	inline void RoundTripDouble(fp64_t value)
	{
		fp64_t epsilon = 0.000001;
		String str = ToString(value);
		fp64_t output = 0;
		assert(TryParse(str, output));
		assert(abs(value - output) < epsilon);
	}

	inline void RunTests_ToStringParse()
	{
		std::cout << "RunTests_ToStringParse\n";

		RoundTrip(0);
		RoundTrip(-1);
		RoundTrip(1);
		RoundTrip(UINT64_MAX);
		RoundTrip(INT64_MIN);
		RoundTrip(INT64_MAX);
		RoundTripDouble(-432.342);
		RoundTripDouble(-432.34234);
		RoundTripDouble(-432.3423445379834574353498);
		RoundTripDouble(-4322343242343242334.3423445379834574353498);
		RoundTrip(true);
		RoundTrip(false);

		int64_t intResult = 0;
		uint64_t uintResult = 0;
		fp64_t fpResult = 0;

		assert(TryParse("-0", intResult));
		assert(!TryParse("-1", uintResult));
		assert(TryParse("1234567890", intResult));
		assert(TryParse("1234567890", uintResult));
		assert(TryParse("1234567890", fpResult));

	}
}