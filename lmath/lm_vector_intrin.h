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
	void transform_vector(T& v, OpUnary op) {
		for (size_t i = 0; i < T::size; ++i) {
			v.data[i] = op(v.data[i]);
		}
	}

	template<typename T, typename OpUnary, typename TResult = std::remove_cv<T>::type>
	TResult transform_vector_copy(T& v, OpUnary op) {
		TResult result;
		for (size_t i = 0; i < T::size; ++i) {
			result.data[i] = op(v.data[i]);
		}
		return result;
	}


	template<typename T> auto pow(const T& v, typename T::element_type arg) {
		return transform_vector_copy(v, [arg](T::element_type e) { return std::pow(e, arg); });
	}

	template<typename T> auto abs(const T& v) {
		return transform_vector_copy(v, [](T::element_type e) { return std::abs(e); });
	}

	template<typename T> auto acos(const T& v) {
		return transform_vector_copy(v, [](T::element_type e) { return std::acos(e); });
	}

	template<typename T> auto asin(const T& v) {
		return transform_vector_copy(v, [](T::element_type e) { return std::asin(e); });
	}
	template<typename T> auto atan(const T& v) {
		return transform_vector_copy(v, [](T::element_type e) { return std::atan(e); });
	}
}

#endif // lm_vector_intrin_h__
