#ifndef lm_matrix_h__
#define lm_matrix_h__

#include "lm_vector.h"
#include <array>
//row-major matrix

namespace lm {
	struct Matrix_base {

	};
	template<typename T, size_t RowsCount, size_t ColumnsCount>
	struct Matrix_data {
		Matrix_data() {
			for(size_t i= 0; i < (RowsCount * ColumnsCount); ++i){
				data[i] = (T)0;
			}
		}
		
	    union
		{
			std::array<T, RowsCount * ColumnsCount> data;
			std::array<std::array<T, ColumnsCount>, RowsCount> data_2d;
			Vector<T, ColumnsCount> rows[RowsCount];
		};
	};

	template<typename T, size_t RowsCount, size_t ColumnsCount>
	struct Matrix : public Matrix_base, public Matrix_data<T, RowsCount, ColumnsCount> {
		static constexpr size_t rows_count = RowsCount;
		static constexpr size_t columns_count = ColumnsCount;
		typedef Vector<T, ColumnsCount> row_type;
		typedef Vector<T, RowsCount> column_type;

		Matrix()
		{
			
		}

		column_type get_column(size_t id){
			column_type result;
			for( size_t y = 0; y < rows_count; ++y)
			{
				result.data[y] = data_2d[y][id];
			}
			return result;
		}
	};
}
#endif // lm_matrix_h__
