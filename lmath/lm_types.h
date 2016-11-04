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
#else
#define RESTRICT(...)
#endif

	typedef uint32_t lm_size_type;
}

#endif // lm_types_h__
