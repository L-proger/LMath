#ifndef lm_vector_traits_h__
#define lm_vector_traits_h__

#include <type_traits>

namespace lm {
struct Vector_base {

};

namespace vector_traits {

	template<typename T>
	struct is_vector {
		static constexpr bool value = std::is_base_of<lm::Vector_base, T>::value;
	};


	template<typename T, bool IsVector = is_vector<T>::value>
	struct field_type {
		typedef typename T::element_type type;
	};

	template<typename T>
	struct field_type<T, false> {
		typedef T type;
	};

	template<typename T1, typename T2, bool IsVectors = (is_vector<T1>::value && is_vector<T2>::value)>
	struct is_same_extent {
		static constexpr bool value = T1::size == T2::size;
	};

	template<typename T1, typename T2>
	struct is_same_extent<T1, T2, false> {
		static constexpr bool value = false;
	};
}
}

#endif // lm_vector_traits_h__
