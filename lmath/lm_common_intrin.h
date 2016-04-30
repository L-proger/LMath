#ifndef lm_common_intrin_h__
#define lm_common_intrin_h__

#include "lm_vector.h"
#include "lm_vector_intrin.h"

#include "lm_matrix.h"
#include "lm_matrix_intrin.h"

#include "lm_common.h"
#include <algorithm>
#include <cmath>

namespace lm {

#define LM_PI 3.14159265358979

	inline double asdouble(uint32_t lowbits, uint32_t highbits) RESTRICT(cpu, amp) {
		double result;
		uint32_t* memory = reinterpret_cast<uint32_t*>(&result);
		memory[0] = lowbits;
		memory[1] = highbits;
		return result;
	}

	template<typename T, typename = std::enable_if<vector_traits::is_vector<T>::value && (T::size == 2) && std::is_same<typename T::element_type, uint32_t>::value>::type>
	auto asdouble(const T& lowbits, const T& highbits) RESTRICT(cpu, amp) {
		return Vector<double, 2>(asdouble(lowbits.x, highbits.x), asdouble(lowbits.y, highbits.y));
	}

	template<typename T, typename = std::enable_if<common_traits::is_lm_type<T>::value>>
	auto ceil(const T& v)RESTRICT(cpu) {
		return transform_copy_helper<T>::execute(v, [](auto x) {return (std::ceil)(x); });
	}
	template<typename T, typename = std::enable_if<common_traits::is_lm_type<T>::value>>
	auto ceil(const T& v)RESTRICT(amp) {
		return transform_copy_helper<T>::execute(v, [](auto x) {return concurrency::precise_math::ceil(x); });
	}


	template<typename T, typename = std::enable_if<common_traits::is_lm_type<T>::value>>
	auto floor(const T& v) RESTRICT(cpu) {
		return transform_copy_helper<T>::execute(v, [](auto x) {return std::floor(x); });
	}
	template<typename T, typename = std::enable_if<common_traits::is_lm_type<T>::value>>
	auto floor(const T& v) RESTRICT(amp) {
		return transform_copy_helper<T>::execute(v, [](auto x) {return concurrency::precise_math::floor(x); });
	}



	template<typename T, typename = std::enable_if<common_traits::is_lm_type<T>::value>>
	auto clamp(const T& v, typename T::element_type min_value, typename T::element_type max_value) RESTRICT(cpu) {
		return transform_copy_helper<T>::execute(v, [min_value, max_value](auto x) {return (std::min)(max_value, (std::max)(x, min_value)); });
	}
	
	template<typename T> auto pow(const T& v, typename T::element_type arg) RESTRICT(cpu) {
		return transform_copy_helper<T>::execute(v, [arg](T::element_type e)RESTRICT(cpu) { return std::pow(e, arg); });
	}

	template<typename T> auto abs(const T& v) RESTRICT(cpu) {
		return transform_copy_helper<T>::execute(v, [](T::element_type e) { return std::abs(e); });
	}

	template<typename T> auto acos(const T& v) RESTRICT(cpu) {
		return transform_copy_helper<T>::execute(v, [](T::element_type e) { return std::acos(e); });
	}
	
	template<typename T> auto asin(const T& v)RESTRICT(cpu) {
		return transform_copy_helper<T>::execute(v, [](T::element_type e) { return std::asin(e); });
	}

	template<typename T> auto atan(const T& v) RESTRICT(cpu) {
		return transform_copy_helper<T>::execute(v, [](T::element_type e) { return std::atan(e); });
	}
	
	template<typename T> auto cos(const T& v) RESTRICT(cpu) {
		return transform_copy_helper<T>::execute(v, [](common_traits::field_type<T>::type e) { return std::cos(e); });
	}
	
	template<typename T> auto cosh(T& v)RESTRICT(cpu) {
		return transform_copy_helper<T>::execute(v, [](typename common_traits::field_type<T>::type e) { return std::cosh(e); });
	}
	
	template<typename T> auto degrees(const T& v) RESTRICT(cpu, amp) {
		return transform_copy_helper<T>::execute(v, [](typename common_traits::field_type<T>::type e) { return e * (decltype(e))180 / (decltype(e))LM_PI; });
	}

	template<typename T> auto all(const T& v)RESTRICT(cpu, amp) {
		for (lm_size_type i = 0; i < T::size; ++i) {
			if (v.data[i] == (typename T::element_type)(0)) {
				return false;
			}
		}
		return true;
	}

	template<typename T> auto any(const T& v) RESTRICT(cpu, amp) {
		for (lm_size_type i = 0; i < T::size; ++i) {
			if (v.data[i] != (typename T::element_type)(0)) {
				return true;
			}
		}
		return false;
	}

#if defined(LM_AMP_SUPPORTED)
	template<typename T, typename = std::enable_if<common_traits::is_lm_type<T>::value>>
	auto clamp(const T& v, typename T::element_type min_value, typename T::element_type max_value) RESTRICT(amp) {
		return transform_copy_helper<T>::execute(v, [min_value, max_value](auto x) {return x > max_value ? max_value : (x < min_value ? min_value : x); });
	}
	template<typename T> auto asin(const T& v)RESTRICT(amp) {
		return transform_copy_helper<T>::execute(v, [](T::element_type e) { return concurrency::precise_math::asin(e); });
	}
	template<typename T> auto atan(const T& v) RESTRICT(amp) {
		return transform_copy_helper<T>::execute(v, [](T::element_type e) { return concurrency::precise_math::atan(e); });
	}
	template<typename T> auto cos(const T& v) RESTRICT(amp) {
		return transform_copy_helper<T>::execute(v, [](common_traits::field_type<T>::type e) { return concurrency::precise_math::cos(e); });
	}
	template<typename T> auto cosh(T& v)RESTRICT(amp) {
		return transform_copy_helper<T>::execute(v, [](typename common_traits::field_type<T>::type e) { return concurrency::precise_math::cosh(e); });
	}
	template<typename T> auto pow(const T& v, typename T::element_type arg) RESTRICT(amp) {
		return transform_copy_helper<T>::execute(v, [arg](T::element_type e)RESTRICT(amp) { return concurrency::precise_math::pow(e, arg); });
	}
	template<typename T> auto abs(const T& v) RESTRICT(amp) {
		return transform_copy_helper<T>::execute(v, [](T::element_type e) { return concurrency::precise_math::fabs(e); });
	}
	template<typename T> auto acos(const T& v) RESTRICT(amp) {
		return transform_copy_helper<T>::execute(v, [](T::element_type e) { return concurrency::precise_math::acos(e); });
	}
#endif
}
#endif // lm_common_intrin_h__
