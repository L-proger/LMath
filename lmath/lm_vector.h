#ifndef lm_vector_h__
#define lm_vector_h__

#include <type_traits>
#include <array>
#include "lm_types.h"

namespace lm {

#define VECTOR_ARITHMETIC_OP(_Op) \
	template<typename U> \
	auto operator _Op (const U& divider) const RESTRICT(cpu, amp){ \
		Vector<DivideType<T, U>, N> result; \
		for (LmSize i = 0; i < N; ++i) { \
			result[i] = data[i] _Op divider; \
		} \
		return result; \
	} \
	template<typename U> \
	auto operator _Op (Vector<U, N> divider) const RESTRICT(cpu, amp){ \
		Vector<DivideType<T, U>, N> result; \
		for (LmSize i = 0; i < N; ++i) { \
			result[i] = data[i] _Op divider[i]; \
		} \
		return result; \
	} \
	template<typename U>\
	auto operator _Op##= (const U& divider) RESTRICT(cpu, amp){ \
		for (LmSize i = 0; i < N; ++i) { \
			data[i] _Op##= divider; \
		} \
		return *this; \
	} \
	template<typename U> \
	auto operator _Op##= (Vector<U, N> divider) RESTRICT(cpu, amp){ \
		for (LmSize i = 0; i < N; ++i) { \
			data[i] _Op##= divider[i]; \
		} \
		return *this; \
	}

	template<typename T, LmSize N>
	struct Vector {
	public:
		static constexpr LmSize Size = N;
		typedef T ElementType;

		T data[Size];

		constexpr Vector() RESTRICT(cpu, amp) {}

		template<typename ... Args, typename = std::enable_if_t<sizeof...(Args) <= N>>
		constexpr Vector(const Args& ... rest)  RESTRICT(cpu, amp) : data{ rest... } {}

		template<typename Arg>
		Vector(Arg arg) RESTRICT(cpu, amp) {
			for (LmSize i = 0; i < N; ++i) {
				data[i] = arg;
			}
		}

		template<typename TR = MultiplyType<T>>
		auto lengthSquared() const RESTRICT(cpu, amp) {
			TR result{};
			for (LmSize i = 0; i < N; ++i) {
				result += data[i] * data[i];
			}
			return result;
		}

		auto length() const RESTRICT(cpu) {
			return std::sqrt(lengthSquared());
		}

#if defined(LM_AMP_SUPPORTED)
		auto length() const RESTRICT(amp) {
			return concurrency::precise_math::sqrt(lengthSquared());
		}
#endif

		auto normalized() const RESTRICT(cpu, amp) {
			return (*this) / length();
		}

		VECTOR_ARITHMETIC_OP(+);
		VECTOR_ARITHMETIC_OP(-);
		VECTOR_ARITHMETIC_OP(*);
		VECTOR_ARITHMETIC_OP(/);

		T& operator [] (size_t id) RESTRICT(cpu, amp) {
			return data[id];
		}
		const T& operator [] (size_t id) const RESTRICT(cpu, amp) {
			return data[id];
		}

	};

	template<typename T1, typename T2, LmSize N>
	auto distance(const Vector<T1, N>& v1, const Vector<T2, N>& v2) RESTRICT(cpu, amp) {
		return (v2 - v1).length();
	}

	template<typename T, typename U>
	auto cross(const Vector<T, 3>& a, const Vector<U, 3>& b)RESTRICT(cpu, amp) {
		return Vector<MultiplyType<T, U>, 3>(
			(a[1] * b[2]) - (a[2] * b[1]),
			(a[2] * b[0]) - (a[0] * b[2]),
			(a[0] * b[1]) - (a[1] * b[0]));
	}

	template<typename T1, typename T2, LmSize N>
	auto dot(const Vector<T1, N>& left, const Vector<T2, N>& right)RESTRICT(cpu, amp) {
		auto result = (MultiplyType<T1, T2>)0;
		for (LmSize i = 0; i < N; ++i) {
			result += left[i] * right[i];
		}
		return result;
	}


	template<typename T, LmSize N>
	auto normalize(const Vector<T, N>& v) RESTRICT(cpu, amp) {
		return v.normalized();
	}

	template<typename TA, typename TB, LmSize N, typename TC>
	auto lerp(const Vector<TA, N>& a, const Vector<TB, N>& b, TC c)RESTRICT(cpu, amp) {
		return a * (static_cast<TC>(1) - c) + b * c;
	}



	typedef Vector<float, 2> float2;
	typedef Vector<float, 3> float3;
	typedef Vector<float, 4> float4;
	typedef Vector<double, 2> double2;
	typedef Vector<double, 3> double3;
	typedef Vector<double, 4> double4;
}
#endif // lm_vector_h__
