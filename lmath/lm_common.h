#ifndef lm_common_h__
#define lm_common_h__

#include "lm_vector_traits.h"
#include "lm_matrix_traits.h"

namespace lm{
	namespace common_traits {
		template<typename T, bool Success = (lm::vector_traits::is_vector<T>::value || lm::matrix_traits::is_matrix<T>::value)>
		struct is_lm_type {
			static constexpr bool value = true;
		};

		template<typename T>
		struct is_lm_type<T, false> {
			static constexpr bool value = false;
		};

		template<typename T, bool Valid = common_traits::is_lm_type<T>::value>
		struct field_type {
			typedef typename T::element_type type;
		};

		template<typename T>
		struct field_type<T, false> {
			typedef T type;
		};
	}

	template<typename T, bool IsScalar = !common_traits::is_lm_type<T>::value>
	struct transform_copy_helper {
		template<typename OpUnary>
		static auto execute(const T& v, OpUnary op)RESTRICT(cpu) {
			typename std::remove_cv<T>::type result;
			for (lm_size_type i = 0; i < T::size; ++i) {
				result.data[i] = op(v.data[i]);
			}
			return result;
		}
#if defined(LM_AMP_SUPPORTED)
		template<typename OpUnary>
		static auto execute(const T& v, OpUnary op)RESTRICT(amp) {
			std::remove_cv<T>::type result;
			for (lm_size_type i = 0; i < T::size; ++i) {
				result.data[i] = op(v.data[i]);
			}
			return result;
		}
#endif
	};


	template<typename T>
	struct transform_copy_helper<T, true> {
		template<typename OpUnary>
		static auto execute(const T& v, OpUnary op) RESTRICT(cpu) {
			return op(v);
		}
#if defined(LM_AMP_SUPPORTED)
		template<typename OpUnary>
		static auto execute(const T& v, OpUnary op) RESTRICT(amp) {
			return op(v);
		}
#endif
	};
}

#endif // lm_common_h__
