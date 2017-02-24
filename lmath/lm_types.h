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

	typedef size_t LmSize;
}

#endif // lm_types_h__
