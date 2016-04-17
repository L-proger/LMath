#ifndef lm_matrix_traits_h__
#define lm_matrix_traits_h__

#include <type_traits>

namespace matrix_traits {

	template<typename T, bool IsMatrix = std::is_base_of<Matrix_base, T>::value>
	struct is_matrix {

	};

	template<typename T>
	struct is_matrix<T, true> {
		static constexpr bool value = true;
	};

	template<typename T>
	struct is_matrix<T, false> {
		static constexpr bool value = false;
	};


	template<typename T, bool IsMatrix = is_matrix<T>::value>
	struct field_type {

	};

	template<typename T>
	struct field_type<T, true> {
		typedef typename T::element_type type;
	};

	template<typename T>
	struct field_type<T, false> {
		typedef T type;
	};



	template<typename T1, typename T2, bool IsMatrices = (is_matrix<T1>::value && is_matrix<T2>::value)>
	struct can_multiply {

	};

	template<typename T1, typename T2>
	struct can_multiply<T1, T2, true> {
		static constexpr bool value = T1::columns_count == T2::rows_count;
	};

	template<typename T1, typename T2>
	struct can_multiply<T1, T2, false> {
		static constexpr bool value = false;
	};



	template<typename T, bool IsMatrix = is_matrix<T>::value>
	struct is_square {

	};

	template<typename T>
	struct is_square<T, true> {
		static constexpr bool value = T::rows_count == T::columns_count;
	};

	template<typename T>
	struct is_square<T, false> {
		static constexpr bool value = false;
	};


	template<typename T1, typename T2 = T1>
	using field_add_t = decltype(std::declval<field_type<T1>::type>() + std::declval<field_type<T2>::type>());

	template<typename T1, typename T2 = T1>
	using field_sub_t = decltype(std::declval<field_type<T1>::type>() - std::declval<field_type<T2>::type>());

	template<typename T1, typename T2 = T1>
	using field_mul_t = decltype(std::declval<field_type<T1>::type>() * std::declval<field_type<T2>::type>());

	template<typename T1, typename T2 = T1>
	using field_div_t = decltype(std::declval<field_type<T1>::type>() / std::declval<field_type<T2>::type>());

}
#endif // lm_matrix_traits_h__
