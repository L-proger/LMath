#ifndef lm_stdio_h__
#define lm_stdio_h__

#include <ostream>
#include "lm_vector.h"

namespace lm
{
	template<typename T, size_t Size>
	std::ostream& operator<<(std::ostream& os, const Vector<T, Size>& obj) {
		os << "[";
		for (size_t i = 0; i < Size; ++i) {
			os << obj.data[i];
			if(i != (Size - 1))
			{
				os << ", ";
			}
		}
		os << "]";
		return os;
	}
}


#endif // lm_stdio_h__
