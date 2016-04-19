#ifndef lm_common_intrin_h__
#define lm_common_intrin_h__

#include "lm_vector.h"
#include "lm_vector_intrin.h"

#include "lm_matrix.h"
#include "lm_matrix_intrin.h"

namespace lm {
	namespace common_traits
	{
		template<typename T, bool Success = vector_traits::is_vector<T>::value || matrix_traits::is_matrix<T>::value>
		struct is_lm_type{	
			static constexpr bool value = true;
		};

		template<typename T>
		struct is_lm_type<T, false> {
			static constexpr bool value = false;
		};
	}

#define LM_PI 3.14159265358979

	inline double asdouble(uint32_t lowbits, uint32_t highbits) {
		double result;
		uint32_t* memory = reinterpret_cast<uint32_t*>(&result);
		memory[0] = lowbits;
		memory[1] = highbits;
		return result;
	}

	template<typename T, typename = std::enable_if<vector_traits::is_vector<T>::value && (T::size == 2) && std::is_same<typename T::element_type, uint32_t>::value>::type>
	auto asdouble(const T& lowbits, const T& highbits) {
		return Vector<double, 2>(asdouble(lowbits.x, highbits.x), asdouble(lowbits.y, highbits.y));
	}

	template<typename T, typename = std::enable_if<common_traits::is_lm_type<T>::value>>
	auto ceil(const T& v){
		return transform_copy(v, [](auto x) {return std::ceil(x); });
	}

	template<typename T, typename = std::enable_if<common_traits::is_lm_type<T>::value>>
	auto floor(const T& v) {
		return transform_copy(v, [](auto x) {return std::floor(x); });
	}

	template<typename T, typename = std::enable_if<common_traits::is_lm_type<T>::value>>
	auto clamp(const T& v, typename T::element_type min_value, typename T::element_type max_value) {
		return transform_copy(v, [min_value, max_value](auto x) {return std::min(max_value, std::max(x, min_value)); });
	}

	template<typename T> auto pow(const T& v, typename T::element_type arg) {
		return transform_copy(v, [arg](T::element_type e) { return std::pow(e, arg); });
	}

	template<typename T> auto abs(const T& v) {
		return transform_copy(v, [](T::element_type e) { return std::abs(e); });
	}

	template<typename T> auto acos(const T& v) {
		return transform_copy(v, [](T::element_type e) { return std::acos(e); });
	}

	template<typename T> auto asin(const T& v) {
		return transform_copy(v, [](T::element_type e) { return std::asin(e); });
	}
	template<typename T> auto atan(const T& v) {
		return transform_copy(v, [](T::element_type e) { return std::atan(e); });
	}

	template<typename T> auto cos(const T& v) {
		return transform_copy(v, [](T::element_type e) { return std::cos(e); });
	}
	template<typename T> auto cosh(const T& v) {
		return transform_copy(v, [](T::element_type e) { return std::cosh(e); });
	}

	template<typename T> auto degrees(const T& v) {
		return transform_copy(v, [](T::element_type e) { return e * (T::element_type)180 / (T::element_type)LM_PI; });
	}

	//
	template<typename T> auto all(const T& v) {
		for (typename T::size_type i = 0; i < T::size; ++i) {
			if (v.data[i] == (typename T::element_type)(0)) {
				return false;
			}
		}
		return true;
	}

	template<typename T> auto any(const T& v) {
		for (typename T::size_type i = 0; i < T::size; ++i) {
			if (v.data[i] != (typename T::element_type)(0)) {
				return true;
			}
		}
		return false;
	}


}
#endif // lm_common_intrin_h__
