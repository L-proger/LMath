#ifndef lm_matrix_intrin_h__
#define lm_matrix_intrin_h__

#include "lm_matrix.h"

namespace lm {

	template<typename T1, typename T2, lm_size_type Size>
	auto dot(const Vector<T1, Size>& left, const Vector<T2, Size>& right) {
		auto result = (MultiplyType<T1, T2>)0;
		for (lm_size_type i = 0; i < Size; ++i) {
			result += left[i] * right[i];
		}
		return result;
	}


	template<typename T1, typename T2, lm_size_type M, lm_size_type N, lm_size_type N2>
	auto mul(const Matrix<T1, M, N>& left, const Matrix<T2, N, N2>& right) {
		auto result = Matrix<MultiplyType<T1, T2>, M, N2>{};
		for (lm_size_type y = 0; y < M; ++y) {
			for (lm_size_type x = 0; x < N2; ++x) {
				for (lm_size_type i = 0; i < N; ++i) {
					result[y][x] += left[y][i] * right[i][x];
				}
			}
		}
		return result;
	}

	template<typename TM, typename TV, lm_size_type M, lm_size_type N>
	auto mul(const Matrix<TM, M, N>& left, const Vector<TV, N>& right) {
		auto result = Vector<MultiplyType<TM, TV>, N>{};
		for (lm_size_type y = 0; y < M; ++y) {
			result[y] = dot(left[y], right);
		}
		return result;
	}
	
	/*template<typename T, typename = typename std::enable_if<matrix_traits::is_square<T>::value && (T::rows_count == 3 || T::rows_count == 4)>::type>
	auto matrix_determinant_affine(const T& src) RESTRICT(cpu, amp) {
		return  (src.data_2d[0][0] * src.data_2d[1][1] * src.data_2d[2][2] +
			src.data_2d[1][0] * src.data_2d[2][1] * src.data_2d[0][2] +
			src.data_2d[0][1] * src.data_2d[1][2] * src.data_2d[2][0]) -
			(src.data_2d[0][2] * src.data_2d[1][1] * src.data_2d[2][0] +
				src.data_2d[0][1] * src.data_2d[1][0] * src.data_2d[2][2] +
				src.data_2d[1][2] * src.data_2d[2][1] * src.data_2d[0][0]);
	}

	template<typename T, typename = void, typename = typename std::enable_if<matrix_traits::is_square<T>::value && (T::rows_count == 2)>::type>
	auto determinant(const T& src) RESTRICT(cpu, amp) {
		return src.data_2d[0][0] * src.data_2d[1][1] - src.data_2d[0][1] * src.data_2d[1][0];
	}

	
	template<typename T>
	Matrix<T, 4, 4> matrix4x4_inverse(const Matrix<T, 4, 4>& u, bool affine = true) {
		Matrix<T, 4, 4> result;
		if (affine) {
			T s = static_cast<T>(1) / matrix_determinant_affine(u);
			result.data_2d[0][0] = (u.data_2d[1][1] * u.data_2d[2][2] - u.data_2d[1][2] * u.data_2d[2][1]) * s;
			result.data_2d[0][1] = (u.data_2d[2][1] * u.data_2d[0][2] - u.data_2d[2][2] * u.data_2d[0][1]) * s;
			result.data_2d[0][2] = (u.data_2d[0][1] * u.data_2d[1][2] - u.data_2d[0][2] * u.data_2d[1][1]) * s;
			result.data_2d[0][3] =  u.data_2d[0][3];
			result.data_2d[1][0] = (u.data_2d[1][2] * u.data_2d[2][0] - u.data_2d[1][0] * u.data_2d[2][2]) * s;
			result.data_2d[1][1] = (u.data_2d[2][2] * u.data_2d[0][0] - u.data_2d[2][0] * u.data_2d[0][2]) * s;
			result.data_2d[1][2] = (u.data_2d[0][2] * u.data_2d[1][0] - u.data_2d[0][0] * u.data_2d[1][2]) * s;
			result.data_2d[1][3] =  u.data_2d[1][3];
			result.data_2d[2][0] = (u.data_2d[1][0] * u.data_2d[2][1] - u.data_2d[1][1] * u.data_2d[2][0]) * s;
			result.data_2d[2][1] = (u.data_2d[2][0] * u.data_2d[0][1] - u.data_2d[2][1] * u.data_2d[0][0]) * s;
			result.data_2d[2][2] = (u.data_2d[0][0] * u.data_2d[1][1] - u.data_2d[0][1] * u.data_2d[1][0]) * s;
			result.data_2d[2][3] =  u.data_2d[2][3];
			result.data_2d[3][0] = -(result.data_2d[0][0] * u.data_2d[3][0] + result.data_2d[1][0] * u.data_2d[3][1] + result.data_2d[2][0] * u.data_2d[3][2]);
			result.data_2d[3][1] = -(result.data_2d[0][1] * u.data_2d[3][0] + result.data_2d[1][1] * u.data_2d[3][1] + result.data_2d[2][1] * u.data_2d[3][2]);
			result.data_2d[3][2] = -(result.data_2d[0][2] * u.data_2d[3][0] + result.data_2d[1][2] * u.data_2d[3][1] + result.data_2d[2][2] * u.data_2d[3][2]);
			result.data_2d[3][3] =  u.data_2d[3][3];
		} else {
			// transpose matrix
			T src[16];
			for (size_t i = 0; i < 4; ++i) {
				src[i] = u.data[i * 4];
				src[i + 4] = u.data[i * 4 + 1];
				src[i + 8] = u.data[i * 4 + 2];
				src[i + 12] = u.data[i * 4 + 3];
			}

			// calculate pairs for first 8 elements (cofactors)
			T tmp[12]; // temp array for pairs
			tmp[0] = src[10] * src[15];
			tmp[1] = src[11] * src[14];
			tmp[2] = src[9] * src[15];
			tmp[3] = src[11] * src[13];
			tmp[4] = src[9] * src[14];
			tmp[5] = src[10] * src[13];
			tmp[6] = src[8] * src[15];
			tmp[7] = src[11] * src[12];
			tmp[8] = src[8] * src[14];
			tmp[9] = src[10] * src[12];
			tmp[10] = src[8] * src[13];
			tmp[11] = src[9] * src[12];

			// calculate first 8 elements (cofactors)
			result.data[0] = (tmp[0] * src[5] + tmp[3] * src[6] + tmp[4] * src[7]) - (tmp[1] * src[5] + tmp[2] * src[6] + tmp[5] * src[7]);
			result.data[1] = (tmp[1] * src[4] + tmp[6] * src[6] + tmp[9] * src[7]) - (tmp[0] * src[4] + tmp[7] * src[6] + tmp[8] * src[7]);
			result.data[2] = (tmp[2] * src[4] + tmp[7] * src[5] + tmp[10] * src[7]) - (tmp[3] * src[4] + tmp[6] * src[5] + tmp[11] * src[7]);
			result.data[3] = (tmp[5] * src[4] + tmp[8] * src[5] + tmp[11] * src[6]) - (tmp[4] * src[4] + tmp[9] * src[5] + tmp[10] * src[6]);
			result.data[4] = (tmp[1] * src[1] + tmp[2] * src[2] + tmp[5] * src[3]) - (tmp[0] * src[1] + tmp[3] * src[2] + tmp[4] * src[3]);
			result.data[5] = (tmp[0] * src[0] + tmp[7] * src[2] + tmp[8] * src[3]) - (tmp[1] * src[0] + tmp[6] * src[2] + tmp[9] * src[3]);
			result.data[6] = (tmp[3] * src[0] + tmp[6] * src[1] + tmp[11] * src[3]) - (tmp[2] * src[0] + tmp[7] * src[1] + tmp[10] * src[3]);
			result.data[7] = (tmp[4] * src[0] + tmp[9] * src[1] + tmp[10] * src[2]) - (tmp[5] * src[0] + tmp[8] * src[1] + tmp[11] * src[2]);

			// calculate pairs for second 8 elements (cofactors)
			tmp[0] = src[2] * src[7];
			tmp[1] = src[3] * src[6];
			tmp[2] = src[1] * src[7];
			tmp[3] = src[3] * src[5];
			tmp[4] = src[1] * src[6];
			tmp[5] = src[2] * src[5];
			tmp[6] = src[0] * src[7];
			tmp[7] = src[3] * src[4];
			tmp[8] = src[0] * src[6];
			tmp[9] = src[2] * src[4];
			tmp[10] = src[0] * src[5];
			tmp[11] = src[1] * src[4];

			// calculate second 8 elements (cofactors)
			result.data[8] = (tmp[0] * src[13] + tmp[3] * src[14] + tmp[4] * src[15]) - (tmp[1] * src[13] + tmp[2] * src[14] + tmp[5] * src[15]);
			result.data[9] = (tmp[1] * src[12] + tmp[6] * src[14] + tmp[9] * src[15]) - (tmp[0] * src[12] + tmp[7] * src[14] + tmp[8] * src[15]);
			result.data[10] = (tmp[2] * src[12] + tmp[7] * src[13] + tmp[10] * src[15]) - (tmp[3] * src[12] + tmp[6] * src[13] + tmp[11] * src[15]);
			result.data[11] = (tmp[5] * src[12] + tmp[8] * src[13] + tmp[11] * src[14]) - (tmp[4] * src[12] + tmp[9] * src[13] + tmp[10] * src[14]);
			result.data[12] = (tmp[2] * src[10] + tmp[5] * src[11] + tmp[1] * src[9]) - (tmp[4] * src[11] + tmp[0] * src[9] + tmp[3] * src[10]);
			result.data[13] = (tmp[8] * src[11] + tmp[0] * src[8] + tmp[7] * src[10]) - (tmp[6] * src[10] + tmp[9] * src[11] + tmp[1] * src[8]);
			result.data[14] = (tmp[6] * src[9] + tmp[11] * src[11] + tmp[3] * src[8]) - (tmp[10] * src[11] + tmp[2] * src[8] + tmp[7] * src[9]);
			result.data[15] = (tmp[10] * src[10] + tmp[4] * src[8] + tmp[9] * src[9]) - (tmp[8] * src[9] + tmp[11] * src[10] + tmp[5] * src[8]);

			// calculate determinant
			T det = src[0] * result.data[0] + src[1] * result.data[1] + src[2] * result.data[2] + src[3] * result.data[3];

			// calculate matrix inverse
			det = 1 / det;
			for (int j = 0; j < 16; ++j)
				result.data[j] *= det;
		}
		return result;
	}*/
}
#endif // lm_matrix_intrin_h__
