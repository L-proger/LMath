#ifndef lm_vector_intrin_h__
#define lm_vector_intrin_h__

#include <cmath>
#include <numeric>
#include "lm_vector.h"

namespace lm {

	template<typename T>
	inline auto length_sq(const T& v) RESTRICT(cpu, amp) {
		auto result = static_cast<typename T::element_type>(0);
		for (lm_size_type i = 0; i < T::size; ++i) {
			result += v.data[i] * v.data[i];
		}
		return result;
	}

	template<typename T>
	inline auto length(const T& v) RESTRICT(cpu) {
		return std::sqrt(lm::length_sq(v));
	}

#if defined(LM_AMP_SUPPORTED)
	template<typename T>
	inline auto length(const T& v) RESTRICT(amp) {
		return concurrency::precise_math::sqrt(lm::length_sq(v));
	}

	template<typename T>
	inline auto normalize(const T& v) RESTRICT(amp) {
		return v / lm::length(v);
	}

	template<typename T1, typename T2, typename = typename std::enable_if<vector_traits::is_same_extent<T1, T2>::value>::type>
	inline auto distance(const T1& v1, const T2& v2) RESTRICT(amp) {
		auto result = (decltype(v1.data[0] * v2.data[0]))0;
		for (lm_size_type i = 0; i < T1::size; ++i) {
			auto val = v1.data[i] - v2.data[i];
			result += val * val;
		}
		return concurrency::precise_math::sqrt(result);
	}
#endif

	template<typename T>
	inline auto normalize(const T& v) RESTRICT(cpu) {
		return v / lm::length(v);
	}

	template<typename T1, typename T2, typename = typename std::enable_if<vector_traits::is_same_extent<T1, T2>::value>::type>
	inline auto distance(const T1& v1, const T2& v2) RESTRICT(cpu) {
		auto result = (decltype(v1.data[0] * v2.data[0]))0;
		for (lm_size_type i = 0; i < T1::size; ++i) {
			auto val = v1.data[i] - v2.data[i];
			result += val * val;
		}
		return std::sqrt(result);
	}

	template<typename T1, typename T2, typename = typename std::enable_if<vector_traits::is_same_extent<T1, T2>::value>::type>
	inline auto dot(const T1& v1, const T2& v2) RESTRICT(amp, cpu) {
		//return std::inner_product(std::begin(v1.data), std::end(v1.data), std::begin(v2.data), static_cast<typename T1::element_type>(0)); Unchecked iterator warning.
		typename T1::element_type accumulator = (typename T1::element_type)0;
		for (lm_size_type i = 0; i < T1::size; ++i) {
			accumulator += v1.data[i] * v2.data[i];
		}
		return accumulator;
	}
	
	template<typename T>
	inline auto cross(const Vector<T, 3>& v1, const Vector<T, 3>& v2)RESTRICT(cpu, amp) {
		return Vector<T, 3>(
			(v1.y * v2.z) - (v1.z * v2.y),
			(v1.z * v2.x) - (v1.x * v2.z),
			(v1.x * v2.y) - (v1.y * v2.x));
	}
}

#endif // lm_vector_intrin_h__
