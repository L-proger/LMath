#ifndef lm_quaternion_intrin_h__
#define lm_quaternion_intrin_h__

#include "lm_quaternion.h"

namespace lm
{
	template<typename Q1, typename Q2>
	auto mul(const lm::Quaternion<Q1>& q1, const lm::Quaternion<Q2>& q2) {
		return lm::Quaternion<decltype(q1.x() * q2.x())>(
			(q1.w() * q2.x() + q1.x() * q2.w() + q1.y() * q2.z() - q1.z() * q2.y()),
			(q1.w() * q2.y() + q1.y() * q2.w() + q1.z() * q2.x() - q1.x() * q2.z()),
			(q1.w() * q2.z() + q1.z() * q2.w() + q1.x() * q2.y() - q1.y() * q2.x()),
			(q1.w() * q2.w() - q1.x() * q2.x() - q1.y() * q2.y() - q1.z() * q2.z())
		);
	}
}
#endif // lm_quaternion_intrin_h__
