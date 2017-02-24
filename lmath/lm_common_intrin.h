#ifndef lm_common_intrin_h__
#define lm_common_intrin_h__

#include "lm_vector.h"
#include "lm_matrix.h"
#include "lm_common.h"
#include "lm_quaternion.h"

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

#define LM_PI 3.14159265358979

/*
	template<typename T, typename = std::enable_if<common_traits::is_lm_type<T>::value>>
	auto clamp(const T& v, typename T::element_type min_value, typename T::element_type max_value) RESTRICT(cpu) {
		return transform_copy_helper<T>::execute(v, [min_value, max_value](auto x) {return (std::min)(max_value, (std::max)(x, min_value)); });
	}

	template<typename T> auto degrees(const T& v) RESTRICT(cpu, amp) {
		return transform_copy_helper<T>::execute(v, [](typename lm::common_traits::field_type<T>::type e) { return e * (decltype(e))180 / (decltype(e))LM_PI; });
	}
	*/

	template<typename T>
	static inline Matrix<T, 4, 4> matrix4x4_lookat_lh(const Vector<T, 3>& position, const Vector<T, 3>& target, const Vector<T, 3>& up_direction) {
		Vector<T, 3> forward = lm::normalize(target - position);//res1
		Vector<T, 3> right = lm::normalize(lm::cross(up_direction, forward));//res2
		Vector<T, 3> up = lm::normalize(lm::cross(forward, right));

		return Matrix<T, 4, 4>(
			Vector<T, 4>(right[0], up[0], forward[0], static_cast<T>(0)),
			Vector<T, 4>(right[1], up[1], forward[1], static_cast<T>(0)),
			Vector<T, 4>(right[2], up[2], forward[2], static_cast<T>(0)),
			Vector<T, 4>(-lm::dot(right, position), -lm::dot(up, position), -lm::dot(forward, position), static_cast<T>(1))
		);
	}


	template<typename T>
	static inline Matrix<T, 4, 4> matrix4x4Perspective(T fov, T aspect, T near_clip, T far_clip) {
		Matrix<T, 4, 4> result;

		auto zero = static_cast<T>(0);
		auto one = static_cast<T>(1);
		auto two = static_cast<T>(2);

		auto yScale = one / (lm::tan(fov / two));
		result[0] = Vector<T, 4>(yScale / aspect, zero, zero, zero);
		result[1] = Vector<T, 4>(zero, yScale, zero, zero);
		result[2] = Vector<T, 4>(zero, zero, far_clip / (far_clip - near_clip), one);
		result[3] = Vector<T, 4>(zero, zero, (-near_clip * far_clip) / (far_clip - near_clip), zero);
		return result;
	}

	template<typename T>
	static inline Matrix<T, 4, 4> matrix4x4Rotation(float angle) {
		Matrix<T, 4, 4> matrix;
		auto cos_angle = lm::cos(angle); //num2
		auto sin_angle = lm::sin(angle); //num

		auto zero = static_cast<T>(0);
		auto one = static_cast<T>(1);

		matrix.rows[0] = Vector<T, 4>(cos_angle, zero, -sin_angle, zero);
		matrix.rows[1] = Vector<T, 4>(zero, one, zero, zero);
		matrix.rows[2] = Vector<T, 4>(sin_angle, zero, cos_angle, zero);
		matrix.rows[3] = Vector<T, 4>(zero, zero, zero, one);

		return matrix;
	}


	template<typename T>
	static inline Matrix<T, 4, 4> matrix4x4Rotation(const lm::Quaternion<T>& q) {
		auto num1 = q.x * q.x;
		auto num2 = q.y * q.y;
		auto num3 = q.z * q.z;
		auto num4 = q.x * q.y;
		auto num5 = q.z * q.w;
		auto num6 = q.z * q.x;
		auto num7 = q.y * q.w;
		auto num8 = q.y * q.z;
		auto num9 = q.x * q.w;

		auto zero = static_cast<T>(0);
		auto one = static_cast<T>(1);
		auto two = static_cast<T>(2);
		
		return Matrix<T, 4, 4>(
			Vector<T, 4>(one - two * (num2 + num3), two * (num4 + num5), two * (num6 - num7), zero),
			Vector<T, 4>(two * (num4 - num5), one - two * (num3 + num1), two * (num8 + num9), zero),
			Vector<T, 4>(two * (num6 + num7), two * (num8 - num9), one - two * (num2 + num1), zero),
			Vector<T, 4>(zero, zero, zero, one));
	}

	template<typename T, typename U = T>
	static inline Matrix<U, 4, 4> matrix4x4_scale(T sx, T sy, T sz) {
		return Matrix<U, 4, 4>(
			Vector<U, 4>(sx, 0.0f, 0.0f, 0.0f),
			Vector<U, 4>(0.0f, sy, 0.0f, 0.0f),
			Vector<U, 4>(0.0f, 0.0f, sz, 0.0f),
			Vector<U, 4>(0.0f, 0.0f, 0.0f, 1.0f));
	}

	template<typename T, typename U = T>
	static inline Matrix<U, 4, 4> matrix4x4_scale(const Vector<T, 3>& scale) {
		return matrix4x4_scale<T, U>(scale.x, scale.y, scale.z);
	}

	template<typename T, typename U = T>
	static inline Matrix<U, 4, 4> matrix4x4_translation(T x, T y, T z) {
		return Matrix<U, 4, 4>(
			Vector<U, 4>(1, 0, 0, 0),
			Vector<U, 4>(0, 1, 0, 0),
			Vector<U, 4>(0, 0, 1, 0),
			Vector<U, 4>(x, y, z, 1));
	}

	template<typename T, typename U = T>
	static inline Matrix<U, 4, 4> matrix4x4_translation(const Vector<T, 3>& position) {
		return matrix4x4_translation<T, U>(position.x, position.y, position.z);
	}
}
#endif // lm_common_intrin_h__
