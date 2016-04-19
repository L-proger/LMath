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

	template<typename T, typename OpUnary>
	void transform(T& v, OpUnary op) {
		for (size_t i = 0; i < T::size; ++i) {
			v.data[i] = op(v.data[i]);
		}
	}

	template<typename T, typename OpUnary, typename TResult = std::remove_cv<T>::type>
	TResult transform_copy(T& v, OpUnary op) {
		TResult result;
		for (size_t i = 0; i < T::size; ++i) {
			result.data[i] = op(v.data[i]);
		}
		return result;
	}

	template<typename T>
	auto cross(const Vector<T, 3>& v1, const Vector<T, 3>& v2) {
		return Vector<T, 3>(
			(v1.y * v2.z) - (v1.z * v2.y),
			(v1.z * v2.x) - (v1.x * v2.z),
			(v1.x * v2.y) - (v1.y * v2.x));
	}




}

#endif // lm_vector_intrin_h__
