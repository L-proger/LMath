#ifndef lm_matrix_h__
#define lm_matrix_h__

#include "lm_vector.h"
#include "lm_types.h"
#include <array>
#include "lm_matrix_traits.h"

namespace lm {
//row-major matrix
	
	/*template<typename T, lm_size_type RowsCount, lm_size_type ColumnsCount>
	struct Matrix_data {
	
	

		Matrix& operator=(const Matrix& value) RESTRICT(cpu, amp) {
			for (lm_size_type i = 0; i < RowsCount * ColumnsCount; ++i) {
				this->data[i] = value.data[i];
			}
			return *this;
		}

#if defined(LM_AMP_SUPPORTED)
		template<typename = std::enable_if<rows_count == 1>::type>
		Matrix(row_type r0) RESTRICT(amp) : Matrix_data{ r0 } {}

		template<typename = std::enable_if<rows_count == 2>::type>
		Matrix(row_type r0, row_type r1) RESTRICT(amp) : Matrix_data{ r0,r1 } {}

		template<typename = std::enable_if<rows_count == 3>::type>
		Matrix(row_type r0, row_type r1, row_type r2) RESTRICT(amp) : Matrix_data{ r0,r1,r2 } {}

		template<typename = std::enable_if<rows_count == 4>::type>
		Matrix(row_type r0, row_type r1, row_type r2, row_type r3) RESTRICT(amp) : Matrix_data{ r0,r1,r2,r3 } {}

		template<typename U = Matrix>
		static U identity(typename std::enable_if<(U::rows_count == 4) && (U::columns_count == 4)>::type* = 0) RESTRICT(amp) {
			return U(row_type(1, 0, 0, 0), row_type(0, 1, 0, 0), row_type(0, 0, 1, 0), row_type(0, 0, 0, 1));
		}

		template<typename U = Matrix>
		static U identity(typename std::enable_if<(U::rows_count == 3) && (U::columns_count == 3)>::type* = 0) RESTRICT(amp) {
			return U(row_type(1, 0, 0), row_type(0, 1, 0), row_type(0, 0, 1));
		}

		template<typename U = Matrix>
		static U identity(typename std::enable_if<(U::rows_count == 2) && (U::columns_count == 2)>::type* = 0) RESTRICT(amp) {
			return U(row_type(1, 0), row_type(0, 1));
		}
#endif

		Matrix() RESTRICT(amp, cpu) {}

		Matrix(typename data_t::element_type value) RESTRICT(amp, cpu) : data_t(value){}

		template<typename U = Matrix>
		static constexpr U identity(typename std::enable_if<(U::rows_count == 4) && (U::columns_count == 4)>::type* = 0) RESTRICT(cpu) {
			return U(typename data_t::row_type(1, 0, 0, 0), typename data_t::row_type(0, 1, 0, 0), typename data_t::row_type(0, 0, 1, 0), typename data_t::row_type(0, 0, 0, 1));
		}

		template<typename U = Matrix>
		static constexpr U identity(typename std::enable_if<(U::rows_count == 3) && (U::columns_count == 3)>::type* = 0) RESTRICT(cpu) {
			return U(typename data_t::row_type(1, 0, 0), typename data_t::row_type(0, 1, 0), typename data_t::row_type(0, 0, 1));
		}

		template<typename U = Matrix>
		static constexpr U identity(typename std::enable_if<(U::rows_count == 2) && (U::columns_count == 2)>::type* = 0)RESTRICT(cpu) {
			return U(typename data_t::row_type(1, 0), typename data_t::row_type(0, 1));
		}

		template<typename U = Matrix>
		static U identity(typename std::enable_if<(matrix_traits::is_square<U>::value && ((U::rows_count < 2) || (U::rows_count > 4)))>::type* = 0) RESTRICT(cpu, amp){
			U result;
			for(lm_size_type y = 0; y < U::rows_count; ++y) {
				result.data_2d[y][y] = (typename U::element_type)1;
			}
			return result;
		}

		auto get_column(lm_size_type id) const RESTRICT(cpu, amp) {
			typename data_t::column_type result;
			for(lm_size_type y = 0; y < data_t::rows_count; ++y) {
				result.data[y] = this->data_2d[y][id];
			}
			return result;
		}

		template<lm_size_type ResultRowsCount, lm_size_type ResultColumnsCount, typename = typename std::enable_if<(data_t::rows_count >= ResultRowsCount) && (data_t::columns_count >= ResultColumnsCount)>::type>
		operator Matrix<T, ResultRowsCount, ResultColumnsCount>() const RESTRICT(cpu, amp) {
			Matrix<T, ResultRowsCount, ResultColumnsCount> result;
			for(lm_size_type y = 0; y < ResultRowsCount; ++y)
			{
				for (lm_size_type x = 0; x < ResultColumnsCount; ++x) {
					result.data_2d[y][x] = this->data_2d[y][x];
				}
			}
			return result;
		}
	};*/

	template<typename T, lm_size_type Rows, lm_size_type Columns>
	using Matrix = Vector<Vector<T, Columns>, Rows>;


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
