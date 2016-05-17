#ifndef lm_quaternion_h__
#define lm_quaternion_h__

#include <cmath>
#include "lm_vector_intrin.h"


namespace lm {
	template<typename T>
	struct Quaternion {
		union{
			T data[4];
			struct{
				T x;
				T y;
				T z;
				T w;
			};
		};

		Quaternion() : data{0,0,0,0} {}

		Quaternion(T _x, T _y, T _z, T _w) : data{ _x,_y,_z,_w } {}

		static Quaternion angle_axis(T angle, Vector<T, 3> axis) {
			T theta = angle / static_cast<T>(2);
			T s = static_cast<T>(std::sin(theta) / lm::length(axis));
			T c = static_cast<T>(std::cos(theta));

			return Quaternion(
				axis.x * s,
				axis.y * s,
				axis.z * s, c);
		}
	};

	typedef Quaternion<float> Quaternion_f;
}
#endif // lm_quaternion_h__
