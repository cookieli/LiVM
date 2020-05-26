//
// Created by lzx on 5/15/20.
//
#include <LiVM/classfile/constantPool.h>
#include <cmath>
namespace LiVM {
    CONSTANT_Utf8_info::CONSTANT_Utf8_info(): cp_info(), length(0),bytes(nullptr),_cached(false), _cached_string(){}
    String CONSTANT_Utf8_info::getConstant()  {
        if(!_cached){
            _cached_string = strings::fromBytes(bytes.get(), length);
            _cached = true;
        }
        return _cached_string;
    }
    float CONSTANT_Float_info::getConstant() const {
        if (FLOAT_IS_POSITIVE_INFINITY(bytes)) {
            return _FLOAT_POSITIVE_INFINITY;

        } else if (FLOAT_IS_NEGATIVE_INFINITY(bytes)) {
            return _FLOAT_NEGATIVE_INFINITY;

        } else if (FLOAT_IS_NAN(bytes)) {
            return FLOAT_NAN;

        } else {
            int s = ((bytes >> 31) == 0) ? 1 : -1;
            int e = ((bytes >> 23) & 0xff);
            int m = (e == 0)
                    ? (bytes & 0x7fffff) << 1
                    : (bytes & 0x7fffff) | 0x800000;
            return static_cast<float>(s * m * pow(2, e - 150));
        }
    }
    double CONSTANT_Double_info::getConstant() const {
        long bits = ((long) high_bytes << 32) + low_bytes;

        if (DOUBLE_IS_POSITIVE_INFINITY(bits)) {
            return _DOUBLE_POSITIVE_INFINITY;

        } else if (DOUBLE_IS_NEGATIVE_INFINITY(bits)) {
            return _DOUBLE_NEGATIVE_INFINITY;

        } else if (DOUBLE_IS_NAN(bits)) {
            return DOUBLE_NAN;

        } else {
            int s = ((bits >> 63) == 0) ? 1 : -1;
            int e = (int) ((bits >> 52) & 0x7ffL);
            long m = (e == 0)
                     ? (bits & 0xfffffffffffffL) << 1
                     : (bits & 0xfffffffffffffL) | 0x10000000000000L;
            return s * m * pow(2, e - 1075);
        }
    }
}
