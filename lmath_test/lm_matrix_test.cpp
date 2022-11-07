#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Src/lmath/lmath.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace lm;

namespace lmath_test {
	TEST_CLASS(lm_matrix_test_class) {
public:
	TEST_METHOD(lm_matrix_test) {
		lm::float3 z = lm::float3::unitZ();
		auto ryp90 = lm::matrix4x4RotationY<float>(lm::pi_f / 2.0f);
		auto ryn90 = lm::matrix4x4RotationY<float>(-lm::pi_f / 2.0f);
		auto xp = lm::mul(ryp90, lm::float4(z, 1.0f)).slice<0,3>();
		auto xn = lm::mul(ryn90, lm::float4(z, 1.0f)).slice<0, 3>();

		Assert::IsTrue(xp.equals(lm::float3::unitX(), 0.001f));
		Assert::IsTrue(xn.equals(lm::float3::unitX() * -1.0f, 0.001f));
	}

	TEST_METHOD(lm_matrix_rotation_quaternion) {
		auto m0 = lm::matrix4x4RotationY<float>(lm::pi_f / 2.0f);
		auto m1 = matrix4x4RotationQuaternion(lm::Quaternion_f::angleAxis(lm::pi_f / 2.0f, float3::unitY()));
		
		for (LmSize y = 0; y < 4; ++y) {
			for (LmSize x = 0; x < 4; ++x) {
				Assert::AreEqual(m0[y][x], m1[y][x], 0.0001f);
			}
		}
	}
	};
}