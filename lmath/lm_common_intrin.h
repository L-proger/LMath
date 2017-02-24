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
		Vector<T, 3> forward = lm::normalize(target - position);//res1
		Vector<T, 3> right = lm::normalize(lm::cross(up_direction, forward));//res2
		Vector<T, 3> up = lm::normalize(lm::cross(forward, right));

		return Matrix<T, 4, 4>(
			Vector<T, 4>(right[0], right[1], right[2], -lm::dot(right, position)),
			Vector<T, 4>(up[0], up[1], up[2], -lm::dot(up, position)),
			Vector<T, 4>(forward[0], forward[1], forward[2], -lm::dot(forward, position)),
			Vector<T, 4>(static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(1))
		);
	}

	template<typename T>
	static inline Matrix<T, 4, 4> matrix4x4Perspective(T fov, T aspect, T near_clip, T far_clip) {
		Matrix<T, 4, 4> result;

		auto yScale = one<T> / (lm::tan(fov / two<T>));
		result[0] = Vector<T, 4>(yScale / aspect, zero<T>, zero<T>, zero<T>);
		result[1] = Vector<T, 4>(zero<T>, yScale, zero<T>, zero<T>);
		result[2] = Vector<T, 4>(zero<T>, zero<T>, far_clip / (far_clip - near_clip), (-near_clip * far_clip) / (far_clip - near_clip));
		result[3] = Vector<T, 4>(zero<T>, zero<T>, one<T>, zero<T>);
		return result;
	}

	template<typename T>
	static inline Matrix<T, 4, 4> matrix4x4Rotation(float angle) {
		Matrix<T, 4, 4> matrix;
		auto cos_angle = lm::cos(angle); //num2
		auto sin_angle = lm::sin(angle); //num

		matrix.rows[0] = Vector<T, 4>(cos_angle, zero<T>, sin_angle, zero<T>);
		matrix.rows[1] = Vector<T, 4>(zero<T>, one<T>, zero<T>, zero<T>);
		matrix.rows[2] = Vector<T, 4>(-sin_angle, zero<T>, cos_angle, zero<T>);
		matrix.rows[3] = Vector<T, 4>(zero<T>, zero<T>, zero<T>, one<T>);

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
		
		return Matrix<T, 4, 4>(
			Vector<T, 4>(one<T> - two<T> * (num2 + num3), two<T> * (num4 - num5), two<T> * (num6 + num7), zero<T>),
			Vector<T, 4>(two<T> * (num4 + num5), one<T> - two<T> * (num3 + num1), two<T> * (num8 - num9), zero<T>),
			Vector<T, 4>(two<T> * (num6 - num7), two<T> * (num8 + num9), one<T> - two<T> * (num2 + num1), zero<T>),
			Vector<T, 4>(zero<T>, zero<T>, zero<T>, one<T>));
	}

	template<typename T, typename U = T>
	static inline Matrix<U, 4, 4> matrix4x4Scale(T sx, T sy, T sz) {
		return Matrix<U, 4, 4>(
			Vector<U, 4>(sx, zero<T>, zero<T>, zero<T>),
			Vector<U, 4>(zero<T>, sy, zero<T>, zero<T>),
			Vector<U, 4>(zero<T>, zero<T>, sz, zero<T>),
			Vector<U, 4>(zero<T>, zero<T>, zero<T>, one<T>));
	}

	template<typename T, typename U = T>
	static inline Matrix<U, 4, 4> matrix4x4Scale(const Vector<T, 3>& scale) {
		return matrix4x4Scale<T, U>(scale.x, scale.y, scale.z);
	}

	template<typename T, typename U = T>
	static inline Matrix<U, 4, 4> matrix4x4Translation(T x, T y, T z) {
		return Matrix<U, 4, 4>(
			Vector<U, 4>(one<T>, zero<T>, zero<T>, x),
			Vector<U, 4>(zero<T>, one<T>, zero<T>, y),
			Vector<U, 4>(zero<T>, zero<T>, one<T>, z),
			Vector<U, 4>(zero<T>, zero<T>, zero<T>, one<T>));
	}

	template<typename T, typename U = T>
	static inline Matrix<U, 4, 4> matrix4x4Translation(const Vector<T, 3>& position) {
		return matrix4x4Translation<T, U>(position.x, position.y, position.z);
	}
}
#endif // lm_common_intrin_h__
