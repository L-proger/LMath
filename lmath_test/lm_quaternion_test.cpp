#include "stdafx.h"
#include "CppUnitTest.h"
#include "../lmath/lmath.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace lm;

namespace lmath_test {
	TEST_CLASS(lm_quaternion_test_class) {
public:
	TEST_METHOD(lm_quaternion_test) {
		lm::Quaternion<float> q(0.0f, 1.0f, 2.0f, 3.0f);
		Assert::AreEqual(0.0f, q[0]);
		Assert::AreEqual(1.0f, q[1]);
		Assert::AreEqual(2.0f, q[2]);
		Assert::AreEqual(3.0f, q[3]);

		q = Quaternion<float>::identity();
		Assert::AreEqual(0.0f, q[0]);
		Assert::AreEqual(0.0f, q[1]);
		Assert::AreEqual(0.0f, q[2]);
		Assert::AreEqual(1.0f, q[3]);

		
		lm::Quaternion<float> q2 = q;

		auto q1 = lm::Quaternion_f::angleAxis(lm::pi_f / 2.0f, float3::unitY());

		Assert::AreEqual(0.0f, q1[0], L"Q0");
		Assert::AreEqual(lm::cos(lm::pi_f / 4.0f), q1[1], L"Q1");
		Assert::AreEqual(0.0f, q1[2], L"Q2");
		Assert::AreEqual(lm::cos(lm::pi_f / 4.0f), q1[3], L"Q3");

		q1 = q1 * q1;

		auto q3 = lm::Quaternion_f::angleAxis(lm::pi_f, float3::unitY());

		const float epsilon = 0.0001f;
		Assert::AreEqual(q3[0], q1[0], epsilon, L"Q0");
		Assert::AreEqual(q3[1], q1[1], epsilon, L"Q1");
		Assert::AreEqual(q3[2], q1[2], epsilon, L"Q2");
		Assert::AreEqual(q3[3], q1[3], epsilon, L"Q3");

		Assert::AreEqual(0.0f, q1[0], epsilon, L"Q0");
		Assert::AreEqual(1.0f, q1[1], epsilon, L"Q1");
		Assert::AreEqual(0.0f, q1[2], epsilon, L"Q2");
		Assert::AreEqual(0.0f, q1[3], epsilon, L"Q3");

	}
	};
}