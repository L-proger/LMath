#ifndef lm_stdio_h__
#define lm_stdio_h__

#include <ostream>
#include "lm_vector.h"

namespace lm
{
	template<typename T, size_t N>
	std::ostream& operator<<(std::ostream& os, const Vector<T, N>& obj) {
		os << "[";
		for (size_t i = 0; i < N; ++i) {
			os << obj.data[i];
			if(i != (N - 1))
			{
				os << ", ";
			}
		}
		os << "]";
		return os;
	}
}


#endif // lm_stdio_h__
