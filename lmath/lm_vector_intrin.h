#ifndef lm_vector_intrin_h__
#define lm_vector_intrin_h__

#include <cmath>
#include "lm_vector.h"

namespace lm {
	template<typename T, typename TResult = T::element_type>
	auto length_sq(const T& v) {
		auto result = static_cast<TResult>(0);
		for (size_t i = 0; i < T::size; ++i) {
			result += v.data[i] * v.data[i];
		}
		return result;
	}

	template<typename T>
	auto length(const T& v) {
		return std::sqrt(lm::length_sq(v));
	}

	template<typename T>
	auto normalize(const T& v) {
		return v / lm::length(v);
	}

	template<typename T1, typename T2, typename = std::enable_if<vector_traits::is_same_extent<T1, T2>::value>::type>
	auto distance(const T1& v1, const T2& v2) {
		auto result = (decltype(v1.data[0] * v2.data[0]))0;
		for (size_t i = 0; i < T1::size; ++i) {
			auto val = v1.data[i] - v2.data[i];
			result += val * val;
		}
		return std::sqrt(result);
	}

	template<typename T1, typename T2, typename = std::enable_if<vector_traits::is_same_extent<T1, T2>::value>::type>
	auto dot(const T1& v1, const T2& v2) {
		auto result = (decltype(v1.data[0] * v2.data[0]))0;
		for (size_t i = 0; i < T1::size; ++i) {
			result += v1.data[i] * v2.data[i];
		}
		return result;
	}

	template<typename T, typename OpUnary>
	void transform(T& v, OpUnary op) {
		for (size_t i = 0; i < T::size; ++i) {
			v.data[i] = op(v.data[i]);
		}
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
