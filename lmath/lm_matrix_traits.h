#ifndef lm_matrix_traits_h__
#define lm_matrix_traits_h__

#include "lm_vector_traits.h"
#include "lm_types.h"

namespace lm{
	

	template<typename T>
	struct MatrixSize {
		static constexpr size_t rows = VectorSize<T>::value;
		static constexpr size_t columns = VectorSize<T::ElementType>::value;
		static constexpr auto value = lm::Vector<lm::LmSize, 2>(rows, columns);
	};

	template<typename T>
	struct MatrixRowType {
		typedef typename T::ElementType type;
	};

	template<typename T, bool IsVector>
	struct MatrixColumnType_ {
		typedef Vector<typename T::ElementType, T::Size> type;
	};

	template<typename T>
	struct MatrixColumnType_<T, true> {
		typedef Vector<typename T::ElementType::ElementType, T::Size> type;
	};

	template<typename T>
	struct MatrixColumnType : MatrixColumnType_<T, IsVector<typename T::ElementType>::value> {
	};


	template<typename T>
	struct IsSquare {
		static constexpr bool Value = MatrixSize<T>::rows == MatrixSize<T>::columns;
	};

	template<typename Left, typename Right>
	struct CanMultiplyMatrix {
		static constexpr bool Value = MatrixSize<Left>::columns == MatrixSize<Right>::rows;
	};



	template<typename Left, typename Right>
	using DotResultType = MultiplyType<decltype(std::declval<Left>()[0]), decltype(std::declval<Right>()[0])>;

}

#endif // lm_matrix_traits_h__
