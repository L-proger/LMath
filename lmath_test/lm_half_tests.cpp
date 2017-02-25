#include "stdafx.h"
#include "CppUnitTest.h"
#include "../lmath/lmath.h"
#include <limits>
#include <cmath>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace lm;

namespace lmath_test {
	TEST_CLASS(lm_half_test_class) {
public:
	TEST_METHOD(lm_half_test) {
		const float tolerance = 0.01f;
		Half h1;

		h1 = 1.0f;
		Assert::AreEqual(1.0f, h1.toFloat(), tolerance);
		h1 = 0.0f;
		Assert::AreEqual(0.0f, h1.toFloat(), tolerance);
		h1 = -1.0f;
		Assert::AreEqual(-1.0f, h1.toFloat(), tolerance);
		h1 = -2.34f;
		Assert::AreEqual(-2.34f, h1.toFloat(), tolerance);

		
		Assert::IsFalse(h1.isNan());
		Assert::IsFalse(std::isnan(h1.toFloat()));
		Assert::IsFalse(h1.isInf());
		Assert::IsFalse(std::isinf(h1.toFloat()));

		h1 = std::numeric_limits<float>::signaling_NaN();
		Assert::IsTrue(h1.isNan());
		Assert::IsTrue(std::isnan(h1.toFloat()));

		h1 = std::numeric_limits<float>::infinity();
		Assert::IsTrue(h1.isInf());
		Assert::IsTrue(std::isinf(h1.toFloat()));

	}
	};
}