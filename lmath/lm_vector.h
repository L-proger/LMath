#ifndef lm_vector_h__
#define lm_vector_h__

#include "lm_vector_traits.h"
#include <type_traits>
#include <array>
#include "lm_types.h"

namespace lm {
	template<typename T, lm_size_type Size>
	struct Vector_data {
		T data[Size];
	};

	template<typename T, lm_size_type Size>
	struct Vector : public Vector_base, public Vector_data<T, Size> {
	public:
		static constexpr lm_size_type size = Size;
		typedef T element_type;

		constexpr Vector() RESTRICT(cpu) {}

		constexpr Vector(T value) RESTRICT(cpu) : Vector_data<T, Size>(value) {}

		template<lm_size_type S = Size, typename = typename std::enable_if<S == 2>::type>
		constexpr Vector(T x, T y) RESTRICT(cpu) : Vector_data<T, Size>(x, y) {}

		template<lm_size_type S = Size, typename = typename std::enable_if<S == 3>::type>
		constexpr Vector(T x, T y, T z) RESTRICT(cpu) : Vector_data<T, Size>(x, y, z) {}

		template<lm_size_type S = Size, typename = typename std::enable_if<S == 4>::type>
		constexpr Vector(T x, T y, T z, T w) RESTRICT(cpu) : Vector_data<T, Size>(x, y, z, w) {}

		template<lm_size_type S = Size, typename = typename std::enable_if<S == 3>::type>
		static constexpr Vector right() RESTRICT(cpu) {
			return Vector(1, 0, 0);
		}

		template<lm_size_type S = Size, typename = typename std::enable_if<S == 3>::type>
		static constexpr Vector up()RESTRICT(cpu) {
			return Vector(0, 1, 0);
		}

		template<lm_size_type S = Size, typename = typename std::enable_if<S == 3>::type>
		static constexpr Vector forward()RESTRICT(cpu) {
			return Vector(0, 0, 1);
		}

		template<typename U = Vector>
		static constexpr Vector zero(typename std::enable_if<U::size == 3>::type* p = nullptr) RESTRICT(cpu) {
			return Vector(0, 0, 0);
		}

		template<typename U = Vector>
		static constexpr Vector zero(typename std::enable_if<U::size == 2>::type* p = nullptr) RESTRICT(cpu) {
			return Vector(0, 0);
		}

#if defined(LM_AMP_SUPPORTED)
		Vector() RESTRICT(amp) {}

		Vector(T x) RESTRICT(amp) : Vector_data<T, Size>(x) {}

		template<typename = std::enable_if<Size == 2>::type>
		Vector(T x, T y) RESTRICT(amp) : Vector_data<T, Size>(x, y) {}

		template<typename = std::enable_if<Size == 3>::type>
		Vector(T x, T y, T z) RESTRICT(amp) : Vector_data<T, Size>(x, y, z) {}

		template<typename = std::enable_if<Size == 4>::type>
		Vector(T x, T y, T z, T w) RESTRICT(amp) : Vector_data<T, Size>(x, y, z, w) {}

		template<typename = std::enable_if<Size == 3>::type>
		static Vector up() RESTRICT(amp) {
			return Vector(0, 1, 0);
		}

		template<typename = std::enable_if<Size == 3>::type>
		static Vector forward()RESTRICT(amp) {
			return Vector(0, 0, 1);
		}

		template<typename = std::enable_if<Size == 3>::type>
		static Vector right() RESTRICT(amp) {
			return Vector(1, 0, 0);
		}
		template<typename U = Vector>
		static Vector zero(typename std::enable_if<U::size == 3>::type* p = nullptr) RESTRICT(amp) {
			return Vector(0, 0, 0);
		}

		template<typename U = Vector>
		static Vector zero(typename std::enable_if<U::size == 2>::type* p = nullptr) RESTRICT(amp) {
			return Vector(0, 0);
		}
#endif

		//op add
		template<typename U, typename = typename std::enable_if<!vector_traits::is_vector<U>::value>::type>
		auto operator+(U value) const RESTRICT(cpu, amp) {
			Vector<decltype(this->data[0] + value), Size> result;
			for (lm_size_type i = 0; i < Size; ++i) {
				result.data[i] = this->data[i] + value;
			}
			return result;
		}

		template<typename U, typename = typename std::enable_if<vector_traits::is_vector<U>::value && (U::size == Size)>::type>
		auto operator+(const U& value) const RESTRICT(cpu, amp) {
			Vector<decltype(this->data[0] + value.data[0]), Size> result;
			for (lm_size_type i = 0; i < Size; ++i) {
				result.data[i] = this->data[i] + value.data[i];
			}
			return result;
		}


		//op sub
		template<typename U, typename = typename std::enable_if<!vector_traits::is_vector<U>::value>::type>
		auto operator-(U value) const RESTRICT(cpu, amp) {
			Vector<decltype(this->data[0] - value), Size> result;
			for (lm_size_type i = 0; i < Size; ++i) {
				result.data[i] = this->data[i] - value;
			}
			return result;
		}

		template<typename U, typename = typename std::enable_if<vector_traits::is_vector<U>::value && (U::size == Size)>::type>
		auto operator-(const U& value) const RESTRICT(cpu, amp) {
			Vector<decltype(this->data[0] - value.data[0]), Size> result;
			for (lm_size_type i = 0; i < Size; ++i) {
				result.data[i] = this->data[i] - value.data[i];
			}
			return result;
		}

		//op mul
		template<typename U, typename = typename std::enable_if<!vector_traits::is_vector<U>::value>::type>
		auto operator*(U value) const RESTRICT(cpu, amp) {
			Vector<decltype(this->data[0] * value), Size> result;
			for (lm_size_type i = 0; i < Size; ++i) {
				result.data[i] = this->data[i] * value;
			}
			return result;
		}

		template<typename U, typename = typename std::enable_if<vector_traits::is_vector<U>::value && (U::size == Size)>::type>
		auto operator *(const U& value) const RESTRICT(cpu, amp) {
			Vector<decltype(this->data[0] * value.data[0]), Size> result;
			for (lm_size_type i = 0; i < Size; ++i) {
				result.data[i] = this->data[i] * value.data[i];
			}
			return result;
		}

		//op div
		template<typename U, typename = typename std::enable_if<!vector_traits::is_vector<U>::value>::type>
		auto operator/(U value) const RESTRICT(cpu, amp) {
			Vector<decltype(this->data[0] / value), Size> result;
			for (lm_size_type i = 0; i < Size; ++i) {
				result.data[i] = this->data[i] / value;
			}
			return result;
		}

		template<typename U, typename = typename std::enable_if<vector_traits::is_vector<U>::value && (U::size == Size)>::type>
		auto operator /(const U& value) const RESTRICT(cpu, amp) {
			Vector<decltype(this->data[0] / value.data[0]), Size> result;
			for (lm_size_type i = 0; i < Size; ++i) {
				result.data[i] = this->data[i] / value.data[i];
			}
			return result;
		}


		Vector& operator=(const Vector& value) RESTRICT(cpu, amp) {
			for (lm_size_type i = 0; i < Size; ++i) {
				this->data[i] = value.data[i];
			}
			return *this;
		}
	};

	template<typename T>
	struct Vector_data<T, 1> {
		union {
			T data[1];
			T x;
		};
	};

	template<typename T>
	struct Vector_data<T, 2> {
		constexpr Vector_data() RESTRICT(cpu) : data{ 0,0 } {}
		constexpr Vector_data(T _x, T _y) RESTRICT(cpu) : data{ _x, _y } {}
		constexpr Vector_data(T value) RESTRICT(cpu) : data{ value, value } {}

#if defined(LM_AMP_SUPPORTED)
		Vector_data() RESTRICT(amp) : data{ 0,0 } {}
		Vector_data(T _x, T _y) RESTRICT(amp) : data{ _x, _y } {}
		Vector_data(T value) RESTRICT(amp) : data{ value,value } {}
#endif

		union {
			T data[2];
			struct {
				T x;
				T y;
			};
			struct {
				T u;
				T v;
			};
			struct {
				T r;
				T g;
			};
		};
	};

	template<typename T>
	struct Vector_data<T, 3> {
		constexpr Vector_data() RESTRICT(cpu) : data{ 0,0,0 } {}
		constexpr Vector_data(T _x, T _y, T _z) RESTRICT(cpu) : data{ _x, _y, _z } {}
		constexpr Vector_data(T value) RESTRICT(cpu) : data{ value, value, value }  {}
#if defined(LM_AMP_SUPPORTED)
		Vector_data() RESTRICT(amp) : data{ 0,0,0 } {}
		Vector_data(T _x, T _y, T _z) RESTRICT(amp) : data{ _x, _y, _z } {}
		Vector_data(T value) RESTRICT(amp) : data{ value, value, value } {}
		#endif

		union {
			T data[3];
			Vector<T, 2> xy;
			Vector<T, 2> rg;
			T r;
			struct {
				T x;
				union {
					struct {
						T y;
						T z;
					};
					struct {
						T g;
						T b;
					};
					Vector<T, 2> yz;
					Vector<T, 2> gb;
				};
			};
		};
	};

	template<typename T>
	struct Vector_data<T, 4> {
		constexpr Vector_data() RESTRICT(cpu) : data{ 0,0,0,0 } {}
		constexpr Vector_data(T value) RESTRICT(cpu) : data { value, value, value, value } {}
		constexpr Vector_data(T _x, T _y, T _z, T _w) RESTRICT(cpu) : data{ _x, _y, _z, _w } {}

#if defined(LM_AMP_SUPPORTED)
		Vector_data() RESTRICT(amp) : data{ 0,0,0,0 } {}
		Vector_data(T value) RESTRICT(amp) : data{ value,value,value,value } {}
		Vector_data(T _x, T _y, T _z, T _w) RESTRICT(amp) : data{ _x, _y, _z, _w } {}
#endif
		union {
			T data[4];
			Vector<T, 3> rgb;
			Vector<T, 3> rg;
			Vector<T, 3> xyz;
			Vector<T, 2> xy;
			T r;
			struct {
				T x;
				union {
					T g;
					struct {
						T y;
						union {
							struct {
								T z;
								T w;
							};
							struct {
								T b;
								T a;
							};
							Vector<T, 2> zw;
							Vector<T, 2> ba;
						};
					};
					Vector<T, 2> gb;
					Vector<T, 3> gba;
					Vector<T, 2> yz;
					Vector<T, 3> yzw;
				};
			};
		};
	};

	typedef Vector<float, 2> float2;
	typedef Vector<float, 3> float3;
	typedef Vector<float, 4> float4;
	typedef Vector<double, 2> double2;
	typedef Vector<double, 3> double3;
	typedef Vector<double, 4> double4;
}
#endif // lm_vector_h__
