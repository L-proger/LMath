#ifndef lm_vector_intrin_h__
#define lm_vector_intrin_h__

#include <cmath>
#include "lm_vector.h"

namespace lm {
	template<typename T, typename TResult = T::element_type>
	auto length_sq(const T& v) {
		auto result = static_cast<TResult>(0);
		for (int i = 0; i < T::size; ++i) {
			result += v.data[i] * v.data[i];
		}
		return result;
	}

	template<typename T, typename TResult = T::element_type>
	auto length(const T& v) {
		return std::sqrt(lm::length_sq(v));
	}


	/*template<typename T, typename TParam, typename TResult = T::element_type>
	auto pow(const T& v, TParam arg) {
		Vector<de 
	}*/
}

#endif // lm_vector_intrin_h__
