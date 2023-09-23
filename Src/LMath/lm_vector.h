#ifndef lm_vector_h__
#define lm_vector_h__

#include <type_traits>

#include "lm_types.h"
#include "lm_constants.h"
#include "lm_macro.h"
#include "lm_traits.h"
#include <cmath>

namespace lm {

#define LM_VECTOR_ARITHMETIC_OP_SCALAR(_Op, _Suffix) \
	template<typename T, LmSize N, InstructionSet Instructions, typename U> \
	auto operator _Op (const Vector<T, N, Instructions>& l, const U& divider) _Suffix { \
		Vector<DivideType<T, U>, N, Instructions> result; \
		for (LmSize i = 0; i < N; ++i) { \
				result.set(i, l.get(i) _Op divider); \
		} \
		return result; \
	} 

#define LM_VECTOR_ARITHMETIC_OP_VECTOR(_Op, _Suffix) \
	template<typename T, LmSize N, InstructionSet Instructions, typename U> \
		auto operator _Op (const Vector<T, N, Instructions>& l, Vector<U, N, Instructions> divider) _Suffix {\
		Vector<DivideType<T, U>, N, Instructions> result; \
		for (LmSize i = 0; i < N; ++i) {\
				result.set(i, l.get(i) _Op divider.get(i)); \
		} \
		return result; \
	} \

#define LM_VECTOR_ARITHMETIC_OP_SELF_SCALAR(_Op, _Suffix) \
	template<typename T, LmSize N, InstructionSet Instructions, typename U>\
	auto operator _Op## = (Vector<T, N, Instructions>& l, const U& divider) _Suffix { \
		for (LmSize i = 0; i < N; ++i) { \
			l.set(i, l.get(i) _Op divider); \
		} \
		return l; \
	} 

#define LM_VECTOR_ARITHMETIC_OP_SELF_VECTOR(_Op, _Suffix) \
	template<typename T, LmSize N, InstructionSet Instructions, typename U> \
	auto operator _Op##= (Vector<T, N, Instructions>& l, Vector<U, N, Instructions> divider) _Suffix{ \
		for (LmSize i = 0; i < N; ++i) { \
			l.set(i, l.get(i) _Op divider.get(i)); \
		} \
		return l; \
	}

#define LM_VECTOR_ARITHMETIC_OP(_Op) \
	GEN_METHOD_PARAMS(LM_VECTOR_ARITHMETIC_OP_SCALAR, _Op) \
	GEN_METHOD_PARAMS(LM_VECTOR_ARITHMETIC_OP_VECTOR, _Op) \
	GEN_METHOD_PARAMS(LM_VECTOR_ARITHMETIC_OP_SELF_SCALAR, _Op) \
	GEN_METHOD_PARAMS(LM_VECTOR_ARITHMETIC_OP_SELF_VECTOR, _Op)


#define MATH_VECTOR_FUNC(_Name, _TemplateParams, _TemplateParamsNames, _ExecParams, _Params) \
	namespace impl { \
		template<typename T UNPACK _TemplateParams> \
		struct _Name { \
			template<typename = void> \
			static auto exec(const T& v UNPACK _ExecParams) RESTRICT(cpu) { return std:: _Name (v UNPACK _Params); } \
			ENABLE_IF_AMP(template<typename = void>  static auto exec(const T& v UNPACK _ExecParams) RESTRICT(amp) { return concurrency::precise_math:: _Name (v UNPACK _Params); }) \
		}; \
		template<typename T, LmSize N UNPACK _TemplateParams> \
		struct _Name <Vector<T, N> UNPACK _TemplateParamsNames> { \
			template<typename = void> \
			static auto exec(const Vector<T, N>& v UNPACK _ExecParams) RESTRICT(cpu, amp) { \
				Vector<decltype( _Name <T UNPACK _TemplateParamsNames>::exec(*static_cast<T*>(nullptr) UNPACK _Params)), N> result; \
				for (LmSize i = 0; i < N; ++i) { result[i] = impl:: _Name <T UNPACK _TemplateParamsNames>::exec(v[i] UNPACK _Params); } \
				return result; \
			} \
		}; \
	} \
	template<typename T UNPACK _TemplateParams> \
	auto _Name(const T& v UNPACK _ExecParams) RESTRICT(cpu, amp) { \
		return impl:: _Name <T UNPACK _TemplateParamsNames>::exec(v UNPACK _Params); \
	}

#define MATH_VECTOR_FUNC_ONE_PARAM(_Name)  MATH_VECTOR_FUNC(_Name, (, typename TParam), (, TParam), (, TParam x), (, x))
#define MATH_VECTOR_FUNC_NO_PARAM(_Name)  MATH_VECTOR_FUNC(_Name, (), (), (), ())


	enum class InstructionSet {
		Generic,
		SSE,
		AVX
	};

	template<typename T, LmSize N, InstructionSet Instructions = InstructionSet::Generic>
	struct VectorData {
		T data[N];

#define VECTOR_DATA_GET(_Suffix) \
		template<typename = void> \
		T& get(LmSize id)   _Suffix{ \
			return data[id]; \
		} \
		template<typename = void> \
		const T& get(LmSize id) const  _Suffix{ \
			return data[id]; \
		}
		GEN_METHOD(VECTOR_DATA_GET)

#define VECTOR_DATA_SET(_Suffix) \
		template<typename = void> \
		void set(LmSize id, T value) _Suffix { \
			data[id] = value; \
		}
		GEN_METHOD(VECTOR_DATA_SET)

		constexpr VectorData() {}

#ifdef LM_AMP_SUPPORTED
		template<typename = void>
		constexpr VectorData() restrict(amp) {}
#endif

#define CTOR_VA(_Suffix) \
		template<typename ... Args, LmSize M = N, typename = std::enable_if_t<(sizeof...(Args) == M) && (M > 4)>> \
		constexpr VectorData(const Args& ... rest) _Suffix : data{ rest... } {}
		GEN_METHOD(CTOR_VA)

			//Vector2 constructors
#define CTOR_V2(_Suffix) \
		template<typename TA, LmSize M = N,typename = std::enable_if_t<M == 2>> \
		constexpr VectorData(const TA& a, const TA& b) _Suffix : data{ a, b } {}
			GEN_METHOD(CTOR_V2)

			//Vector3 constructors
#define CTOR_V3_0(_Suffix) \
		template<typename TA, LmSize M = N,typename = typename std::enable_if<M == 3, T>::type>\
		VectorData(const TA& a, const TA& b, const TA& c) _Suffix : data{ static_cast<T>(a), static_cast<T>(b), static_cast<T>(c) } {}
			GEN_METHOD(CTOR_V3_0)

#define CTOR_V3_1_BASE(_Suffix) \
		template<typename TA, LmSize M = N,typename = std::enable_if_t<M == 3>> \
		constexpr VectorData(const VectorData<TA, 2>& a, const TA& b) _Suffix : data{ a.data[0], a.data[1], b } {}
			GEN_METHOD(CTOR_V3_1_BASE)

#define CTOR_V3_2_BASE(_Suffix) \
		template<typename TA, LmSize M = N,typename = std::enable_if_t<M == 3>> \
		constexpr VectorData(const TA& a, const VectorData<TA, 2>& b) _Suffix : data{ a, b.data[0], b.data[1] } {}
			GEN_METHOD(CTOR_V3_2_BASE)

			//Vector4 constructors
#define CTOR_V4_0_BASE(_Suffix) \
		template<typename TA, LmSize M = N,typename = std::enable_if_t<M == 4>> \
		constexpr VectorData(const TA& a, const TA& b, const TA& c, const TA& d) _Suffix : data{ static_cast<T>(a), static_cast<T>(b), static_cast<T>(c), static_cast<T>(d) } {}
			GEN_METHOD(CTOR_V4_0_BASE)

#define CTOR_V4_1_BASE(_Suffix) \
		template<typename TA, LmSize M = N,typename = std::enable_if_t<M == 4>> \
		constexpr VectorData(const TA& a, const TA& b, const VectorData<T, 2>& c) _Suffix : data{ a, b, c.data[0], c.data[1] } {}
			GEN_METHOD(CTOR_V4_1_BASE)

#define CTOR_V4_2_BASE(_Suffix) \
		template<typename TA, LmSize M = N,typename = std::enable_if_t<M == 4>> \
		constexpr VectorData(const VectorData<TA, 2>& a, const TA& b, const TA& c) _Suffix  : data{ a.data[0], a.data[1], b, c } {}
			GEN_METHOD(CTOR_V4_2_BASE)

#define CTOR_V4_3_BASE(_Suffix) \
		template<typename TA, LmSize M = N,typename = std::enable_if_t<M == 4>> \
		constexpr VectorData(const VectorData<TA, 2>& a, const VectorData<TA, 2>& b) _Suffix  : data{ a.data[0], a.data[1], b.data[0], b.data[1] } {}
			GEN_METHOD(CTOR_V4_3_BASE)

#define CTOR_V4_4_BASE(_Suffix) \
		template<typename TA, LmSize M = N,typename = std::enable_if_t<M == 4>> \
		constexpr VectorData(const TA& a, const VectorData<TA, 3>& b) _Suffix : data{ a, b.data[0], b.data[1], b.data[2] } {}
			GEN_METHOD(CTOR_V4_4_BASE)

#define CTOR_V4_5_BASE(_Suffix) \
		template<typename TA, LmSize M = N,typename = std::enable_if_t<M == 4>> \
		constexpr VectorData(const VectorData<TA, 3>& a, const TA& b) _Suffix  : data{ a.data[0], a.data[1], a.data[2], b } {}
			GEN_METHOD(CTOR_V4_5_BASE)

#define CTOR_S(_Suffix) \
		template<typename Arg> \
		VectorData(Arg arg) _Suffix { \
			for (LmSize i = 0; i < N; ++i) { \
				data[i] = arg; \
			} \
		}
		GEN_METHOD(CTOR_S)
	};

	template<typename T, LmSize N, InstructionSet Instructions = InstructionSet::Generic>
	struct Vector : public VectorData<T, N, Instructions> {
	public:
		static constexpr LmSize Size = N;
		typedef T ElementType;

		constexpr Vector() {}

#ifdef LM_AMP_SUPPORTED
		template<typename = void>
		constexpr Vector() restrict(amp) {}
#endif

		typedef VectorData<T, N, Instructions> Base;

		using Base::Base;

#define CTOR_V3_1(_Suffix) \
		template<typename TA, LmSize M = N,typename = std::enable_if_t<M == 3>> \
		constexpr Vector(const Vector<TA, 2>& a, const TA& b) _Suffix : Base{ (const VectorData<TA, 2>&)a, b } {}
		GEN_METHOD(CTOR_V3_1)

#define CTOR_V3_2(_Suffix) \
		template<typename TA, LmSize M = N, typename = std::enable_if_t<M == 3>> \
		constexpr Vector(const TA& a, const Vector<TA, 2>& b) _Suffix : Base{ a, (const VectorData<TA, 2>&)b} {}
		GEN_METHOD(CTOR_V3_2)

		//Vector4 constructors
#define CTOR_V4_1(_Suffix) \
		template<typename TA, LmSize M = N,typename = std::enable_if_t<M == 4>> \
		constexpr Vector(const TA& a, const TA& b, const Vector<T, 2>& c) _Suffix : Base {a, b, (const VectorData<T, 2>&)c} {}
		GEN_METHOD(CTOR_V4_1)

#define CTOR_V4_2(_Suffix) \
		template<typename TA, LmSize M = N,typename = std::enable_if_t<M == 4>> \
		constexpr Vector(const Vector<TA, 2>& a, const TA& b, const TA& c) _Suffix  : Base{(const VectorData<TA, 2>&)a, b, c}{}
		GEN_METHOD(CTOR_V4_2)

#define CTOR_V4_3(_Suffix) \
		template<typename TA, LmSize M = N, typename = std::enable_if_t<M == 4>> \
		constexpr Vector(const Vector<TA, 2>& a, const Vector<TA, 2>& b) _Suffix  : Base { (const VectorData<TA, 2>&)a, (const VectorData<TA, 2>&)b} {}
		GEN_METHOD(CTOR_V4_3)

#define CTOR_V4_4(_Suffix) \
		template<typename TA, LmSize M = N,typename = std::enable_if_t<M == 4>> \
		constexpr Vector(const TA& a, const Vector<TA, 3>& b) _Suffix : Base {a, (const VectorData<TA, 3>&)b}  {}
		GEN_METHOD(CTOR_V4_4)

#define CTOR_V4_5(_Suffix) \
		template<typename TA, LmSize M = N,typename = std::enable_if_t<M == 4>> \
		constexpr Vector(const Vector<TA, 3>& a, const TA& b) _Suffix  : Base{ (const VectorData<TA, 3>&)a, b} {}
		GEN_METHOD(CTOR_V4_5)


		//UnitX
#define UNIT_X0(_Suffix) \
		template<LmSize M = N> \
		static typename std::enable_if<M == 1, Vector>::type unitX() _Suffix { \
			return Vector(static_cast<T>(1)); \
		}
		GEN_METHOD(UNIT_X0)

#define UNIT_X1(_Suffix) \
		template<LmSize M = N> \
		static typename std::enable_if<M == 2, Vector>::type unitX() _Suffix { \
			return Vector(static_cast<T>(1), static_cast<T>(0)); \
		}
		GEN_METHOD(UNIT_X1)

#define UNIT_X2(_Suffix) \
		template<LmSize M = N> \
		static typename std::enable_if<M == 3, Vector>::type unitX() _Suffix { \
			return Vector(static_cast<T>(1), static_cast<T>(0), static_cast<T>(0)); \
		}
		GEN_METHOD(UNIT_X2)

#define UNIT_X3(_Suffix) \
		template<LmSize M = N> \
		static typename std::enable_if<M == 4, Vector>::type unitX() _Suffix { \
			return Vector(static_cast<T>(1), static_cast<T>(0), static_cast<T>(0), static_cast<T>(0)); \
		}
		GEN_METHOD(UNIT_X3)


		//UnitY
#define UNIT_Y0(_Suffix) \
		template<LmSize M = N> \
		static typename std::enable_if<M == 2, Vector>::type unitY() _Suffix { \
			return Vector(static_cast<T>(0), static_cast<T>(1)); \
		}
		GEN_METHOD(UNIT_Y0)

#define UNIT_Y1(_Suffix) \
		template<LmSize M = N> \
		static typename std::enable_if<M == 3, Vector>::type unitY() _Suffix { \
			return Vector(static_cast<T>(0), static_cast<T>(1), static_cast<T>(0)); \
		}
		GEN_METHOD(UNIT_Y1)

#define UNIT_Y2(_Suffix) \
		template<LmSize M = N> \
		static typename std::enable_if<M == 4, Vector>::type unitY() _Suffix { \
			return Vector(static_cast<T>(0), static_cast<T>(1), static_cast<T>(0), static_cast<T>(0)); \
		}
		GEN_METHOD(UNIT_Y2)

		//UnitZ
#define UNIT_Z0(_Suffix) \
		template<LmSize M = N> \
		static typename std::enable_if<M == 3, Vector>::type unitZ() _Suffix { \
			return Vector(static_cast<T>(0), static_cast<T>(0), static_cast<T>(1)); \
		}
		GEN_METHOD(UNIT_Z0)

#define UNIT_Z1(_Suffix) \
		template<LmSize M = N>  \
		static typename std::enable_if<M == 4, Vector>::type unitZ() _Suffix { \
			return Vector(static_cast<T>(0), static_cast<T>(0), static_cast<T>(1), static_cast<T>(0));   \
		}
		GEN_METHOD(UNIT_Z1)

		//UnitW
#define UNIT_W0(_Suffix) \
		template<LmSize M = N> \
		static typename std::enable_if<M == 4, Vector>::type unitW() _Suffix { \
			return Vector(static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(1)); \
		}
		GEN_METHOD(UNIT_W0)

			//EMPTY_SUFFIX

#define VECTOR_ITEM_ACCESSOR_BASE(_Name, _Index, _Modifier, _Restrict) \
			template<LmSize S = Size, typename = std::enable_if_t<(S == Size) && (S > _Index)>> \
			_Modifier auto& _Name () _Modifier _Restrict { return this->get(_Index); }

#define VECTOR_ITEM_ACCESSOR(_Name, _Index) \
			VECTOR_ITEM_ACCESSOR_BASE(_Name, _Index, EMPTY_SUFFIX, EMPTY_SUFFIX) \
			VECTOR_ITEM_ACCESSOR_BASE(_Name, _Index, const, EMPTY_SUFFIX) \
			ENABLE_IF_AMP(VECTOR_ITEM_ACCESSOR_BASE(_Name, _Index, EMPTY_SUFFIX, restrict(amp)) ) \
			ENABLE_IF_AMP(VECTOR_ITEM_ACCESSOR_BASE(_Name, _Index, const, restrict(amp)) )

		//accessors
		VECTOR_ITEM_ACCESSOR(x, 0);
		VECTOR_ITEM_ACCESSOR(y, 1);
		VECTOR_ITEM_ACCESSOR(z, 2);
		VECTOR_ITEM_ACCESSOR(w, 3);


#define SLICE(_Prefix, _Suffix) \
		template<LmSize Offset, LmSize Length, typename = std::enable_if_t<Offset + Length <= N>> \
		_Prefix Vector<T, Length>& slice() _Suffix { \
			return *((Vector<T, Length>*)&this->data[Offset]); \
		}
		GEN_METHOD2(SLICE)

#define LINEAR_SLICE_ACCESSOR_BASE(_Name, _Offset, _Length, _Prefix, _Suffix) \
		template<LmSize SliceOffset = _Offset, LmSize SliceLength = _Length, typename = std::enable_if_t<(SliceOffset == _Offset) && (SliceLength == _Length) &&  ((SliceOffset + SliceLength) <= N)>> \
		_Prefix auto& _Name () _Suffix { return slice<SliceOffset, SliceLength>(); }


#define LM_VECTOR_LINEAR_SLICE_ACCESSOR_XY(_Prefix, _Suffix) LINEAR_SLICE_ACCESSOR_BASE(xy, 0, 2, _Prefix, _Suffix)
#define LM_VECTOR_LINEAR_SLICE_ACCESSOR_XYZ(_Prefix, _Suffix) LINEAR_SLICE_ACCESSOR_BASE(xyz, 0, 3, _Prefix, _Suffix)
#define LM_VECTOR_LINEAR_SLICE_ACCESSOR_XYZW(_Prefix, _Suffix) LINEAR_SLICE_ACCESSOR_BASE(xyzw, 0, 4, _Prefix, _Suffix)
#define LM_VECTOR_LINEAR_SLICE_ACCESSOR_YZ(_Prefix, _Suffix) LINEAR_SLICE_ACCESSOR_BASE(yz, 1, 2, _Prefix, _Suffix)
#define LM_VECTOR_LINEAR_SLICE_ACCESSOR_YZW(_Prefix, _Suffix) LINEAR_SLICE_ACCESSOR_BASE(yzw, 1, 3, _Prefix, _Suffix)
#define LM_VECTOR_LINEAR_SLICE_ACCESSOR_ZW(_Prefix, _Suffix) LINEAR_SLICE_ACCESSOR_BASE(zw, 2, 2, _Prefix, _Suffix)

			GEN_METHOD2(LM_VECTOR_LINEAR_SLICE_ACCESSOR_XY)
			GEN_METHOD2(LM_VECTOR_LINEAR_SLICE_ACCESSOR_XYZ)
			GEN_METHOD2(LM_VECTOR_LINEAR_SLICE_ACCESSOR_XYZW)
			GEN_METHOD2(LM_VECTOR_LINEAR_SLICE_ACCESSOR_YZ)
			GEN_METHOD2(LM_VECTOR_LINEAR_SLICE_ACCESSOR_YZW)
			GEN_METHOD2(LM_VECTOR_LINEAR_SLICE_ACCESSOR_ZW)


#define LEN_SQ(_Prefix, _Suffix) \
		template<typename TR = MultiplyType<T>> \
		_Prefix auto lengthSquared() _Suffix { \
			TR result = DefaultValues<TR>::zero(); \
			for (LmSize i = 0; i < N; ++i) { \
				result += this->get(i) * this->get(i); \
			} \
			return result;\
		}

		GEN_METHOD2(LEN_SQ)

		auto length() const RESTRICT(cpu);
#if defined(LM_AMP_SUPPORTED)
		auto length() const RESTRICT(amp);
#endif 

#define OP_INDEX(_Prefix, _Suffix) \
		template<typename = void> \
		_Prefix T& operator [] (LmSize id) _Suffix { \
			return this->get(id); \
		}
		GEN_METHOD2(OP_INDEX)

#define NORMALIZED(_Suffix) \
		auto normalized() _Suffix { \
			return (*this) / length(); \
		}
		GEN_METHOD_CONST(NORMALIZED)


		/*LM_VECTOR_ARITHMETIC_OP(+);
		LM_VECTOR_ARITHMETIC_OP(-);
		LM_VECTOR_ARITHMETIC_OP(*);
		LM_VECTOR_ARITHMETIC_OP(/ );*/

		template<typename = void>
		bool equals(const Vector& other, T tolerance) const;
		ENABLE_IF_AMP(template<typename = void> bool equals(const Vector& other, T tolerance) const restrict(amp); )

#define OP_NEG(_Suffix) \
		template<typename = void> \
		auto operator-() const _Suffix { \
			Vector<typename std::decay<decltype(-this->x())>::type, Size> result; \
			for (LmSize i = 0; i < Size; ++i) { \
				result[i] = -this->get(i); \
			} \
			return result; \
		}

		GEN_METHOD(OP_NEG)

#define OP_EQ(_Suffix) \
		template<typename T2> \
		bool operator==(const Vector<T2, Size>& right) const _Suffix { \
			for (LmSize i = 0; i < Size; ++i) { \
				if (this->get(i) != right[i]) { \
					return false; \
				} \
			} \
			return true; \
		}
		GEN_METHOD(OP_EQ)

#define OP_NEQ(_Suffix) \
		template<typename T2> \
		bool operator!=(const Vector<T2, Size>& right) const _Suffix { \
			for (LmSize i = 0; i < Size; ++i) { \
				if (this->get(i) != right[i]) { \
					return true; \
				} \
			} \
			return false; \
		}
		GEN_METHOD(OP_NEQ)

#define OP_CAST_RAW(_Prefix, _Suffix) \
		template<typename = void> \
		explicit operator _Prefix T*() _Suffix { \
			return Size == 0 ? nullptr : &this->get(0); \
		}
		GEN_METHOD2(OP_CAST_RAW)

	};



	template<typename T, typename U>
	auto cross(const Vector<T, 3>& a, const Vector<U, 3>& b) RESTRICT(cpu, amp) {
		return Vector<MultiplyType<T, U>, 3>(
			(a[1] * b[2]) - (a[2] * b[1]),
			(a[2] * b[0]) - (a[0] * b[2]),
			(a[0] * b[1]) - (a[1] * b[0]));
	}

	template<typename T1, typename T2, LmSize N>
	auto dot(const Vector<T1, N>& left, const Vector<T2, N>& right)RESTRICT(cpu, amp) {
		auto result = DefaultValues<MultiplyType<T1, T2>>::zero();
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
		return a * (DefaultValues<TC>::one() - c) + b * c;
	}

	template<typename T, LmSize N>
	auto all(const Vector<T, N>& v) RESTRICT(cpu, amp) {
		for (LmSize i = 0; i < N; ++i) {
			if (v[i] == DefaultValues<T>::zero()) {
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


	namespace impl {
		template<typename T>
		struct abs {};
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

	LM_VECTOR_ARITHMETIC_OP(+);
	LM_VECTOR_ARITHMETIC_OP(-);
	LM_VECTOR_ARITHMETIC_OP(*);
	LM_VECTOR_ARITHMETIC_OP(/ );


	template<typename T>
	auto abs(const T& v) RESTRICT(cpu, amp) {
		return impl::abs<T>::exec(v);
	}


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

	MATH_VECTOR_FUNC_NO_PARAM(sqrt);

#define LM_VEC_METHOD_LENGTH(_Suffix) \
	template<typename T, LmSize N, InstructionSet Instructions> \
	auto Vector<T, N, Instructions>::length() const _Suffix { \
		return lm::sqrt(lengthSquared()); \
	}

	GEN_METHOD(LM_VEC_METHOD_LENGTH)


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

	template<typename T, LmSize N, typename = std::enable_if_t<(N > 0)>>
	auto min(const Vector<T, N>& v) RESTRICT(cpu, amp) {
		LmSize id = 0;
		for (LmSize i = 1; i < N; ++i) {
			if (v[i] < v[id]) {
				id = i;
			}
		}
		return v[id];
	}

	template<typename T, LmSize N, typename = std::enable_if_t <(N > 0)>>
		auto max(const Vector<T, N>& v) RESTRICT(cpu, amp) {
		LmSize id = 0;
		for (LmSize i = 1; i < N; ++i) {
			if (v[i] > v[id]) {
				id = i;
			}
		}
		return v[id];
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

#define LM_VECTOR_EQUALS(_Suffix) \
	template<typename T, LmSize N, InstructionSet Instructions> \
template<typename> \
	bool Vector<T, N, Instructions>::equals(const Vector<T, N, Instructions>& other, T tolerance) const _Suffix { \
		for (LmSize i = 0; i < N; ++i) { \
			if (lm::abs(this->get(i) - other.get(i)) >= tolerance) { \
				return false; \
			} \
		} \
		return true; \
	}

	GEN_METHOD(LM_VECTOR_EQUALS)

	template<typename T, LmSize N, InstructionSet Instructions>
	bool equals(const Vector<T, N, Instructions>& a, const Vector<T, N, Instructions>& b, T tolerance) RESTRICT(cpu, amp) {
		return a.equals(b);
	}

	template<typename T1, typename T2, LmSize N>
	auto distance(const Vector<T1, N>& v1, const Vector<T2, N>& v2) RESTRICT(cpu, amp) {
		return (v2 - v1).length();
	}

	

	typedef Vector<float, 2> float2;
	typedef Vector<float, 3> float3;
	typedef Vector<float, 4> float4;
	typedef Vector<double, 2> double2;
	typedef Vector<double, 3> double3;
	typedef Vector<double, 4> double4;
}
#endif // lm_vector_h__
