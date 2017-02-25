#ifndef lm_types_h__
#define lm_types_h__

#if defined(_MSC_VER)
//#define LM_AMP_SUPPORTED
#endif


#if defined(LM_AMP_SUPPORTED)
#include <amp.h>
#include <amp_math.h>
#endif

#include <cstdint>

namespace lm {
#if defined(LM_AMP_SUPPORTED)
#define RESTRICT(...) restrict(__VA_ARGS__)
#define ENABLE_IF_AMP(...) __VA_ARGS__
#else
#define RESTRICT(...)
#define ENABLE_IF_AMP(...)
#endif

	template<typename T>
	struct DefaultValues {
		static T zero()  RESTRICT(cpu, amp) {
			return static_cast<T>(0);
		}
		static T one()  RESTRICT(cpu, amp) {
			return static_cast<T>(1);
		}
		static T two()  RESTRICT(cpu, amp) {
			return static_cast<T>(2);
		}
	};


	template<typename T>
	T zero = static_cast<T>(0);
	template<typename T>
	T one = static_cast<T>(1);
	template<typename T>
	T two = static_cast<T>(2);

	typedef size_t LmSize;
}

#endif // lm_types_h__
