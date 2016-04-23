#include "lm_vector_intrin.h"
#include "lm_matrix_intrin.h"
#include "lm_common_intrin.h"
#include <iostream>

using namespace concurrency;
using namespace lm;

static void test_float2() restrict(cpu, amp) {
	float2 t = lm::float2(1, 2);
	float2 t1 = t * 2;
	t1 = t1 / 8;
	t1 = t1 + 3;
	t1 = t1 - 2;

	float2 t2 = float2(10);
	float2 t3 = t1 + t2;
	t3 = t1 - t2;
	t3 = t1 * t2;
	t3 = t1 / t2;
}

static void test_float3() restrict(cpu, amp) {
	typedef float3 test_vector_t;
	test_vector_t c0 = test_vector_t();
	test_vector_t c1 = test_vector_t(1);
	test_vector_t c2 = test_vector_t(1,2,3);

	test_vector_t t = test_vector_t(1, 2, 3);
	test_vector_t t1 = t * 2;
	t1 = t1 / 8;
	t1 = t1 + 3;
	t1 = t1 - 2;

	test_vector_t t2 = test_vector_t::up();
	t2 = test_vector_t::right();
	t2 = test_vector_t::forward();
	t2 = test_vector_t(10);

	test_vector_t t3 = t1 + t2;
	t3 = t1 - t2;
	t3 = t1 * t2;
	t3 = t1 / t2;
}

static void test_float4() restrict(cpu, amp) {
	typedef float4 test_vector_t;
	test_vector_t c0 = test_vector_t();
	test_vector_t c1 = test_vector_t(1);
	test_vector_t c2 = test_vector_t(1, 2, 3, 4);

	test_vector_t t = test_vector_t(1, 2, 3, 4);
	test_vector_t t1 = t * 2;
	t1 = t1 / 8;
	t1 = t1 + 3;
	t1 = t1 - 2;

	test_vector_t t2 = test_vector_t(10);

	test_vector_t t3 = t1 + t2;
	t3 = t1 - t2;
	t3 = t1 * t2;
	t3 = t1 / t2;
}

int main() {

	std::vector<lm::float3> amp_vector = { float3(1),float3(2),float3(3) };
	std::vector<lm::float3> amp_result_vector(amp_vector.size());
	array_view<lm::float3, 1> arr(amp_vector);
	array_view<lm::float3, 1> arr_result(amp_result_vector);
	arr_result.discard_data();

	//AMP test
	parallel_for_each(arr.extent, [=](index<1> idx) restrict(amp) {
		/*test_float2();
		test_float3();
		test_float4();*/

		float3x3 m1;
		auto col1 = m1.get_column(0);
		auto m2 = float3x3::identity();
		auto m3 = float4x4::identity();
		auto m4 = lm::mul(m1, m2);
		auto det1 = lm::determinant(m4);

		float3 t3(1);
		auto len_1 = lm::length_sq(t3);
		auto len_2 = lm::length(t3);

		auto norm_1 = lm::normalize(t3);

		auto dist_1 = lm::distance(t3, norm_1);

		auto dot_1 = lm::dot(t3, t3);
		auto cross_1 = lm::cross(float3::up(), float3::right());
		t3.gb.r = 1.0f;
		
		auto asdouble_0 = lm::asdouble(123, 456);
		auto asdouble_1 = lm::asdouble(Vector<uint32_t, 2>(2, 2), Vector<uint32_t, 2>(300, 400));

		auto pow_1 = lm::pow(float4(1, 2, 3, 4), 2.0f);
		auto abs_1 = lm::abs(float4(1, -2, 3, -4));
		auto acos_1 = lm::acos(float4(1, 0, -1, -0.75f));

		arr_result[idx] = lm::normalize(t3) + 2;

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
			float2x3(float3(1, 3, 2), float3(0, 4, -1)),
			float3x4(float4(2, 0, -1, 1), float4(3, -2, 1, 2), float4(0, 1, 2, 3)));

		auto is_square1 = lm::matrix_traits::is_square<double5x5>::value;


		auto asin_1 = lm::asin(float4(1, 0, -1, -0.75f));
		auto atan_1 = lm::atan(float4(1, 0, -1, -0.75f));

		auto all_1 = lm::all(float4(0, 0, 0, 0));
		auto all_2 = lm::all(float4(0, 0, 0, 1));
		auto all_3 = lm::all(float4(0, 0, 1, 1));
		auto all_4 = lm::all(float4(0, 1, 1, 1));
		auto all_5 = lm::all(float4(1, 1, 1, 1));

		auto any_1 = lm::any(float4(0, 0, 0, 0));
		auto any_2 = lm::any(float4(0, 0, 0, 1));
		auto any_3 = lm::any(float4(0, 0, 1, 1));
		auto any_4 = lm::any(float4(0, 1, 1, 1));
		auto any_5 = lm::any(float4(1, 1, 1, 1));




		float4 test_vector = float4(3.3f, 4.4f, 5.5f, 6.6f);
		float3x3 test_matrix = float3x3(float3(1.1f, 2.2f, 3.3f), float3(4.4f, 5.5f, 6.6f), float3(7.7f, 8.8f, 9.9f));

		auto ceil_1 = lm::ceil(test_vector);
		auto ceil_2 = lm::ceil(test_matrix);

		auto clamp_1 = lm::clamp(test_vector, 1.0f, 2.0f);
		auto clamp_2 = lm::clamp(test_matrix, 0.0f, 7.0f);

		auto cos_1 = lm::cos(test_vector);
		auto cos_2 = lm::cos(test_matrix);

		auto cosh_1 = lm::cosh(test_vector);
		auto cosh_2 = lm::cosh(test_matrix);

		auto cross_2 = lm::cross(float3(0, 1, 0), float3(0, 0, 1));
		auto cross_3 = lm::cross(float3(0, 0, 1), float3(0, 1, 0));

		auto degrees_0 = lm::degrees(float3((float)LM_PI / 2.0f, (float)LM_PI / 1.0f, (float)LM_PI / 4.0f));

		auto det_1 = lm::determinant(float2x2(float2(1, 2), float2(-1, 3)));

		auto det_2 = lm::determinant(float3x3::identity());



		auto norm_2 = lm::normalize(float3(1, 2, 4));

		auto p1 = float3::up() / lm::length(float3::up());




		auto dot_2 = lm::dot(float3::up(), float3::up());
		auto dot_3 = lm::dot(float3::up(), lm::normalize(float3::up() + float3::right()));

		auto deg_1 = lm::degrees(concurrency::precise_math::acos(dot_1));
		auto deg_2 = lm::degrees(concurrency::precise_math::acos(dot_2));
		auto deg_3 = lm::degrees(concurrency::precise_math::acos(dot_3));



	});

	arr_result.synchronize(access_type_auto);
	
	auto val = std::is_same<float, typename float3::element_type>::value;

	static_assert(sizeof(float2) == sizeof(float) * 2, "Float2 size incorrect");
	static_assert(sizeof(double2) == sizeof(double) * 2, "double2 size incorrect");

	static_assert(sizeof(float3) == sizeof(float) * 3, "Float2 size incorrect");
	static_assert(sizeof(double3) == sizeof(double) * 3, "double3 size incorrect");

	static_assert(sizeof(float4) == sizeof(float) * 4, "Float4 size incorrect");
	static_assert(sizeof(double4) == sizeof(double) * 4, "double4 size incorrect");

	//CPU test
	test_float2();
	test_float3();
	test_float4();

	auto asdouble_0 = lm::asdouble(123, 456);
	auto asdouble_1 = lm::asdouble(Vector<uint32_t, 2>(2, 2), Vector<uint32_t, 2>(300, 400));


	float3x3 m1;
	auto col1 = m1.get_column(0);
	auto m2 = float3x3::identity();
	auto m3 = float4x4::identity();
	auto m4 = lm::mul(m1, m2);
	auto det1 = lm::determinant(m4);

	float3 t3(1);
	auto len_1 = lm::length_sq(t3);
	auto len_2 = lm::length(t3);
	auto norm_1 = lm::normalize(t3);
	auto dist_1 = lm::distance(t3, norm_1);
	auto dot_1 = lm::dot(lm::normalize(t3), lm::normalize(t3));

	t3.gb.r = 1.0f;

	
	std::cin.get();


	//static_assert(noexcept(Test_amp()), "Not constexpr (");
	
	static constexpr float3 f3 = float3::up();

	static constexpr float4 f4 = float4(1, 0, 0, 1);

	static constexpr float4 f4_value = float4(1);

	static constexpr float4 f4_init = { 1.0f,2.0f,3.0f,4.0f};


	
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

	auto pow_1 = lm::pow(float4(1, 2, 3, 4), 2.0f);
	auto abs_1 = lm::abs(float4(1, -2, 3, -4));
	auto acos_1 = lm::acos(float4(1, 0, -1, -0.75f));

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


	
	
	auto asin_1 = lm::asin(float4(1, 0, -1, -0.75f));
	auto atan_1 = lm::atan(float4(1, 0, -1, -0.75f));

	auto all_1 = lm::all(float4(0, 0, 0, 0));
	auto all_2 = lm::all(float4(0, 0, 0, 1));
	auto all_3 = lm::all(float4(0, 0, 1, 1));
	auto all_4 = lm::all(float4(0, 1, 1, 1));
	auto all_5 = lm::all(float4(1, 1, 1, 1));

	auto any_1 = lm::any(float4(0, 0, 0, 0));
	auto any_2 = lm::any(float4(0, 0, 0, 1));
	auto any_3 = lm::any(float4(0, 0, 1, 1));
	auto any_4 = lm::any(float4(0, 1, 1, 1));
	auto any_5 = lm::any(float4(1, 1, 1, 1));

	
	

	float4 test_vector = float4(3.3f, 4.4f, 5.5f, 6.6f);
	float3x3 test_matrix = float3x3(float3(1.1f, 2.2f, 3.3f), float3(4.4f, 5.5f, 6.6f), float3(7.7f, 8.8f, 9.9f));

	auto ceil_1 = lm::ceil(test_vector);
	auto ceil_2 = lm::ceil(test_matrix);

	auto clamp_1 = lm::clamp(test_vector, 1.0f, 2.0f);
	auto clamp_2 = lm::clamp(test_matrix, 0.0f, 7.0f);

	auto cos_1 = lm::cos(test_vector);
	auto cos_2 = lm::cos(test_matrix);

	auto cosh_1 = lm::cosh(test_vector);
	auto cosh_2 = lm::cosh(test_matrix);

	auto cross_1 = lm::cross(float3(0, 0, 1), float3(1, 0, 0));
	auto cross_2 = lm::cross(float3(0, 1, 0), float3(0, 0, 1));
	auto cross_3 = lm::cross(float3(0, 0, 1), float3(0, 1, 0));

	auto degrees_0 = lm::degrees(float3((float)LM_PI / 2.0f, (float)LM_PI / 1.0f, (float)LM_PI / 4.0f));

	auto det_1 = lm::determinant(float2x2(float2(1, 2), float2(-1, 3)));

	auto det_2 = lm::determinant(float3x3::identity());



	auto norm_2 = lm::normalize(float3(1, 2, 4));

	auto p1 = float3::up() / lm::length(float3::up());




	auto dot_2 = lm::dot(float3::up(), float3::up());
	auto dot_3 = lm::dot(float3::up(), lm::normalize(float3::up() + float3::right()));

	auto deg_1 = lm::degrees(std::acos(dot_1));
	auto deg_2 = lm::degrees(std::acos(dot_2));
	auto deg_3 = lm::degrees(std::acos(dot_3));


	

	return 0;
}

