#ifndef lm_matrix_h__
#define lm_matrix_h__

#include "lm_vector.h"
#include "lm_types.h"
#include <array>

//row-major matrix

namespace lm {
	struct Matrix_base {

	};

	template<typename T, lm_size_type RowsCount, lm_size_type ColumnsCount>
	struct Matrix_data {
		static constexpr lm_size_type rows_count = RowsCount;
		static constexpr lm_size_type columns_count = ColumnsCount;
		static constexpr lm_size_type size = rows_count * columns_count;
		typedef Vector<T, ColumnsCount> row_type;
		typedef Vector<T, RowsCount> column_type;
		typedef T element_type;

		Matrix_data() RESTRICT(cpu, amp){
			for(lm_size_type i= 0; i < (RowsCount * ColumnsCount); ++i){
				data[i] = (T)0;
			}
		}

		Matrix_data(element_type value) RESTRICT(cpu, amp) {
			for (lm_size_type i = 0; i < (RowsCount * ColumnsCount); ++i) {
				data[i] = value;
			}
		}

		template<typename = typename std::enable_if<rows_count == 1>::type>
		constexpr Matrix_data(row_type r0) RESTRICT(cpu) : rows{ r0} {}

		template<typename = typename std::enable_if<rows_count == 2>::type>
		constexpr Matrix_data(row_type r0, row_type r1) RESTRICT(cpu) : rows{ r0,r1} {}

		template<typename = typename std::enable_if<rows_count == 3>::type>
		constexpr Matrix_data(row_type r0, row_type r1, row_type r2) RESTRICT(cpu) : rows{ r0,r1,r2} {}

		template<typename = typename std::enable_if<rows_count == 4>::type>
		constexpr Matrix_data(row_type r0, row_type r1, row_type r2, row_type r3) RESTRICT(cpu) : rows{ r0,r1,r2,r3 } {}

#if defined(LM_AMP_SUPPORTED)
		template<typename = std::enable_if<rows_count == 1>::type>
		Matrix_data(row_type r0) RESTRICT(amp) : rows{ r0 } {}

		template<typename = std::enable_if<rows_count == 2>::type>
		Matrix_data(row_type r0, row_type r1) RESTRICT(amp) : rows{ r0,r1 } {}

		template<typename = std::enable_if<rows_count == 3>::type>
		Matrix_data(row_type r0, row_type r1, row_type r2) RESTRICT(amp) : rows{ r0,r1,r2 } {}

		template<typename = std::enable_if<rows_count == 4>::type>
		Matrix_data(row_type r0, row_type r1, row_type r2, row_type r3) RESTRICT(amp) : rows{ r0,r1,r2,r3 } {}
#endif

	    union {
			T data[RowsCount * ColumnsCount];
			T data_2d[RowsCount][ColumnsCount];
			Vector<T, ColumnsCount> rows[RowsCount];
		};
	};

#include "lm_matrix_traits.h"

	template<typename T, lm_size_type RowsCount, lm_size_type ColumnsCount>
	struct Matrix : public Matrix_base, public Matrix_data<T, RowsCount, ColumnsCount> {

		template<typename = std::enable_if<rows_count == 1>::type>
		constexpr Matrix(row_type r0) RESTRICT(cpu) : Matrix_data{ r0 } {}

		template<typename = std::enable_if<rows_count == 2>::type>
		constexpr Matrix(row_type r0, row_type r1) RESTRICT(cpu) : Matrix_data{ r0,r1} {}

		template<typename = std::enable_if<rows_count == 3>::type>
		constexpr Matrix(row_type r0, row_type r1, row_type r2) RESTRICT(cpu) : Matrix_data{ r0,r1,r2 } {}

		template<typename = std::enable_if<rows_count == 4>::type>
		constexpr Matrix(row_type r0, row_type r1, row_type r2, row_type r3) RESTRICT(cpu) : Matrix_data{ r0,r1,r2,r3 } {}

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

		Matrix(element_type value)  RESTRICT(amp, cpu) : Matrix_data(value){}

		template<typename U = Matrix>
		static constexpr U identity(typename std::enable_if<(U::rows_count == 4) && (U::columns_count == 4)>::type* = 0) RESTRICT(cpu) {
			return U(row_type(1, 0, 0, 0), row_type(0, 1, 0, 0), row_type(0, 0, 1, 0), row_type(0, 0, 0, 1));
		}

		template<typename U = Matrix>
		static constexpr U identity(typename std::enable_if<(U::rows_count == 3) && (U::columns_count == 3)>::type* = 0) RESTRICT(cpu) {
			return U(row_type(1, 0, 0), row_type(0, 1, 0), row_type(0, 0, 1));
		}

		template<typename U = Matrix>
		static constexpr U identity(typename std::enable_if<(U::rows_count == 2) && (U::columns_count == 2)>::type* = 0)RESTRICT(cpu) {
			return U(row_type(1, 0), row_type(0, 1));
		}





		template<typename U = Matrix>
		static U identity(typename std::enable_if<(matrix_traits::is_square<U>::value && ((U::rows_count < 2) || (U::rows_count > 4)))>::type* = 0) RESTRICT(cpu, amp){
			U result;
			for(lm_size_type y = 0; y < U::rows_count; ++y) {
				result.data_2d[y][y] = (typename U::element_type)1;
			}
			return result;
		}

		column_type get_column(lm_size_type id) const RESTRICT(cpu, amp) {
			column_type result;
			for(lm_size_type y = 0; y < rows_count; ++y) {
				result.data[y] = data_2d[y][id];
			}
			return result;
		}

		template<lm_size_type ResulRowsCount, lm_size_type ResulColumnsCount, typename = std::enable_if<(rows_count >= ResulRowsCount) && (columns_count >= ResulColumnsCount)>::type>
		operator Matrix<T, ResulRowsCount, ResulColumnsCount>() const RESTRICT(cpu, amp) {
			Matrix<T, ResulRowsCount, ResulColumnsCount> result;
			for(lm_size_type y = 0; y <ResulRowsCount; ++y)
			{
				for (lm_size_type x = 0; x < ResulColumnsCount; ++x) {
					result.data_2d[y][x] = data_2d[y][x];
				}
			}
			return result;
		}
	};

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
