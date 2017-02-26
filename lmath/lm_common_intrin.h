#ifndef lm_common_intrin_h__
#define lm_common_intrin_h__

#include "lm_vector.h"
#include "lm_matrix.h"
#include "lm_common.h"
#include "lm_quaternion.h"
#include "lm_types.h"

#include <algorithm>
#include <cmath>
#include <type_traits>

namespace lm {

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

	template<typename T>
	static inline Matrix<T, 4, 4> matrix4x4LookatLh(const Vector<T, 3>& position, const Vector<T, 3>& target, const Vector<T, 3>& up_direction) {
		auto forward = lm::normalize(target - position);
		auto right = lm::normalize(lm::cross(up_direction, forward));
		auto up = lm::normalize(lm::cross(forward, right));

		auto zero = DefaultValues<T>::zero();
		auto one = DefaultValues<T>::one();

		return Matrix<T, 4, 4>{
			Vector<T, 4>(right, -lm::dot(right, position)),
			Vector<T, 4>(up, -lm::dot(up, position)),
			Vector<T, 4>(forward, -lm::dot(forward, position)),
			Vector<T, 4>(zero, zero, zero, one)
		};
	}

	template<typename T>
	static inline Matrix<T, 4, 4> matrix4x4Perspective(T fov, T aspect, T near_clip, T far_clip) {
		auto zero = DefaultValues<T>::zero();
		auto one = DefaultValues<T>::one();
		auto two = DefaultValues<T>::two();
		auto yScale = one / (lm::tan(fov / two));
		return Matrix<T, 4, 4>(
			Vector<T, 4>(yScale / aspect, zero, zero, zero),
			Vector<T, 4>(zero, yScale, zero, zero),
			Vector<T, 4>(zero, zero, far_clip / (far_clip - near_clip), (-near_clip * far_clip) / (far_clip - near_clip)),
			Vector<T, 4>(zero, zero, one, zero));
	}

	template<typename T>
	static inline Matrix<T, 4, 4> matrix4x4RotationY(float angleInRadians) {
		auto cosAngle = lm::cos(angleInRadians);
		auto sinAngle = lm::sin(angleInRadians);

		auto zero = DefaultValues<T>::zero();
		auto one = DefaultValues<T>::one();

		return Matrix<T, 4, 4>(
			Vector<T, 4>(cosAngle, zero, sinAngle, zero),
			Vector<T, 4>(zero, one, zero, zero),
			Vector<T, 4>(-sinAngle, zero, cosAngle, zero),
			Vector<T, 4>(zero, zero, zero, one));;
	}


	template<typename T>
	static inline Matrix<T, 4, 4> matrix4x4RotationY(const lm::Quaternion<T>& q) {
		auto num1 = q.x * q.x;
		auto num2 = q.y * q.y;
		auto num3 = q.z * q.z;
		auto num4 = q.x * q.y;
		auto num5 = q.z * q.w;
		auto num6 = q.z * q.x;
		auto num7 = q.y * q.w;
		auto num8 = q.y * q.z;
		auto num9 = q.x * q.w;

		auto zero = DefaultValues<T>::zero();
		auto one  = DefaultValues<T>::one();
		auto two  = DefaultValues<T>::two();
		
		return Matrix<T, 4, 4>(
			Vector<T, 4>(one - two * (num2 + num3), two * (num4 - num5), two * (num6 + num7), zero),
			Vector<T, 4>(two * (num4 + num5), one - two * (num3 + num1), two * (num8 - num9), zero),
			Vector<T, 4>(two * (num6 - num7), two * (num8 + num9), one - two * (num2 + num1), zero),
			Vector<T, 4>(zero, zero, zero, one));
	}

	template<typename T, typename U = T>
	static inline Matrix<U, 4, 4> matrix4x4Scale(T sx, T sy, T sz) {

		auto zero = DefaultValues<T>::zero();
		auto one = DefaultValues<T>::one();

		return Matrix<U, 4, 4>(
			Vector<U, 4>(sx, zero, zero, zero),
			Vector<U, 4>(zero, sy, zero, zero),
			Vector<U, 4>(zero, zero, sz, zero),
			Vector<U, 4>(zero, zero, zero, one));
	}

	template<typename T, typename U = T>
	static inline Matrix<U, 4, 4> matrix4x4Scale(const Vector<T, 3>& scale) {
		return matrix4x4Scale<T, U>(scale.x, scale.y, scale.z);
	}

	template<typename T, typename U = T>
	static inline Matrix<U, 4, 4> matrix4x4Translation(T x, T y, T z) {
		auto zero = DefaultValues<T>::zero();
		auto one = DefaultValues<T>::one();

		return Matrix<U, 4, 4>(
			Vector<U, 4>(one, zero, zero, x),
			Vector<U, 4>(zero, one, zero, y),
			Vector<U, 4>(zero, zero, one, z),
			Vector<U, 4>(zero, zero, zero, one));
	}

	template<typename T, typename U = T>
	static inline Matrix<U, 4, 4> matrix4x4Translation(const Vector<T, 3>& position) {
		return matrix4x4Translation<T, U>(position.x, position.y, position.z);
	}
}
#endif // lm_common_intrin_h__
