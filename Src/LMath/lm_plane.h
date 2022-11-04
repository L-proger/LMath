#ifndef lm_plane_h__
#define lm_plane_h__

#include "lm_vector.h"

namespace lm
{
	template<typename T>
	struct Plane {
		union{
			T data[4];
			struct
			{
				lm::Vector<T, 3> normal;
				T d;
			};
		};

		Plane() : normal(0,1,0), d(0) {}

		Plane(T _a, T _b, T _c, T _d) : normal(_a,_b,_c), d(_d) {}

		Plane(Vector<T,3>& _normal, float _d) : normal(_normal), d(_d) {}
		
		Plane& operator=(const Plane& v) {
			normal = v.normal;
			d = v.d;
			return *this;
		}
	};
}
#endif // lm_plane_h__
