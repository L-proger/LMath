#include "stdafx.h"
#include "CppUnitTest.h"
#include "../lmath/lmath.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace lm;

namespace lmath_test{		
	TEST_CLASS(lm_traits_test_class){
	public:
		TEST_METHOD(lm_traits_test){
			Assert::IsTrue(std::is_same<lm::vector_traits::field_type<float2>::type, float>::value, L"Float3 type test failed");
			Assert::IsTrue(std::is_same<lm::vector_traits::field_type<double3>::type, double>::value, L"double3 type test failed");
			Assert::IsTrue(std::is_same<lm::vector_traits::field_type<Vector<int,4>>::type, int>::value, L"int3 type test failed");

			Assert::IsFalse(std::is_same<lm::vector_traits::field_type<float2>::type, double>::value, L"Float3 type test failed");
			Assert::IsFalse(std::is_same<lm::vector_traits::field_type<double3>::type, float>::value, L"double3 type test failed");
			Assert::IsFalse(std::is_same<lm::vector_traits::field_type<Vector<int, 4>>::type, short>::value, L"int3 type test failed");
		}
	};
}