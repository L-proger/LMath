#ifndef lm_vector_sse_h__
#define lm_vector_sse_h__

#include "lm_vector.h"
#include <intrin.h>


namespace lm {

	template<>
	struct VectorData<float, 4, InstructionSet::SSE2> {
		
		VectorData() {
			sseValue = _mm_setzero_ps();
		}

		VectorData(float value) {
			sseValue = _mm_set_ps1(value);
		}

		float get(LmSize id) const {
			if (id == 0) {
				return _mm_cvtss_f32(sseValue);
			}
			else if (id == 1) {
				return _mm_cvtss_f32(_mm_shuffle_ps(sseValue, sseValue, _MM_SHUFFLE(1, 1, 1, 1)));
			}
			else if (id == 2) {
				return _mm_cvtss_f32(_mm_shuffle_ps(sseValue, sseValue, _MM_SHUFFLE(2, 2, 2, 2)));
			}
			else {
				return _mm_cvtss_f32(_mm_shuffle_ps(sseValue, sseValue, _MM_SHUFFLE(3, 3, 3, 3)));
			
			}
		}
		void set(LmSize id, float value) {
			if (id == 0) {
				sseValue = _mm_move_ss(sseValue, _mm_set_ss(value));
			}
			else if (id == 1) {
				auto temp = _mm_move_ss(sseValue, _mm_set_ss(value));
				temp = _mm_shuffle_ps(temp, temp, _MM_SHUFFLE(3, 2, 0, 0));
				sseValue = _mm_move_ss(temp, sseValue);
			}
			else if (id == 2) {
				auto temp = _mm_move_ss(sseValue, _mm_set_ss(value));
				temp = _mm_shuffle_ps(temp, temp, _MM_SHUFFLE(3, 0, 1, 0));
				sseValue = _mm_move_ss(temp, sseValue);
			}
			else {
				auto temp = _mm_move_ss(sseValue, _mm_set_ss(value));
				temp = _mm_shuffle_ps(temp, temp, _MM_SHUFFLE(0, 2, 1, 0));
				sseValue = _mm_move_ss(temp, sseValue);
			}
			
		}

		__m128 sseValue;
	};


	Vector<float, 4, InstructionSet::SSE2> operator * (const Vector<float, 4, InstructionSet::SSE2>& l, const Vector<float, 4, InstructionSet::SSE2>& r) {
		Vector<float, 4, InstructionSet::SSE2> result;
		result.sseValue = _mm_mul_ps(l.sseValue, r.sseValue);
		return result;
	}

	
}

#endif // lm_vector_sse_h__
