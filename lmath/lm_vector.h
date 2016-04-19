#ifndef lm_vector_h__
#define lm_vector_h__

#include <type_traits>
#include <array>

namespace lm {
	struct Vector_base {

	};

	template<typename T, size_t Size>
	struct Vector_data {
		std::array<T, Size> data;
	};

#include "lm_vector_traits.h"

	template<typename T, size_t Size>
	struct Vector : public Vector_base, public Vector_data<T, Size> {
	public:
		typedef size_t size_type;
		static constexpr size_type size = Size;
		
		typedef T element_type;

		constexpr Vector() {
		}

		constexpr Vector(T value) : Vector_data<T, Size>(value) {

		}

		template<typename = std::enable_if<Size == 2>::type>
		constexpr Vector(T x, T y) : Vector_data<T, Size>(x, y) {

		}

		template<typename = std::enable_if<Size == 3>::type>
		constexpr Vector(T x, T y, T z) : Vector_data<T, Size>(x, y, z) {

		}

		template<typename = std::enable_if<Size == 4>::type>
		constexpr Vector(T x, T y, T z, T w) : Vector_data<T, Size>(x, y, z, w) {

		}

		template<typename = std::enable_if<Size == 3>::type>
		static constexpr Vector right() {
			return Vector(1, 0, 0);
		}

		template<typename = std::enable_if<Size == 3>::type>
		static constexpr Vector up() {
			return Vector(0, 1, 0);
		}

		template<typename = std::enable_if<Size == 3>::type>
		static constexpr Vector forward() {
			return Vector(0, 0, 1);
		}

		//op add
		template<typename T, typename = std::enable_if<!vector_traits::is_vector<T>::value>::type>
		auto operator+(T value) {
			Vector<vector_traits::field_add_t<Vector, T>, Size> result;
			for (size_t i = 0; i < Size; ++i) {
				result.data[i] = data[i] + value;
			}
			return result;
		}

		template<typename T, typename = std::enable_if<vector_traits::is_same_extent<T, Vector>::value>::type>
		auto operator+(const T& value) {
			Vector<vector_traits::field_add_t<Vector, T>, Size> result;
			for (size_t i = 0; i < Size; ++i) {
				result.data[i] = data[i] + value.data[i];
			}
			return result;
		}


		//op sub
		template<typename T, typename = std::enable_if<!vector_traits::is_vector<T>::value>::type>
		auto operator-(T value) {
			Vector<vector_traits::field_sub_t<Vector, T>, Size> result;
			for (size_t i = 0; i < Size; ++i) {
				result.data[i] = data[i] - value;
			}
			return result;
		}

		template<typename T, typename = std::enable_if<vector_traits::is_same_extent<T, Vector>::value>::type>
		auto operator-(const T& value) {
			Vector<vector_traits::field_sub_t<Vector, T>, Size> result;
			for (size_t i = 0; i < Size; ++i) {
				result.data[i] = data[i] - value.data[i];
			}
			return result;
		}

		//op mul
		template<typename T, typename = std::enable_if<!vector_traits::is_vector<T>::value>::type>
		auto operator*(T value) {
			Vector<vector_traits::field_mul_t<Vector, T>, Size> result;
			for (size_t i = 0; i < Size; ++i) {
				result.data[i] = data[i] * value;
			}
			return result;
		}

		template<typename T, typename = std::enable_if<vector_traits::is_same_extent<T, Vector>::value>::type>
		auto operator *(const T& value) {
			Vector<vector_traits::field_mul_t<Vector, T>, Size> result;
			for (size_t i = 0; i < Size; ++i) {
				result.data[i] = data[i] * value.data[i];
			}
			return result;
		}

		//op div
		template<typename T, typename = std::enable_if<!vector_traits::is_vector<T>::value>::type>
		auto operator/(T value) {
			Vector<vector_traits::field_div_t<Vector, T>, Size> result;
			for (size_t i = 0; i < Size; ++i) {
				result.data[i] = data[i] / value;
			}
			return result;
		}

		template<typename T, typename = std::enable_if<vector_traits::is_same_extent<T, Vector>::value>::type>
		auto operator /(const T& value) {
			Vector<vector_traits::field_div_t<Vector, T>, Size> result;
			for (size_t i = 0; i < Size; ++i) {
				result.data[i] = data[i] / value.data[i];
			}
			return result;
		}


		Vector& operator=(const Vector& value) {
			for (size_t i = 0; i < Size; ++i) {
				data[i] = value.data[i];
			}
			return *this;
		}
	};

	template<typename T>
	struct Vector_data<T, 1> {
		union {
			std::array<T, 1> data;
			T x;
		};
	};

	template<typename T>
	struct Vector_data<T, 2> {
		constexpr Vector_data() :data{ 0,0 } {

		}
		constexpr Vector_data(T _x, T _y) : data{ _x, _y } {

		}
		constexpr Vector_data(T value) : data{ value,value } {

		}
		union {
			std::array<T, 2> data;
			struct {
				T x;
				T y;
			};
		};
	};

	template<typename T>
	struct Vector_data<T, 3> {
		constexpr Vector_data() :data{ 0,0,0 } {

		}
		constexpr Vector_data(T _x, T _y, T _z) : data{ _x, _y, _z } {

		}
		constexpr Vector_data(T value) : data{ value,value,value } {

		}
		union {
			std::array<T, 3> data;
			Vector<T, 2> xy;
			struct {
				T x;
				union {
					struct {
						T y;
						T z;
					};
					Vector<T, 2> yz;
				};
			};
		};
	};

	template<typename T>
	struct Vector_data<T, 4> {
		constexpr Vector_data() :data{ 0,0,0,0 } {

		}
		constexpr Vector_data(T value) : data{ value,value,value,value } {

		}
		constexpr Vector_data(T _x, T _y, T _z, T _w) : data{ _x, _y, _z, _w } {

		}
		union {
			std::array<T, 4> data;
			Vector<T, 3> xyz;
			Vector<T, 2> xy;
			struct {
				T x;
				union {
					struct {
						T y;
						union {
							struct {
								T z;
								T w;
							};
							Vector<T, 2> zw;
						};
					};
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
