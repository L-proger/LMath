#ifndef LM_HALF_h__
#define LM_HALF_h__

#include <cstdint>
#include "lm_types.h"

namespace lm{

    class Half{
    private:
        struct FI32{
			static_assert(sizeof(float) == 4, "Not supported float size");
			float& f()  {
				return *(reinterpret_cast<float*>(&i));
			}
            uint32_t i;
        };
        std::uint16_t data;
    public:
        bool isNan()  {
            return ((data & 0x7c00) == 0x7c00) && ((data & 0x3ff) != 0);
        }
        bool isInf()  {
            return ((data & 0x7c00) == 0x7c00) && ((data & 0x3ff) == 0);
        }
        bool isSubnormal()  {
            return ((data & 0x7c00) == 0) && ((data & 0x3ff) != 0);
        }
        bool isPositive()  {
            return (data & 0x8000) == 0;
        }
        bool isZero()  {
            return (data & 0x7fff) == 0;
        }
        const float toFloat() const  {
            return toFloat(*this);
        }
        static Half toHalf(float src)  {
            Half dst;
            FI32 fi;
            fi.f() = src;
            int32_t s,m,e;

            //fp32 exponent zero offset = 127
            //fp16 exponent zero offset = 15

            s = (fi.i >> 16) & 0x8000; //-V519
            m = fi.i & 0x7fffff; //-V519
            //decode exponent from fp32 and encode to fp16
            e = ((fi.i >> 23) & 0xff) - 127 + 15; //-V519

            if ((e > 0) && (e < 30)){ 
                //simple case, no exp overflow, just loose precision
                dst.data = static_cast<uint16_t>((s) | (e << 10) | ((m + 0x1000) >> 13));
            }else if(src == 0.0f){
                //handle zero
                dst.data = 0;
            }else{
                //subnormal values
                if(e <= 0){
                    if(e < -10){ //-10-15 = -25(exp) - too small value to represent in half float
                        dst.data = 0;
                    }else{//just subnormal value, representable in half
                        //add explicitly 1 leading bit
                        m = (m | 0x800000) >> (-e + 1);

                        //round to bigger value
                        if(m & 0x1000)
                            m |= 0x2000;
                        dst.data = static_cast<uint16_t>((s) | (m >> 13));
                    }
                }else if(e == 0xff - 127 + 15){//exp exactly on NAN or INF
                    //mantissa == 0 and exp == 11111 then inf
                    //else, if mantissa  != 0 -> NAN
                    if(m == 0){//INF
                        dst.data = static_cast<uint16_t>(s | 0x7c00);
                    }else{//NAN
                        dst.data = static_cast<uint16_t>(s | 0x7c00 | (m >> 13));
                    }
                }else{//Exp should be > 0  (> 30 or 0 + 1 (rounded))
                    //round
                    if(m & 0x1000){
                        m += 0x2000;
                        if(m & 0x800000){
                            m = 0;
                            e++;
                        }
                    }
                    if(e > 30){//exp overflow
                        dst.data = static_cast<uint16_t>(s | 0x7c00);//too big val, inf
                    }else{//
                        dst.data = static_cast<uint16_t>((s) | (e << 10) | (m >> 13));
                    }
                }
            }
            return dst;
        }
        static float toFloat(const Half& src)  {
            int s, e, m;
            FI32 fi;
            s = src.data >> 15;
            e = (src.data & 0x7c00) >> 10;
            m = (src.data & 0x3ff);

            if(e > 0 && e < 31){//normalized val
                fi.i = (s << 31) | ((e - 15 + 127) << 23) | (m << 13);
            }else if((e == 0) && (m == 0)){//signed zero
                fi.i = s << 31;
            }else if((e == 0) && (m != 0)){//denormalized value
                //normalize value
                while(!(m & 0x400)){
                    m <<= 1;
                    e--;
                }
                e++;
                m = m & 0x3ff;
                fi.i = (s << 31) | ((e - 15 + 127) << 23) | (m << 13);
            }else if((e == 31) && (m == 0)){//INF
                fi.i = (s << 31) | 0x7f800000;
            }else{
                fi.i = (s << 31) | 0x7f800000 | (m << 13);
            }
            return fi.f();
        }

        Half() {

        }
        Half(const Half& val) {
            data = val.data;
        }
        Half(const float& val) {
            *this = toHalf(val);
        }
        Half& operator =(const float& other) {
            *this = toHalf(other);
            return *this;
        }

        //operators
        Half operator +() {
            return *this;
        }
        Half operator -() {
            return toHalf(-toFloat());
        }
        Half operator +(const Half& rval) {
            return toHalf(this->toFloat() + rval.toFloat());
        }
        Half operator -(const Half& rval) {
            return toHalf(this->toFloat() - rval.toFloat());
        }
        Half operator *(const Half& rval) {
            return toHalf(this->toFloat() * rval.toFloat());
        }
        Half operator /(const Half& rval) {
            return toHalf(this->toFloat() / rval.toFloat());
        }

        bool operator ==(const Half& rval) {
            return data == rval.data;
        }
        bool operator !=(const Half& rval) {
            return data != rval.data;
        }
        bool operator <(const Half& rval) {
            return this->toFloat() < rval.toFloat();
        }
        bool operator >(const Half& rval) {
            return this->toFloat() > rval.toFloat();
        }
        bool operator <=(const Half& rval) {
            return this->toFloat() <= rval.toFloat();
        }
        bool operator >=(const Half& rval) {
            return this->toFloat() >= rval.toFloat();
        }
        Half& operator +=(const Half& rval) {
            *this = toHalf(this->toFloat() + rval.toFloat());
            return *this;
        }
        Half& operator -=(const Half& rval) {
            *this = toHalf(this->toFloat() - rval.toFloat());
            return *this;
        }
        Half& operator *=(const Half& rval) {
            *this = toHalf(this->toFloat() * rval.toFloat());
            return *this;
        }
        Half& operator /=(const Half& rval) {
            *this = toHalf(this->toFloat() / rval.toFloat());
            return *this;
        }

        const Half& operator++() {
            *this = toHalf(this->toFloat() + 1.0f);
            return *this;
        }
        const Half operator++(int) {
            Half old(*this);
            *this = toHalf(this->toFloat() + 1.0f);
            return old;
        }
        const Half& operator--() {
            *this = toHalf(this->toFloat() - 1.0f);
            return *this;
        }
        const Half operator--(int) {
            Half old(*this);
            *this = toHalf(this->toFloat() - 1.0f);
            return old;
        }
    };

    typedef Half half;
}
#endif // LM_HALF_h__