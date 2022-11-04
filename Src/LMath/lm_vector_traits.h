#ifndef lm_vector_traits_h__
#define lm_vector_traits_h__

#include "lm_traits.h"
#include "lm_types.h"

namespace lm {
	namespace Detail {
		template<typename T>
		struct IsVector : public std::false_type {};

		template<typename T, lm::LmSize N>
		struct IsVector<lm::Vector<T, N>> : public std::true_type {};

		template<typename T, bool IsVector>
		struct VectorSize : std::integral_constant<lm::LmSize, 1> {};

		template<typename T>
		struct VectorSize<T, true> : std::integral_constant<lm::LmSize, T::Size> {};
	}

	template<typename T>
	struct IsVector : public Detail::IsVector<std::remove_cv_t<T>> {};

	template<typename T>
	struct VectorSize : Detail::VectorSize<T, IsVector<T>::value> {};

}

#endif // lm_vector_traits_h__
