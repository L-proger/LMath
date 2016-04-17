#ifndef lm_matrix_intrin_h__
#define lm_matrix_intrin_h__

#include "lm_matrix.h"

namespace lm {
	template<typename T1, typename T2, typename = std::enable_if<lm::matrix_traits::can_multiply<T1, T2>::value>::type>
	auto mul(T1 l, T2 r) {
		Matrix<matrix_traits::field_mul_t<T1, T2>, T1::rows_count, T2::columns_count> result;

		for (size_t y = 0; y < T1::rows_count; ++y) {
			for (size_t x = 0; x < T2::columns_count; ++x) {
				decltype(result)::element_type e = (decltype(result)::element_type)0;

				for (size_t i = 0; i < T2::rows_count; ++i) {
					e += l.data_2d[y][i] * r.data_2d[i][x];
				}

				result.data_2d[y][x] = e;
			}
		}
		return result;
	}
}
#endif // lm_matrix_intrin_h__
