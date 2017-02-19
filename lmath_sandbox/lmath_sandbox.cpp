#include "../lmath/lm_vector.h"
#include "../lmath/lm_matrix.h"
#include "../lmath/lm_matrix_traits.h"
#include "../lmath/lm_matrix_intrin.h"
#include <iostream>
#include <type_traits>



int main(){


	static constexpr auto a = lm::Vector<float, 4>(1.0f, 2.0f, 3.0f);
	static constexpr auto b = lm::Matrix<float, 3, 4>(a,a,a);

	auto b1 = lm::IsVector<decltype(a)>::value;
	auto b2 = lm::IsVector<decltype(b)>::value;

	auto s1 = lm::VectorSize<decltype(a)>::value;
	auto s2 = lm::VectorSize<decltype(b)>::value;
	auto s3 = lm::VectorSize<float>::value;

	auto m1_c = lm::MatrixSize<decltype(a)>::columns;
	auto m1_r = lm::MatrixSize<decltype(a)>::rows;
	auto m1_s = lm::MatrixSize<decltype(a)>::value;

	auto m2_c = lm::MatrixSize<decltype(b)>::columns;
	auto m2_r = lm::MatrixSize<decltype(b)>::rows;
	auto m2_s = lm::MatrixSize<decltype(b)>::value;

	bool cm1 = lm::CanMultiplyMatrix<decltype(b), decltype(a)>::Value;
	bool cm2 = lm::CanMultiplyMatrix<decltype(a), decltype(b)>::Value;

	lm::float3 v1{ 1.0f,0.0f,0.0f };
	lm::float3 v2{ 0.0f,1.0f,0.0f };
	lm::float3 v3{ 0.0f,0.0f,1.0f };

	auto result1 = lm::dot(v1, v1);
	auto result2 = lm::dot(v1, v2);
	auto result3 = lm::dot(v1, v3);


	lm::float4x4 world;
	lm::float4 position;

	auto transformed = mul(world, position);

	lm::MatrixColumnType<lm::float4x3>::type p1;
	lm::MatrixRowType<lm::float4x3>::type p2;


	auto transformed2 = mul(world, transformed);
	return 0;
}

