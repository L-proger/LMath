#ifndef lm_traits_h__
#define lm_traits_h__

#include <type_traits>

namespace lm {
	template<typename A, typename B = A>
	using MultiplyType = decltype(A() * B());

	template<typename A, typename B = A>
	using DivideType = decltype(A() / B());
}

#endif // lm_traits_h__
