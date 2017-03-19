#include "stdafx.h"
#include "CppUnitTest.h"
#include "../lmath/lmath.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace lm;

namespace lmath_test {
	TEST_CLASS(lm_vector_test_class) {
public:
	TEST_METHOD(lm_vector_test_min) {
		Assert::IsTrue(lm::min(1.0f, 2.0f) == 1.0f);
		Assert::IsTrue(lm::min(2.0f, 1.0f) == 1.0f);

		float3 v1(1.0f, 2.0f, 3.0f);
		float3 v2(-5.0f, 0.0f, 5.0f);

		const float epsilon = 0.001f;
		Assert::IsTrue(lm::min(v1, v2).equals(float3(-5.0f, 0.0f, 3.0f), epsilon));
		Assert::IsTrue(lm::min(v2, v1).equals(float3(-5.0f, 0.0f, 3.0f), epsilon));
		Assert::IsTrue(lm::max(v1, v2).equals(float3(1.0f, 2.0f, 5.0f), epsilon));
		Assert::IsTrue(lm::max(v2, v1).equals(float3(1.0f, 2.0f, 5.0f), epsilon));

		Assert::AreEqual(1.0f, lm::min(v1), epsilon);
		Assert::AreEqual(-5.0f, lm::min(v2), epsilon);

		Assert::AreEqual(3.0f, lm::max(v1), epsilon);
		Assert::AreEqual(5.0f, lm::max(v2), epsilon);
	}
	TEST_METHOD(lm_vector_test_operators) {
		float3 v1(0.0f);
		float3 v2(2.0f);
		double3 v3(0.0);

		Assert::IsTrue(v1 == v1);
		Assert::IsFalse(v1 != v1);

		Assert::IsFalse(v1 == v2);
		Assert::IsTrue(v1 != v2);

		Assert::IsTrue(v1 == v3);
		Assert::IsFalse(v1 != v3);
		const float epsilon = 0.0001f;

		v2 = v2 / 2.0f;
		Assert::IsTrue(v2.equals(float3(1.0f), epsilon));
		v2 = v2 * 5.0f;
		Assert::IsTrue(v2.equals(float3(5.0f), epsilon));
		v2 = v2 + 5.0f;
		Assert::IsTrue(v2.equals(float3(10.0f), epsilon));
		v2 = v2 - 9.0f;
		Assert::IsTrue(v2.equals(float3(1.0f), epsilon));
		v2 *= 3.0f;
		Assert::IsTrue(v2.equals(float3(3.0f), epsilon));
		v2 /= 6.0f;
		Assert::IsTrue(v2.equals(float3(0.5f), epsilon));
		v2 += 1.5f;
		Assert::IsTrue(v2.equals(float3(2.0f), epsilon));
		v2 -= 2.0f;
		Assert::IsTrue(v2.equals(float3(0.0f), epsilon));

	}
	TEST_METHOD(lm_vector_test_constructors_N1) {
		Vector<float, 1> v1{};
		Vector<float, 1> v2(2.2f);
		Vector<float, 1> v3{ 3.3f };
		Vector<float, 1> v4 = v3;

		Vector<float, 1> v5;
		v5 = v3;

		Assert::AreEqual(2.2f, v2[0], L"v2 ctor fail");
		Assert::AreEqual(3.3f, v3[0], L"v3 ctor fail");
		Assert::AreEqual(3.3f, v4[0], L"v4 ctor fail");
		Assert::AreEqual(3.3f, v5[0], L"v5 ctor fail");
	}
	TEST_METHOD(lm_vector_test_constructors_N2) {
		Vector<float, 2> v1{};
		Vector<float, 2> v2(2.2f, 3.3f);
		Vector<float, 2> v3{ 4.4f };
		Assert::AreEqual(2.2f, v2[0], L"v2 ctor fail");
		Assert::AreEqual(3.3f, v2[1], L"v2 ctor fail");
		Assert::AreEqual(4.4f, v3[0], L"v3 ctor fail");
		Assert::AreEqual(4.4f, v3[1], L"v3 ctor fail");
	}
	TEST_METHOD(lm_vector_test_constructors_N3) {
		Vector<float, 3> v1{};
		Vector<float, 3> v2(2.2f, 3.3f, 4.4f);
		Vector<float, 3> v3{ 5.5f };

		Assert::AreEqual(2.2f, v2[0], L"v2 ctor fail");
		Assert::AreEqual(3.3f, v2[1], L"v2 ctor fail");
		Assert::AreEqual(4.4f, v2[2], L"v2 ctor fail");

		Assert::AreEqual(5.5f, v3[0], L"v3 ctor fail");
		Assert::AreEqual(5.5f, v3[1], L"v3 ctor fail");
		Assert::AreEqual(5.5f, v3[2], L"v3 ctor fail");

		Vector<float, 2> v4(1.1f, 2.2f);
		Vector<float, 3> v5(v4, 3.3f);

		Assert::AreEqual(1.1f, v5[0], L"v5 ctor fail");
		Assert::AreEqual(2.2f, v5[1], L"v5 ctor fail");
		Assert::AreEqual(3.3f, v5[2], L"v5 ctor fail");

		Vector<float, 3> v6(3.3f, v4);

		Assert::AreEqual(3.3f, v6[0], L"v6 ctor fail");
		Assert::AreEqual(1.1f, v6[1], L"v6 ctor fail");
		Assert::AreEqual(2.2f, v6[2], L"v6 ctor fail");
	}
	TEST_METHOD(lm_vector_test_constructors_N4) {
		Vector<float, 4> v1{};
		Vector<float, 4> v2(1.1f, 2.2f, 3.3f, 4.4f);
		Vector<float, 4> v3{ 5.5f };

		Assert::AreEqual(1.1f, v2[0], L"v2 ctor fail");
		Assert::AreEqual(2.2f, v2[1], L"v2 ctor fail");
		Assert::AreEqual(3.3f, v2[2], L"v2 ctor fail");
		Assert::AreEqual(4.4f, v2[3], L"v2 ctor fail");

		Assert::AreEqual(5.5f, v3[0], L"v3 ctor fail");
		Assert::AreEqual(5.5f, v3[1], L"v3 ctor fail");
		Assert::AreEqual(5.5f, v3[2], L"v3 ctor fail");
		Assert::AreEqual(5.5f, v3[3], L"v3 ctor fail");

		Vector<float, 2> p2_1{ 6.6f, 7.7f };
		Vector<float, 2> p2_2{ 8.8f, 9.9f };
		Vector<float, 3> p3{ 6.0f, 7.0f, 8.0f };

		Vector<float, 4> v4{ p2_1, p2_2 };
		Assert::AreEqual(p2_1[0], v4[0], L"v4 ctor fail");
		Assert::AreEqual(p2_1[1], v4[1], L"v4 ctor fail");
		Assert::AreEqual(p2_2[0], v4[2], L"v4 ctor fail");
		Assert::AreEqual(p2_2[1], v4[3], L"v4 ctor fail");

		Vector<float, 4> v5(p2_1, 2.0f, 3.0f);
		Assert::AreEqual(p2_1[0], v5[0], L"v5 ctor fail");
		Assert::AreEqual(p2_1[1], v5[1], L"v5 ctor fail");
		Assert::AreEqual(2.0f, v5[2], L"v5 ctor fail");
		Assert::AreEqual(3.0f, v5[3], L"v5 ctor fail");

		Vector<float, 4> v6(2.0f, 3.0f, p2_1);
		Assert::AreEqual(2.0f, v6[0], L"v6 ctor fail");
		Assert::AreEqual(3.0f, v6[1], L"v6 ctor fail");
		Assert::AreEqual(p2_1[0], v6[2], L"v6 ctor fail");
		Assert::AreEqual(p2_1[1], v6[3], L"v6 ctor fail");


		Vector<float, 4> v7(p3, 9.9f);
		Assert::AreEqual(p3[0], v7[0], L"v7 ctor fail");
		Assert::AreEqual(p3[1], v7[1], L"v7 ctor fail");
		Assert::AreEqual(p3[2], v7[2], L"v7 ctor fail");
		Assert::AreEqual(9.9f, v7[3], L"v7 ctor fail");

		Vector<float, 4> v8(9.9f, p3);
		Assert::AreEqual(9.9f, v8[0], L"v8 ctor fail");
		Assert::AreEqual(p3[0], v8[1], L"v8 ctor fail");
		Assert::AreEqual(p3[1], v8[2], L"v8 ctor fail");
		Assert::AreEqual(p3[2], v8[3], L"v8 ctor fail");

	}
	TEST_METHOD(lm_vector_test_constructors_N) {
		Vector<float, 7> v1(5.5f);
		for (LmSize i = 0; i < 7; ++i) {
			Assert::AreEqual(5.5f, v1[i], L"Vector N ctor fail");
		}

		Vector<float, 7> v2(0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f);
		for (LmSize i = 0; i < 7; ++i) {
			Assert::AreEqual((float)i, v2[i], L"Vector N ctor fail");
		}

		v1 = v2;

		for (LmSize i = 0; i < 7; ++i) {
			Assert::AreEqual((float)i, v1[i], L"Vector N ctor fail");
		}
	}
	TEST_METHOD(lm_vector_test_slice) {
		float3 v1(1.0f, 2.0f, 3.0f);
		
		auto& s1 = v1.slice<0, 1>();
		Assert::AreEqual((LmSize)1, s1.Size, L"Invalid slice size");
		Assert::AreEqual(1.0f, s1[0], L"Invalid slice value");

		auto& s2 = v1.slice<0, 2>();
		Assert::AreEqual((LmSize)2, s2.Size, L"Invalid slice size");
		Assert::AreEqual(1.0f, s2[0], L"Invalid slice value");
		Assert::AreEqual(2.0f, s2[1], L"Invalid slice value");

		auto& s3 = v1.slice<0, 3>();
		Assert::AreEqual((LmSize)3, s3.Size, L"Invalid slice size");
		Assert::AreEqual(1.0f, s3[0], L"Invalid slice value");
		Assert::AreEqual(2.0f, s3[1], L"Invalid slice value");
		Assert::AreEqual(3.0f, s3[2], L"Invalid slice value");

		auto& s4 = v1.slice<1, 1>();
		Assert::AreEqual((LmSize)1, s4.Size, L"Invalid slice size");
		Assert::AreEqual(2.0f, s4[0], L"Invalid slice value");

		auto& s5 = v1.slice<2, 1>();
		Assert::AreEqual((LmSize)1, s5.Size, L"Invalid slice size");
		Assert::AreEqual(3.0f, s5[0], L"Invalid slice value");
	}
	TEST_METHOD(lm_vector_test_dot) {
		double3 x{ 1.0, 0.0, 0.0 };
		double3 y{ 0.0, 1.0, 0.0 };
		double3 z{ 0.0, 0.0, 1.0 };

		double3 _x{ -1.0, 0.0, 0.0 };

		const double dotTolerance = 0.01;
		Assert::AreEqual(1.0, lm::dot(x, x), dotTolerance, L"Dot fail");
		Assert::AreEqual(1.0, lm::dot(y, y), dotTolerance, L"Dot fail");
		Assert::AreEqual(1.0, lm::dot(z, z), dotTolerance, L"Dot fail");

		Assert::AreEqual(0.0, lm::dot(x, y), dotTolerance, L"Dot fail");
		Assert::AreEqual(0.0, lm::dot(y, z), dotTolerance, L"Dot fail");
		Assert::AreEqual(0.0, lm::dot(x, z), dotTolerance, L"Dot fail");

		Assert::AreEqual(-1.0, lm::dot(x, _x), dotTolerance, L"Dot fail");

		double3 w{ 1.0, 1.0, 0.0 };
		w = lm::normalize(w);

		Assert::AreEqual(std::cos(lm::pi_d / 4.0), lm::dot(y, w), dotTolerance, L"Dot fail");
		Assert::AreEqual(-std::cos(lm::pi_d / 4.0), lm::dot(y, w * -1.0f), dotTolerance, L"Dot fail");
	}
	TEST_METHOD(lm_vector_test_trigonometry) {
		float3 v{ 2.0f, 3.0f, 4.0f };

		auto vSin = lm::sin(v);

		Assert::AreEqual(std::sin(v[0]), vSin[0], L"Sin fail");
		Assert::AreEqual(std::sin(v[1]), vSin[1], L"Sin fail");
		Assert::AreEqual(std::sin(v[2]), vSin[2], L"Sin fail");
	}
	TEST_METHOD(lm_vector_test_all_any) {
		float3 v1{ 0.0f, 0.0f, 0.0f };
		float3 v2{ 0.0f, 0.0f, 3.0f };
		float3 v3{ 3.0f, 3.0f, 3.0f };

		Assert::IsFalse(lm::all(v1));
		Assert::IsFalse(lm::any(v1));

		Assert::IsFalse(lm::all(v2));
		Assert::IsTrue(lm::any(v2));

		Assert::IsTrue(lm::all(v3));
		Assert::IsTrue(lm::any(v3));
	}
	TEST_METHOD(lm_vector_test_unit) {
		auto x2 = float2::unitX();
		auto y2 = float2::unitY();

		Assert::AreEqual(1.0f, x2[0]);
		Assert::AreEqual(0.0f, x2[1]);

		Assert::AreEqual(0.0f, y2[0]);
		Assert::AreEqual(1.0f, y2[1]);

		auto z3 = float3::unitZ();
		auto w4 = float4::unitW();

		Assert::AreEqual(0.0f, z3[0]);
		Assert::AreEqual(0.0f, z3[1]);
		Assert::AreEqual(1.0f, z3[2]);


		Assert::AreEqual(0.0f, w4[0]);
		Assert::AreEqual(0.0f, w4[1]);
		Assert::AreEqual(0.0f, w4[2]);
		Assert::AreEqual(1.0f, w4[3]);
	}
	TEST_METHOD(lm_vector_test_length_lengthSquared) {
		float3 v1{ 1.0f, 0.0f, 0.0f };
		float3 v2{ 0.0f, 2.0f, 0.0f };
		float3 v3{ 1.0f, 1.0f, 0.0f };
		const float tolerance = 0.001f;

		Assert::AreEqual(1.0f, v1.length(), tolerance);
		Assert::AreEqual(1.0f, v1.lengthSquared(), tolerance);

		Assert::AreEqual(2.0f, v2.length(), tolerance);
		Assert::AreEqual(4.0f, v2.lengthSquared(), tolerance);

		Assert::AreEqual(std::sqrt(2.0f), v3.length(), tolerance);
		Assert::AreEqual(2.0f, v3.lengthSquared(), tolerance);
	}

	TEST_METHOD(lm_vector_test_equals) {
		float3 v1(1.0f, 2.0f, 3.0f);
		float3 v2(2.0f, 2.0f, 3.0f);

		float tolerance = 0.001f;
		Assert::IsTrue(v1.equals(v1, tolerance));
		Assert::IsFalse(v1.equals(v2, tolerance));

		for (LmSize i = 0; i < 3; ++i) {
			v2[i] = v1[i] + tolerance * 2.0f;
		}

		Assert::IsFalse(v1.equals(v2, tolerance));

	}

	TEST_METHOD(lm_vector_test_cross) {
		float3 x{ 1.0f, 0.0f, 0.0f };
		float3 y{ 0.0f, 1.0f, 0.0f };
		float3 z{ 0.0f, 0.0f, 1.0f };
		const float tolerance = 0.001f;

		auto c1 = lm::cross(y, z);
		auto c2 = lm::cross(z, y);

		Assert::AreEqual(1.0f, c1[0], tolerance);
		Assert::AreEqual(0.0f, c1[1], tolerance);
		Assert::AreEqual(0.0f, c1[2], tolerance);

		Assert::AreEqual(-1.0f, c2[0], tolerance);
		Assert::AreEqual(0.0f, c2[1], tolerance);
		Assert::AreEqual(0.0f, c2[2], tolerance);

	}

	TEST_METHOD(lm_vector_test_normalize) {
		float3 v1{ 1.0f,0.0f, 0.0f };
		float3 v2{ 2.0f,0.0f, 0.0f };
		float3 v3{ 2.0f,2.0f, 2.0f };
		float3 v4{ -2.0f, -2.0f, -2.0f };
		
		const float tolerance = 0.001f;
		Assert::AreEqual(1.0f, v1.normalized().length(), tolerance);
		Assert::AreEqual(1.0f, v2.normalized().length(), tolerance);
		Assert::AreEqual(1.0f, v3.normalized().length(), tolerance);
		Assert::AreEqual(1.0f, v4.normalized().length(), tolerance);
	}

	TEST_METHOD(lm_vector_test_lerp) {
		float3 v1{ -1.0f };
		float3 v2{ 1.0f};

		const float tolerance = 0.001f;
		Assert::IsTrue(lm::lerp(v1, v2, 0.0f).equals(v1, tolerance));
		Assert::IsTrue(lm::lerp(v1, v2, 1.0f).equals(v2, tolerance));
		Assert::IsTrue(lm::lerp(v1, v2, 0.5f).equals(float3(0.0f, 0.0f, 0.0f), tolerance));
		Assert::IsTrue(lm::lerp(v1, v2, 0.1f).equals(float3(-0.8f, -0.8f, -0.8f), tolerance));
		Assert::IsTrue(lm::lerp(v1, v2, 0.9f).equals(float3(0.8f, 0.8f, 0.8f), tolerance));
	}




	};
}