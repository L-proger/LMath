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
			Vector<T, 4>(right.x, up.x, forward.x, static_cast<T>(0)),
			Vector<T, 4>(right.y, up.y, forward.y, static_cast<T>(0)),
			Vector<T, 4>(right.z, up.z, forward.z, static_cast<T>(0)),
			Vector<T, 4>(-lm::dot(right, position), -lm::dot(up, position), -lm::dot(forward, position), static_cast<T>(1))
		);
	}


	template<typename T>
	static inline Matrix<T, 4, 4> matrix4x4_perspective(T fov, T aspect, T near_clip, T far_clip) {
		Matrix<T, 4, 4> result;
		float yScale = static_cast<T>(1) / (std::tan(fov / static_cast<T>(2)));
		result.rows[0] = Vector<T, 4>(yScale / aspect, 0, 0, 0);
		result.rows[1] = Vector<T, 4>(0, yScale, 0, 0);
		result.rows[2] = Vector<T, 4>(0, 0, far_clip / (far_clip - near_clip), 1);
		result.rows[3] = Vector<T, 4>(0, 0, (-near_clip * far_clip) / (far_clip - near_clip), 0);
		return result;
	}

	template<typename T>
	static inline Matrix<T, 4, 4> matrix4x4_rotation(float angle) {
		Matrix<T, 4, 4> matrix;
		auto cos_angle = std::cos(angle); //num2
		auto sin_angle = std::sin(angle); //num

		auto zero = static_cast<T>(0);
		auto one = static_cast<T>(0);

		matrix.rows[0] = Vector<T, 4>(cos_angle, 0, -sin_angle, 0);
		matrix.rows[1] = Vector<T, 4>(0,1,0,0);
		matrix.rows[2] = Vector<T, 4>(sin_angle, 0, cos_angle, 0);
		matrix.rows[3] = Vector<T, 4>(0,0,0,1);

		return matrix;
	}


	template<typename T>
	static inline Matrix<T, 4, 4> matrix4x4_rotation(const lm::Quaternion<T>& q) {
		float num1 = q.x * q.x;
		float num2 = q.y * q.y;
		float num3 = q.z * q.z;
		float num4 = q.x * q.y;
		float num5 = q.z * q.w;
		float num6 = q.z * q.x;
		float num7 = q.y * q.w;
		float num8 = q.y * q.z;
		float num9 = q.x * q.w;
		
		return Matrix<T, 4, 4>(
			Vector<T, 4>(1.0f - 2.0f * (num2 + num3), 2.0f * (num4 + num5), 2.0f * (num6 - num7), 0),
			Vector<T, 4>(2.0f * (num4 - num5), 1.0f - 2.0f * (num3 + num1), 2.0f * (num8 + num9), 0),
			Vector<T, 4>(2.0f * (num6 + num7), 2.0f * (num8 - num9), 1.0f - 2.0f * (num2 + num1), 0),
			Vector<T, 4>(0,0,0,1));
	}

	template<typename T, typename U = T>
	static inline Matrix<U, 4, 4> matrix4x4_scale(T sx, T sy, T sz) {
		return Matrix<U, 4, 4>(
			Vector<U, 4>(sx, 0, 0, 0),
			Vector<U, 4>(0, sy, 0, 0),
			Vector<U, 4>(0, 0, sz, 0),
			Vector<U, 4>(0, 0, 0, 1));
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
