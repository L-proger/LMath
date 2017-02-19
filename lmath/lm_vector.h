#ifndef lm_vector_h__
#define lm_vector_h__

#include <type_traits>
#include <array>
#include "lm_types.h"
#include "lm_detail.h"

namespace lm {

	template<typename T, lm_size_type Size>
	struct Vector {
	public:
		static constexpr lm_size_type Size = Size;
		typedef T ElementType;

		std::array<T, Size> data;

		constexpr Vector() = default;

		template<typename ... Args, typename = std::enable_if_t<sizeof...(Args) <= Size>>
		constexpr Vector(const Args& ... rest) : data{ { rest... } } {}

		T& operator [] (size_t id) {
			return data[id];
		}
		const T& operator [] (size_t id) const {
			return data[id];
		}

	};

	
	typedef Vector<float, 2> float2;
	typedef Vector<float, 3> float3;
	typedef Vector<float, 4> float4;
	typedef Vector<double, 2> double2;
	typedef Vector<double, 3> double3;
	typedef Vector<double, 4> double4;
}
#endif // lm_vector_h__
