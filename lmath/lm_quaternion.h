#ifndef lm_quaternion_h__
#define lm_quaternion_h__

#include "lm_vector.h"

namespace lm {
	template<typename T>
	struct Quaternion {
		T data[4];

		constexpr Quaternion() {}

		constexpr Quaternion(T x, T y, T z, T w) : data{ x,y,z,w } {}

		static Quaternion angleAxis(T angle, const Vector<T, 3>& axis) {
			auto theta = angle / DefaultValues<T>::two();
			auto s = lm::sin(theta);
			auto c = lm::cos(theta);

			return Quaternion(
				axis[0] * s,
				axis[1] * s,
				axis[2] * s, c);
		}

		static Quaternion identity() {
			auto zero = DefaultValues<T>::zero();
			auto one = DefaultValues<T>::one();
			return Quaternion(zero, zero, zero, one);
		}

		T normSquare() const{
			return data[0] * data[0] + data[1] * data[1] + data[2] * data[2] + data[3] * data[3];
		}

		T norm() const {
			return lm::sqrt(normSquare());
		}


		void conjugate() const {
			for (LmSize i = 0; i < 3; ++i) {
				data[i] = -data[i];
			}
		}
		void inverse() {
			auto n = static_cast<T>(-1) / normSquare();
			data[0] = data[0] * n;
			data[1] = data[1] * n;
			data[2] = data[2] * n;
			data[3] = data[3] * -n;
		}


		Quaternion operator -() const {
			return Quaternion(-data[0], -data[1], -data[2], data[3]);
		}

		bool operator==(const Quaternion& a) const{
			return
				data[0] == d.data[0] &&
				data[1] == d.data[1] &&
				data[2] == d.data[2] &&
				data[3] == d.data[3];
		}

		bool operator!=(const Quaternion& a) const {
			return
				data[0] != d.data[0] ||
				data[1] != d.data[1] ||
				data[2] != d.data[2] ||
				data[3] != d.data[3];
		}


		Quaternion operator *(const Quaternion& r) const {
			Quaternion result;
			result[3] = r[3] * data[3] - r[0] * data[0] - r[1] * data[1] - r[2] * data[2];
			result[0] = r[3] * data[0] + r[0] * data[3] - r[1] * data[2] + r[2] * data[1];
			result[1] = r[3] * data[1] + r[0] * data[2] + r[1] * data[3] - r[2] * data[0];
			result[2] = r[3] * data[2] - r[0] * data[1] + r[1] * data[0] + r[2] * data[3];
			return result;
		}

		T& operator[](LmSize id) {
			return data[id];
		}
		const T& operator[](LmSize id) const {
			return data[id];
		}
	};

	template<typename T>
	inline auto dot(const Quaternion<T>& a, const Quaternion<T>& b) {
		return a[0]*b[0] + a[1]*b[1] + a[2]*b[2] + a[3]*b[3];
	}


	template<typename T>
	inline Quaternion<T> lerp(const Quaternion<T>& a, const Quaternion<T>& b, const T& x) {
		return Quaternion<T>(
			a[0] + (b[0] - a[0]) * x,
			a[1] + (b[1] - a[1]) * x,
			a[2] + (b[2] - a[2]) * x,
			a[3] + (b[3] - a[3]) * x);
	}

	template<typename T>
	inline Quaternion<T> slerp(const Quaternion<T>& a, const Quaternion<T>& b, const T& x) {
		auto cosom = dot(a, b);

		if ((DefaultValues<T>::one() + cosom) > std::numeric_limits<T>::epsilon()) {
			T sp;
			T sq;
			if ((DefaultValues<T>::one() - cosom) > std::numeric_limits<T>::epsilon()) {
				double omega = lm::acos(cosom);
				double sinom = DefaultValues<T>::one() / lm::sin(omega);

				sp = static_cast<T>(sin((DefaultValues<T>::one() - x) * omega) * sinom);
				sq = static_cast<T>(sin(x * omega) * sinom);
			}
			else {
				sp = DefaultValues<T>::one() - x;
				sq = x;
			}
			return Quaternion<T>(
				a[0] * sp + b[0] * sq,
				a[1] * sp + b[1] * sq,
				a[2] * sp + b[2] * sq,
				a[3] * sp + b[3] * sq);
		}
		else {
			auto halfpi = static_cast<T>(lm:pi_d / DefaultValues<T>::two()); //TODO: cleanup types mess
			auto sp = static_cast<T>(lm::sin((DefaultValues<T>::one() - x) * halfpi));
			auto sq = static_cast<T>(lm::sin(x * halfpi));
			return Quaternion<T>(
				a[0] * sp - a[1] * sq,
				a[1] * sp + a[0] * sq,
				a[2] * sp - a[3] * sq,
				a[2]);
		}
	}

	typedef Quaternion<float> Quaternion_f;
}
#endif // lm_quaternion_h__
