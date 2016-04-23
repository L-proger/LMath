#ifndef lm_matrix_intrin_h__
#define lm_matrix_intrin_h__

#include "lm_matrix.h"

namespace lm {
	template<typename T1, typename T2, typename = std::enable_if<lm::matrix_traits::can_multiply<T1, T2>::value>::type>
	auto mul(T1 l, T2 r) RESTRICT(cpu, amp) {
		Matrix<decltype(l.data_2d[0][0] * r.data_2d[0][0]), T1::rows_count, T2::columns_count> result;

		for (lm_size_type y = 0; y < T1::rows_count; ++y) {
			for (lm_size_type x = 0; x < T2::columns_count; ++x) {
				decltype(result)::element_type e = (decltype(result)::element_type)0;

				for (lm_size_type i = 0; i < T2::rows_count; ++i) {
					e += l.data_2d[y][i] * r.data_2d[i][x];
				}

				result.data_2d[y][x] = e;
			}
		}
		return result;
	}

	template<typename T, typename = std::enable_if<matrix_traits::is_square<T>::value && (T::rows_count == 3)>::type>
	auto determinant(const T& src) RESTRICT(cpu, amp) {
		return  (src.data_2d[0][0] * src.data_2d[1][1] * src.data_2d[2][2] +
			src.data_2d[1][0] * src.data_2d[2][1] * src.data_2d[0][2] +
			src.data_2d[0][1] * src.data_2d[1][2] * src.data_2d[2][0]) -
			(src.data_2d[0][2] * src.data_2d[1][1] * src.data_2d[2][0] +
				src.data_2d[0][1] * src.data_2d[1][0] * src.data_2d[2][2] +
				src.data_2d[1][2] * src.data_2d[2][1] * src.data_2d[0][0]);
	}

	template<typename T, typename = void, typename = std::enable_if<matrix_traits::is_square<T>::value && (T::rows_count == 2)>::type>
	auto determinant(const T& src) RESTRICT(cpu, amp) {
		return src.data_2d[0][0] * src.data_2d[1][1] - src.data_2d[0][1] * src.data_2d[1][0];
	}
}
#endif // lm_matrix_intrin_h__
