#ifndef lm_matrix_traits_h__
#define lm_matrix_traits_h__

#include <type_traits>

namespace lm{
	namespace Detail {
		template<typename T>
		struct IsVector : public std::false_type {};

		template<typename T, lm::lm_size_type Size>
		struct IsVector<lm::Vector<T, Size>> : public std::true_type {};

		template<typename T, bool IsVector>
		struct VectorSize : std::integral_constant<lm::lm_size_type, 1> {};

		template<typename T>
		struct VectorSize<T, true> : std::integral_constant<lm::lm_size_type, T::Size> {};
	}

	template<typename T>
	struct IsVector : public Detail::IsVector<std::remove_cv_t<T>> {};

	template<typename T>
	struct VectorSize : Detail::VectorSize<T, IsVector<T>::value> {};

	template<typename T>
	struct MatrixSize {
		static constexpr size_t rows = VectorSize<T>::value;
		static constexpr size_t columns = VectorSize<T::ElementType>::value;
		static constexpr auto value = lm::Vector<lm::lm_size_type, 2>(rows, columns);
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

	template<typename A, typename B = A>
	using MultiplyType = decltype(std::declval<A>() * std::declval<B>());

	template<typename Left, typename Right>
	using DotResultType = MultiplyType<decltype(std::declval<Left>()[0]), decltype(std::declval<Right>()[0])>;

}

#endif // lm_matrix_traits_h__
