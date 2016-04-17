#include "lm_vector_intrin.h"
#include "lm_matrix.h"

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


	lm::Matrix<float, 4, 4> mat;
	mat.rows[0] = float4(1, 0, 0, 1);
	mat.rows[3] = float4(3);
	auto col0 = mat.get_column(0);

	return 0;
}

