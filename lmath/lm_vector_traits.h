#ifndef lm_vector_traits_h__
#define lm_vector_traits_h__

#include <type_traits>
namespace vector_traits {

	template<typename T, bool IsVector = std::is_base_of<Vector_base, T>::value>
	struct is_vector {

	};

	template<typename T>
	struct is_vector<T, true> {
		static constexpr bool value = true;
	};

	template<typename T>
	struct is_vector<T, false> {
		static constexpr bool value = false;
	};


	template<typename T, bool IsVector = is_vector<T>::value>
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


	template<typename T1, typename T2, bool same = is_vector<T1>::value &&  is_vector<T2>::value && (T1::size == T2::size)>
	struct is_same_extent {

	};

	template<typename T1, typename T2>
	struct is_same_extent<T1, T2, true> {
		static constexpr bool value = true;
	};

	template<typename T1, typename T2>
	struct is_same_extent<T1, T2, false> {
		static constexpr bool value = false;
	};
}
#endif // lm_vector_traits_h__
