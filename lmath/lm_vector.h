#ifndef lm_vector_h__
#define lm_vector_h__

#include <type_traits>
#include <array>
#include "lm_types.h"
#include "lm_constants.h"

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

		//UnitX
		template<LmSize M = N>
		static typename std::enable_if<M == 1, Vector>::type unitX() RESTRICT(cpu, amp) {
			return Vector(static_cast<T>(1));
		}

		template<LmSize M = N>
		static typename std::enable_if<M == 2, Vector>::type unitX() RESTRICT(cpu, amp) {
			return Vector(static_cast<T>(1), static_cast<T>(0));
		}

		template<LmSize M = N>
		static typename std::enable_if<M == 3, Vector>::type unitX() RESTRICT(cpu, amp) {
			return Vector(static_cast<T>(1), static_cast<T>(0), static_cast<T>(0));
		}
		template<LmSize M = N>
		static typename std::enable_if<M == 4, Vector>::type unitX() RESTRICT(cpu, amp) {
			return Vector(static_cast<T>(1), static_cast<T>(0), static_cast<T>(0), static_cast<T>(0));
		}

		//UnitY
		template<LmSize M = N>
		static typename std::enable_if<M == 2, Vector>::type unitY() RESTRICT(cpu, amp) {
			return Vector(static_cast<T>(0), static_cast<T>(1));
		}

		template<LmSize M = N>
		static typename std::enable_if<M == 3, Vector>::type unitY() RESTRICT(cpu, amp) {
			return Vector(static_cast<T>(0), static_cast<T>(1), static_cast<T>(0));
		}
		template<LmSize M = N>
		static typename std::enable_if<M == 4, Vector>::type unitY() RESTRICT(cpu, amp) {
			return Vector(static_cast<T>(0), static_cast<T>(1), static_cast<T>(0), static_cast<T>(0));
		}
		//UnitZ
		template<LmSize M = N>
		static typename std::enable_if<M == 3, Vector>::type unitZ() RESTRICT(cpu, amp) {
			return Vector(static_cast<T>(0), static_cast<T>(0), static_cast<T>(1));
		}
		template<LmSize M = N>
		static typename std::enable_if<M == 4, Vector>::type unitZ() RESTRICT(cpu, amp) {
			return Vector(static_cast<T>(0), static_cast<T>(0), static_cast<T>(1), static_cast<T>(0));
		}
		//UnitW
		template<LmSize M = N>
		static typename std::enable_if<M == 4, Vector>::type unitW() RESTRICT(cpu, amp) {
			return Vector(static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(1));
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

	template<typename T, LmSize N>
	auto all(const Vector<T, N>& v) RESTRICT(cpu, amp) {
		for (LmSize i = 0; i < N; ++i) {
			if (v[i] == static_cast<T>(0)) {
				return false;
			}
		}
		return true;
	}

	template<typename T, LmSize N>
	auto any(const Vector<T, N>& v) RESTRICT(cpu, amp) {
		for (LmSize i = 0; i < N; ++i) {
			if (v[i] != static_cast<T>(0)) {
				return true;
			}
		}
		return false;
	}


#define UNPACK(...) __VA_ARGS__



#define MATH_VECTOR_FUNC(_Name, _TemplateParams, _TemplateParamsNames, _ExecParams, _Params) \
	namespace impl { \
		template<typename T UNPACK _TemplateParams> \
		struct _Name { \
			static auto exec(const T& v UNPACK _ExecParams) RESTRICT(cpu) { return std::##_Name##(v UNPACK _Params); } \
			ENABLE_IF_AMP(static auto exec(const T& v UNPACK _ExecParams) RESTRICT(amp) { return concurrency::precise_math::##_Name##(v UNPACK _Params); }) \
		}; \
		template<typename T, LmSize N UNPACK _TemplateParams> \
		struct _Name <Vector<T, N> UNPACK _TemplateParamsNames> { \
			static auto exec(const Vector<T, N>& v UNPACK _ExecParams) RESTRICT(cpu, amp) { \
				Vector<decltype(##_Name##<T UNPACK _TemplateParamsNames>::exec(*static_cast<T*>(nullptr) UNPACK _Params)), N> result; \
				for (LmSize i = 0; i < N; ++i) { result[i] = _Name##<T UNPACK _TemplateParamsNames>::exec(v[i] UNPACK _Params); } \
				return result; \
			} \
		}; \
	} \
	template<typename T UNPACK _TemplateParams> \
	auto _Name(const T& v UNPACK _ExecParams) RESTRICT(cpu, amp) { \
		return impl::##_Name##<T UNPACK _TemplateParamsNames>::exec(v UNPACK _Params); \
	}


	namespace impl {
		template<typename T>
		struct abs {
		};
		template<>
		struct abs<float> {
			static auto exec(const float& v) RESTRICT(cpu) { return std::fabs(v); } 
			ENABLE_IF_AMP(static auto exec(const float& v) RESTRICT(amp) { return concurrency::precise_math::fabs(v); })
		};
		template<>
		struct abs<double> {
			static auto exec(const double& v) RESTRICT(cpu) { return std::fabs(v); }
			ENABLE_IF_AMP(static auto exec(const double& v) RESTRICT(amp) { return concurrency::precise_math::fabs(v); })
		};

		template<typename T, LmSize N>
		struct abs<Vector<T, N>> {
			static auto exec(const Vector<T, N>& v) RESTRICT(cpu, amp) {
				Vector<T, N> result;
				for (LmSize i = 0; i < N; ++i) { result[i] = abs<T>::exec(v[i]); }
				return result;
			}
		};
	}
	template<typename T>
	auto abs(const T& v) RESTRICT(cpu, amp) {
		return impl::abs<T>::exec(v);
	}

#define MATH_VECTOR_FUNC_ONE_PARAM(_Name)  MATH_VECTOR_FUNC(_Name, (, typename TParam), (, TParam), (, TParam x), (, x))
#define MATH_VECTOR_FUNC_NO_PARAM(_Name)  MATH_VECTOR_FUNC(_Name, (), (), (), ())

	MATH_VECTOR_FUNC_ONE_PARAM(pow);
	MATH_VECTOR_FUNC_NO_PARAM(sin);
	MATH_VECTOR_FUNC_NO_PARAM(cos);
	MATH_VECTOR_FUNC_NO_PARAM(acos);
	MATH_VECTOR_FUNC_NO_PARAM(asin);
	MATH_VECTOR_FUNC_NO_PARAM(cosh);
	MATH_VECTOR_FUNC_NO_PARAM(sinh);
	MATH_VECTOR_FUNC_NO_PARAM(tan);
	MATH_VECTOR_FUNC_NO_PARAM(atan);
	MATH_VECTOR_FUNC_NO_PARAM(floor);
	MATH_VECTOR_FUNC_NO_PARAM(ceil);
	MATH_VECTOR_FUNC_NO_PARAM(exp);
	MATH_VECTOR_FUNC_NO_PARAM(log);

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif
	namespace impl {
		template<typename T>
		struct min {
			static auto exec(const T& a, const T& b) RESTRICT(cpu, amp) {
				return a < b ? a : b;
			}
		};

		template<typename T, LmSize N>
		struct min<Vector<T, N>> {
			static auto exec(const Vector<T, N>& a, const Vector<T, N>& b) RESTRICT(cpu, amp) {
				Vector<T, N> result;
				for (LmSize i = 0; i < N; ++i) { result[i] = impl::min<T>::exec(a[i], b[i]); }
				return result;
			}
		};
	}

	template<typename T>
	auto min(const T& a, const T& b) RESTRICT(cpu, amp) {
		return impl::min<T>::exec(a, b);
	}

	namespace impl {
		template<typename T>
		struct max {
			static auto exec(const T& a, const T& b) RESTRICT(cpu, amp) {
				return a > b ? a : b;
			}
		};

		template<typename T, LmSize N>
		struct max<Vector<T, N>> {
			static auto exec(const Vector<T, N>& a, const Vector<T, N>& b) RESTRICT(cpu, amp) {
				Vector<T, N> result;
				for (LmSize i = 0; i < N; ++i) { result[i] = impl::max<T>::exec(a[i], b[i]); }
				return result;
			}
		};
	}

	template<typename T>
	auto max(const T& a, const T& b) RESTRICT(cpu, amp) {
		return impl::max<T>::exec(a, b);
	}

	template<typename T, typename TRange>
	auto clamp(const T& a, const TRange& minValue, const TRange& maxValue) RESTRICT(cpu, amp) {
		return lm::min(lm::max(a, minValue), maxValue);
	}



	namespace impl {
		template<typename T>
		struct degrees {
			static auto exec(const T& a) RESTRICT(cpu, amp) {
				return (static_cast<T>(180) / static_cast<T>(pi_d)) * a;
			}
		};

		template<typename T, LmSize N>
		struct degrees<Vector<T, N>> {
			static auto exec(const Vector<T, N>& a) RESTRICT(cpu, amp) {
				Vector<T, N> result;
				for (LmSize i = 0; i < N; ++i) { result[i] = impl::degrees<T>::exec(a[i]); }
				return result;
			}
		};
	}

	template<typename T>
	auto degrees(const T& a) RESTRICT(cpu, amp) {
		return impl::degrees<T>::exec(a);
	}


	typedef Vector<float, 2> float2;
	typedef Vector<float, 3> float3;
	typedef Vector<float, 4> float4;
	typedef Vector<double, 2> double2;
	typedef Vector<double, 3> double3;
	typedef Vector<double, 4> double4;
}
#endif // lm_vector_h__
