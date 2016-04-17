#include "lm_vector_intrin.h"
#include "lm_matrix_intrin.h"

using namespace lm;

int main() {
	static constexpr float3 f3 = float3::up();

	static constexpr float4 f4 = float4(1, 0, 0, 1);

	static constexpr float4 f4_value = float4(1);

	static constexpr float4 f4_init = { 1.0f,2.0f,3.0f,4.0f};


	lm::vector_traits::field_add_t<float3, double3> fe;

	auto testf = std::is_same<float, decltype(fe)>::value;
	auto testd = std::is_same<double, decltype(fe)>::value;

	lm::vector_traits::field_type<float3>::type ff;
	lm::vector_traits::field_type<float>::type fg;

	static_assert(std::is_same<float, decltype(ff)>::value, "Fail");
	static_assert(std::is_same<float, decltype(fg)>::value, "Fail");


	lm::vector_traits::field_add_t<float3, float> f_1;
	lm::vector_traits::field_add_t<double3, float> f_2;

	static_assert(std::is_same<float, decltype(f_1)>::value, "Fail");
	static_assert(std::is_same<double, decltype(f_2)>::value, "Fail");

	double3 d_add(4);
	float3 f_add(1);
	f_add = f_add + 2;

	auto b0 = f_add + d_add;
	auto b1 = f_add - d_add;
	auto b2 = f_add * d_add;
	auto b3 = f_add / d_add;

	auto b4 = f_add + 2;
	auto b5 = f_add - 2;
	auto b6 = f_add * 2;
	auto b7 = f_add / 2;

	float3 fup = float3::up() * 2;
	double3 dup = double3::up() * 2;

	auto flen_s = lm::length_sq(fup);
	auto flen = lm::length(fup);


	auto dlen_s = lm::length_sq(dup);
	auto dlen = lm::length(dup);


	float4x4 mat;
	mat.rows[0] = float4(1, 0, 0, 1);
	mat.rows[3] = float4(3);
	auto col0 = mat.get_column(0);


	auto mat33 = (float3x3)mat;
	auto mat44 = (float4x4)mat;

	auto mat33_2 = (float3x3)mat;

	auto is_mat1 = lm::matrix_traits::is_matrix<float4x4>::value;
	auto is_mat2 = lm::matrix_traits::is_matrix<float4>::value;

	auto mmul1 = lm::matrix_traits::can_multiply<float3x3, float3x3>::value;
	auto mmul2 = lm::matrix_traits::can_multiply<float3x3, float3x4>::value;
	auto mmul3 = lm::matrix_traits::can_multiply<float3x3, float4x3>::value;

	auto mul_r1 = lm::mul(mat33, mat33_2);


	auto is_identity_constexpr_4x4 = noexcept(float4x4::identity());
	auto is_identity_constexpr_3x3 = noexcept(float3x3::identity());
	auto id0 = float4x4::identity();
	auto id1 = float3x3::identity();
	auto id2 = float2x2::identity();


	typedef Matrix<double, 5, 5> double5x5;
	auto is_identity_constexpr_5x5 = noexcept(double5x5::identity());
	auto id3 = double5x5::identity();

	auto rc = double5x5::rows_count;

	auto mul_r = lm::mul(float3x4(1), float4x3(1));

	auto mul_r2 = lm::mul(float3x3::identity(), float3x3::identity());

	auto mul_r3 = lm::mul(
		float2x3(float3(1,3,2), float3(0,4,-1)), 
		float3x4(float4(2,0,-1,1), float4(3, -2, 1, 2), float4(0, 1, 2, 3)));

	auto is_square1 = lm::matrix_traits::is_square<double5x5>::value;
	return 0;
}

