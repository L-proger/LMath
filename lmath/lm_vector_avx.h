#ifndef lm_vector_avx_h__
#define lm_vector_avx_h__

#include "lm_vector.h"
#include <intrin.h>


namespace lm {

	template<>
	struct VectorData<double, 4, InstructionSet::AVX> {
		VectorData() {
			avxValue = _mm256_setzero_pd();
		}

		VectorData(__m256d value) : avxValue(value) {

		}
		VectorData(double value) {
			avxValue = _mm256_set_pd(value, value, value, value);
		}

		VectorData(double x, double y, double z, double w) {
			avxValue = _mm256_set_pd(w, z, y, x);
		}


		template<LmSize Index>
		double get() const {
			static_assert(Index < 4);
			alignas(256) double buffer[4];
			_mm256_store_pd(&buffer[0], avxValue);
			return buffer[Index];
		}
		double get(LmSize id) const {
			if (id == 0) {
				return get<0>();
			}
			else if (id == 1) {
				return get<1>();
			}
			else if (id == 2) {
				return get<2>();
			}
			else {
				return get<3>();
			}
		}

		__m256d avxValue;
	};

	using double4_avx = Vector<double, 4, InstructionSet::AVX>;

	double dot(double4_avx l, double4_avx r) {
		//AVX code

		auto m = _mm256_mul_pd(l.avxValue, r.avxValue);
		auto a0 = _mm256_hadd_pd(m, m);
		auto a1 = _mm256_permute4x64_pd(a0, _MM_SHUFFLE(1, 3, 1, 3));
		return _mm_cvtsd_f64(_mm256_castpd256_pd128(_mm256_hadd_pd(a1,a1)));
	}


	double4_avx operator * (double4_avx l, double4_avx r) { return { _mm256_mul_pd(l.avxValue, r.avxValue) }; }
	double4_avx operator / (double4_avx l, double4_avx r) { return { _mm256_div_pd(l.avxValue, r.avxValue) }; }
	double4_avx operator + (double4_avx l, double4_avx r) { return { _mm256_add_pd(l.avxValue, r.avxValue) }; }
	double4_avx operator - (double4_avx l, double4_avx r) { return { _mm256_sub_pd(l.avxValue, r.avxValue) }; }

	double4_avx& operator *= (double4_avx& l, double4_avx r) { l = l * r; return l; }
	double4_avx& operator /= (double4_avx& l, double4_avx r) { l = l / r; return l; }
	double4_avx& operator += (double4_avx& l, double4_avx r) { l = l + r; return l; }
	double4_avx& operator -= (double4_avx& l, double4_avx r) { l = l - r; return l; }

	double4_avx operator == (double4_avx l, double4_avx r) { return { _mm256_cmp_pd(l.avxValue, r.avxValue, _CMP_EQ_OQ) }; }
	double4_avx operator != (double4_avx l, double4_avx r) { return { _mm256_cmp_pd(l.avxValue, r.avxValue, _CMP_NEQ_OQ) }; }
	double4_avx operator <  (double4_avx l, double4_avx r) { return { _mm256_cmp_pd(l.avxValue, r.avxValue, _CMP_LT_OQ) }; }
	double4_avx operator <= (double4_avx l, double4_avx r) { return { _mm256_cmp_pd(l.avxValue, r.avxValue, _CMP_LE_OQ) }; }
	double4_avx operator >  (double4_avx l, double4_avx r) { return { _mm256_cmp_pd(l.avxValue, r.avxValue, _CMP_GT_OQ) }; }
	double4_avx operator >= (double4_avx l, double4_avx r) { return { _mm256_cmp_pd(l.avxValue, r.avxValue, _CMP_GE_OQ) }; }

	double4_avx min(double4_avx l, double4_avx r) { return { _mm256_min_pd(l.avxValue, r.avxValue) }; }
	double4_avx max(double4_avx l, double4_avx r) { return { _mm256_max_pd(l.avxValue, r.avxValue) }; }

}
#endif // lm_vector_avx_h__
