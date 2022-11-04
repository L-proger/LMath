#ifndef lm_matrix_h__
#define lm_matrix_h__

#include "lm_vector.h"
#include "lm_types.h"
#include <array>
#include "lm_matrix_traits.h"
#include <iostream>

namespace lm {
	template<typename T, LmSize M, LmSize N>
	struct Matrix : public Vector<Vector<T, N>, M> {
		typedef Vector<T, N> Row;
		typedef Vector<Vector<T, N>, M> Base;

		template<typename ... Args, typename = std::enable_if_t<sizeof...(Args) <= M>>
		Matrix(const Args& ... rest)  RESTRICT(cpu) : Base{ rest... } {}

		constexpr Matrix() RESTRICT(cpu, amp) {}

		Matrix(const Matrix& m)  RESTRICT(cpu, amp) : Base{ (const Base&)m } {}

		Matrix(const Vector<Vector<T, N>, M>& m)  RESTRICT(cpu, amp) : Base{ m } {}

		auto getColumn(LmSize id) const RESTRICT(cpu, amp) {
			Vector<T, M> result;
			for (LmSize i = 0; i < M; ++i) {
				result[i] = this->get(i)[id];
			}
			return result;
		}


		static Matrix identity() RESTRICT(cpu, amp) {
			auto zero = DefaultValues<T>::zero();
			Matrix result(zero);
			for (LmSize y = 0; y < M; ++y) {
				result[y][y] = static_cast<T>(1);
			}
			return result;
		}

		template<typename T2, LmSize M2, LmSize N2, typename = typename std::enable_if_t<(M2 <= M) && (N2 <= N)>>
		operator Matrix<T2, M2, N2>() const RESTRICT(cpu, amp) {
			Matrix<T2, M2, N2> result;
			for (LmSize y = 0; y < M2; ++y){
				for (LmSize x = 0; x < N2; ++x) {
					result[y][x] = this->data[y][x];
				}
			}
			return result;
		}
	};

	template<typename T1, typename T2, LmSize M, LmSize N, LmSize N2>
	auto mul(const Matrix<T1, M, N>& left, const Matrix<T2, N, N2>& right) RESTRICT(cpu, amp) {
		auto result = Matrix<MultiplyType<T1, T2>, M, N2>{};
		for (LmSize y = 0; y < M; ++y) {
			for (LmSize x = 0; x < N2; ++x) {

				result[y][x] = DefaultValues<std::remove_reference_t<decltype(result[y][x])>>::zero();

				for (LmSize i = 0; i < N; ++i) {
					result[y][x] += left[y][i] * right[i][x];
				}
			}
		}
		return result;
	}

	template<typename TM, typename TV, LmSize M, LmSize N>
	auto mul(const Matrix<TM, M, N>& left, const Vector<TV, N>& right) RESTRICT(cpu, amp) {
		auto result = Vector<MultiplyType<TM, TV>, N>{};
		for (LmSize y = 0; y < M; ++y) {
			result[y] = dot(left[y], right);
		}
		return result;
	}

	template<typename T, LmSize M, LmSize N, typename = std::enable_if_t<(M == N) && (M == 2)>>
	auto determinant(const Matrix<T, M, N>& m) RESTRICT(cpu, amp) {
		return m[0][0] * m[1][1] - m[0][1] * m[1][0];
	}


	template<typename T, LmSize M, LmSize N, typename = std::enable_if_t<(M == N) && ((M == 3) || (M == 4))>>
	auto determinantAffine(const Matrix<T, M, N>& m) RESTRICT(cpu, amp) {
		return
			(m[0][0] * m[1][1] * m[2][2] + m[1][0] * m[2][1] * m[0][2] + m[0][1] * m[1][2] * m[2][0]) -
			(m[0][2] * m[1][1] * m[2][0] + m[0][1] * m[1][0] * m[2][2] + m[1][2] * m[2][1] * m[0][0]);
	}

	template<typename T, LmSize M, LmSize N>
	Matrix<T, N, M> transpose(const Matrix<T, M, N>& v) RESTRICT(cpu, amp) {
		Matrix<T, N, M> result;

		for (LmSize y = 0; y < M; ++y) {
			for (LmSize x = 0; x < N; ++x) {
				result[x][y] = v[y][x];
			}
		}

		return result;
	}
	

	template<typename T>
	Matrix<T, 4, 4> inverse(const Matrix<T, 4, 4>& u, bool affine) RESTRICT(cpu, amp) {
		Matrix<T, 4, 4> result;
		if (affine) {
			T s = static_cast<T>(1) / determinantAffine(u);
			result[0][0] = (u[1][1] * u[2][2] - u[1][2] * u[2][1]) * s;
			result[0][1] = (u[2][1] * u[0][2] - u[2][2] * u[0][1]) * s;
			result[0][2] = (u[0][1] * u[1][2] - u[0][2] * u[1][1]) * s;
			result[0][3] = u[0][3];
			result[1][0] = (u[1][2] * u[2][0] - u[1][0] * u[2][2]) * s;
			result[1][1] = (u[2][2] * u[0][0] - u[2][0] * u[0][2]) * s;
			result[1][2] = (u[0][2] * u[1][0] - u[0][0] * u[1][2]) * s;
			result[1][3] = u[1][3];
			result[2][0] = (u[1][0] * u[2][1] - u[1][1] * u[2][0]) * s;
			result[2][1] = (u[2][0] * u[0][1] - u[2][1] * u[0][0]) * s;
			result[2][2] = (u[0][0] * u[1][1] - u[0][1] * u[1][0]) * s;
			result[2][3] = u[2][3];
			result[3][0] = -(result[0][0] * u[3][0] + result[1][0] * u[3][1] + result[2][0] * u[3][2]);
			result[3][1] = -(result[0][1] * u[3][0] + result[1][1] * u[3][1] + result[2][1] * u[3][2]);
			result[3][2] = -(result[0][2] * u[3][0] + result[1][2] * u[3][1] + result[2][2] * u[3][2]);
			result[3][3] = u[3][3];
		}
		else {

			// transpose matrix
			T src[16];
			for (size_t i = 0; i < 4; ++i) {
				src[i] = u[i][0];
				src[i + 4] = u[i][1];
				src[i + 8] = u[i][2];
				src[i + 12] = u[i][3];
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
			result[0][0] = (tmp[0] * src[5] + tmp[3] * src[6] + tmp[4] * src[7]) - (tmp[1] * src[5] + tmp[2] * src[6] + tmp[5] * src[7]);
			result[0][1] = (tmp[1] * src[4] + tmp[6] * src[6] + tmp[9] * src[7]) - (tmp[0] * src[4] + tmp[7] * src[6] + tmp[8] * src[7]);
			result[0][2] = (tmp[2] * src[4] + tmp[7] * src[5] + tmp[10] * src[7]) - (tmp[3] * src[4] + tmp[6] * src[5] + tmp[11] * src[7]);
			result[0][3] = (tmp[5] * src[4] + tmp[8] * src[5] + tmp[11] * src[6]) - (tmp[4] * src[4] + tmp[9] * src[5] + tmp[10] * src[6]);
			result[1][0] = (tmp[1] * src[1] + tmp[2] * src[2] + tmp[5] * src[3]) - (tmp[0] * src[1] + tmp[3] * src[2] + tmp[4] * src[3]);
			result[1][1] = (tmp[0] * src[0] + tmp[7] * src[2] + tmp[8] * src[3]) - (tmp[1] * src[0] + tmp[6] * src[2] + tmp[9] * src[3]);
			result[1][2] = (tmp[3] * src[0] + tmp[6] * src[1] + tmp[11] * src[3]) - (tmp[2] * src[0] + tmp[7] * src[1] + tmp[10] * src[3]);
			result[1][3] = (tmp[4] * src[0] + tmp[9] * src[1] + tmp[10] * src[2]) - (tmp[5] * src[0] + tmp[8] * src[1] + tmp[11] * src[2]);

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
			result[2][0] = (tmp[0] * src[13] + tmp[3] * src[14] + tmp[4] * src[15]) - (tmp[1] * src[13] + tmp[2] * src[14] + tmp[5] * src[15]);
			result[2][1] = (tmp[1] * src[12] + tmp[6] * src[14] + tmp[9] * src[15]) - (tmp[0] * src[12] + tmp[7] * src[14] + tmp[8] * src[15]);
			result[2][2] = (tmp[2] * src[12] + tmp[7] * src[13] + tmp[10] * src[15]) - (tmp[3] * src[12] + tmp[6] * src[13] + tmp[11] * src[15]);
			result[2][3] = (tmp[5] * src[12] + tmp[8] * src[13] + tmp[11] * src[14]) - (tmp[4] * src[12] + tmp[9] * src[13] + tmp[10] * src[14]);
			result[3][0] = (tmp[2] * src[10] + tmp[5] * src[11] + tmp[1] * src[9]) - (tmp[4] * src[11] + tmp[0] * src[9] + tmp[3] * src[10]);
			result[3][1] = (tmp[8] * src[11] + tmp[0] * src[8] + tmp[7] * src[10]) - (tmp[6] * src[10] + tmp[9] * src[11] + tmp[1] * src[8]);
			result[3][2] = (tmp[6] * src[9] + tmp[11] * src[11] + tmp[3] * src[8]) - (tmp[10] * src[11] + tmp[2] * src[8] + tmp[7] * src[9]);
			result[3][3] = (tmp[10] * src[10] + tmp[4] * src[8] + tmp[9] * src[9]) - (tmp[8] * src[9] + tmp[11] * src[10] + tmp[5] * src[8]);

			// calculate determinant
			T det = src[0] * result[0][0] + src[1] * result[0][1] + src[2] * result[0][2] + src[3] * result[0][3];

			// calculate matrix inverse
			det = static_cast<T>(1) / det;

			for (LmSize i = 0; i < 4; ++i) {
				for (LmSize j = 0; j < 4; ++j) {
					result[i][j] *= det;
				}
			}
		}
		
		return result;
	}
	
	typedef Matrix<float, 2, 2> float2x2;
	typedef Matrix<float, 2, 3> float2x3;
	typedef Matrix<float, 3, 3> float3x3;
	typedef Matrix<float, 3, 4> float3x4;
	typedef Matrix<float, 4, 3> float4x3;
	typedef Matrix<float, 4, 4> float4x4;
	typedef Matrix<double, 2, 2> double2x2;
	typedef Matrix<double, 2, 3> double2x3;
	typedef Matrix<double, 3, 3> double3x3;
	typedef Matrix<double, 3, 4> double3x4;
	typedef Matrix<double, 4, 3> double4x3;
	typedef Matrix<double, 4, 4> double4x4;
}
#endif // lm_matrix_h__
