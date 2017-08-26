#ifndef lm_vector_sse_h__
#define lm_vector_sse_h__

#include "lm_vector.h"
#include <intrin.h>


namespace lm {

	template<>
	struct VectorData<float, 4, InstructionSet::SSE> {
		
		VectorData() {
			sseValue = _mm_setzero_ps();
		}

		VectorData(float value) {
			sseValue = _mm_set_ps1(value);
		}

		VectorData(float x, float y, float z, float w) {
			sseValue = _mm_set_ps(w, z, y, x);
		}

		VectorData(__m128 value) : sseValue(value){
		}


		template<LmSize Index>
		float get() const {
			static_assert(Index < 4);
			if constexpr(Index == 0) {
				return _mm_cvtss_f32(sseValue);
			}
			else if constexpr(Index == 1) {
				return _mm_cvtss_f32(_mm_shuffle_ps(sseValue, sseValue, _MM_SHUFFLE(1, 1, 1, 1)));
			}
			else if constexpr(Index == 2) {
				return _mm_cvtss_f32(_mm_shuffle_ps(sseValue, sseValue, _MM_SHUFFLE(2, 2, 2, 2)));
			}
			else constexpr {
				return _mm_cvtss_f32(_mm_shuffle_ps(sseValue, sseValue, _MM_SHUFFLE(3, 3, 3, 3)));
			}
		}

		float get(LmSize id) const {
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
		
		template<LmSize Index>
		void set(float value) {
			static_assert(Index < 4);
			if constexpr(Index == 0) {
				sseValue = _mm_move_ss(sseValue, _mm_set_ss(value));
			}
			if constexpr(Index == 1) {
				auto temp = _mm_move_ss(sseValue, _mm_set_ss(value));
				temp = _mm_shuffle_ps(temp, temp, _MM_SHUFFLE(3, 2, 0, 0));
				sseValue = _mm_move_ss(temp, sseValue);
			}
			if constexpr(Index == 2) {
				auto temp = _mm_move_ss(sseValue, _mm_set_ss(value));
				temp = _mm_shuffle_ps(temp, temp, _MM_SHUFFLE(3, 0, 1, 0));
				sseValue = _mm_move_ss(temp, sseValue);
			}
			if constexpr(Index == 3) {
				auto temp = _mm_move_ss(sseValue, _mm_set_ss(value));
				temp = _mm_shuffle_ps(temp, temp, _MM_SHUFFLE(0, 2, 1, 0));
				sseValue = _mm_move_ss(temp, sseValue);
			}
		}

		void set(LmSize id, float value) {
			if (id == 0) {
				set<0>(value);
			}
			else if (id == 1) {
				set<1>(value);
			}
			else if (id == 2) {
				set<2>(value);
			}
			else {
				set<3>(value);
			}
		}

		auto mask() const { 
			return _mm_movemask_ps(sseValue) & 0x7;
		}

		__m128 sseValue;
	};

	using float4_sse = Vector<float, 4, InstructionSet::SSE>;

	float4_sse operator * (float4_sse l, float4_sse r) { return { _mm_mul_ps(l.sseValue, r.sseValue) }; }
	float4_sse operator / (float4_sse l, float4_sse r) { return { _mm_div_ps(l.sseValue, r.sseValue) }; }
	float4_sse operator + (float4_sse l, float4_sse r) { return { _mm_add_ps(l.sseValue, r.sseValue) }; }
	float4_sse operator - (float4_sse l, float4_sse r) { return { _mm_sub_ps(l.sseValue, r.sseValue) }; }

	float4_sse& operator *= (float4_sse& l, float4_sse r) { l = l * r; return l; }
	float4_sse& operator /= (float4_sse& l, float4_sse r) { l = l / r; return l; }
	float4_sse& operator += (float4_sse& l, float4_sse r) { l = l + r; return l; }
	float4_sse& operator -= (float4_sse& l, float4_sse r) { l = l - r; return l; }

	float4_sse operator == (float4_sse l, float4_sse r) { return { _mm_cmpeq_ps(l.sseValue, r.sseValue) }; }
	float4_sse operator != (float4_sse l, float4_sse r) { return { _mm_cmpneq_ps(l.sseValue, r.sseValue) }; }
	float4_sse operator <  (float4_sse l, float4_sse r) { return { _mm_cmplt_ps(l.sseValue, r.sseValue) }; }
	float4_sse operator <= (float4_sse l, float4_sse r) { return { _mm_cmple_ps(l.sseValue, r.sseValue) }; }
	float4_sse operator >  (float4_sse l, float4_sse r) { return { _mm_cmpgt_ps(l.sseValue, r.sseValue) }; }
	float4_sse operator >= (float4_sse l, float4_sse r) { return { _mm_cmpge_ps(l.sseValue, r.sseValue) }; }

	float4_sse min(float4_sse l, float4_sse r) { return {_mm_min_ps(l.sseValue, r.sseValue)}; }
	float4_sse max(float4_sse l, float4_sse r) { return {_mm_max_ps(l.sseValue, r.sseValue)}; }

	float min(float4_sse l) { 
		auto ml = _mm_min_ps(l.sseValue, _mm_shuffle_ps(l.sseValue, l.sseValue, _MM_SHUFFLE(3, 2, 3, 2)));
		return _mm_cvtss_f32(_mm_min_ps(ml, _mm_shuffle_ps(ml, ml, _MM_SHUFFLE(1, 1, 1, 1))));
	}

	float max(float4_sse l) {
		auto ml = _mm_max_ps(l.sseValue, _mm_shuffle_ps(l.sseValue, l.sseValue, _MM_SHUFFLE(3, 2, 3, 2)));
		return _mm_cvtss_f32(_mm_max_ps(ml, _mm_shuffle_ps(ml, ml, _MM_SHUFFLE(1, 1, 1, 1))));
	}

	float sum(float4_sse v) {
		//SSE3 code
		auto tmp = _mm_hadd_ps(v.sseValue, v.sseValue);
		return _mm_cvtss_f32(_mm_hadd_ps(tmp, tmp));
	}

	float dot(float4_sse l, float4_sse r) {
		//SSE4 code
		return _mm_cvtss_f32(_mm_dp_ps(l.sseValue, r.sseValue, 0xff));
	}

	//_mm_dp_ps
}

#endif // lm_vector_sse_h__
