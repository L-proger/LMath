#ifndef lm_plane_intrin_h__
#define lm_plane_intrin_h__

#include "lm_plane.h"

namespace lm
{
	template<typename T, typename U>
	static inline auto dot(const Plane<T>& plane, const Vector<U, 4>& value) {
		return ((((plane.normal.x() * value.x()) + (plane.normal.y() * value.y())) + (plane.normal.z() * value.z())) + (plane.d * value.w()));
	}

	/*static inline float PlaneDot(const Plane& plane, const Vector4D& value) {
		return ((((plane.Normal.X * value.X) + (plane.Normal.Y * value.Y)) + (plane.Normal.Z * value.Z)) + (plane.D * value.W));
	}
	static inline float PlaneDotCoordinate(const Plane& plane, const Vector3D& value) {
		return ((((plane.Normal.X * value.X) + (plane.Normal.Y * value.Y)) + (plane.Normal.Z * value.Z)) + plane.D);
	}

	static inline float PlaneDotNormal(const Plane& plane, const Vector3D& value) {
		return (((plane.Normal.X * value.X) + (plane.Normal.Y * value.Y)) + (plane.Normal.Z * value.Z));
	}
	static inline float PlaneDot(const Plane& plane, const Vector3D& point) {
		return ((((plane.Normal.Y * point.Y) + (plane.Normal.X * point.X)) + (plane.Normal.Z * point.Z)) + plane.D);
	}*/


	/*static inline float PlaneDotCoordinate(const Plane& plane, const Vector3D& value) {
		return ((((plane.Normal.X * value.X) + (plane.Normal.Y * value.Y)) + (plane.Normal.Z * value.Z)) + plane.D);
	}
	static inline float PlaneDotNormal(const Plane& plane, const Vector3D& value) {
		return (((plane.Normal.X * value.X) + (plane.Normal.Y * value.Y)) + (plane.Normal.Z * value.Z));
	}*/
	template<typename T, typename U>
	static inline auto dot(const Plane<T>& plane, const Vector<U, 3>& point) {
		return ((((plane.normal.y * point.y) + (plane.normal.x * point.x)) + (plane.normal.z * point.z)) + plane.D);
	}
}
#endif // lm_plane_intrin_h__
