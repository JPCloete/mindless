#include <xmmintrin.h>

#include "../headers/mindless_matrix.h"

mat4 EditMat4Entries(mat4* iMat4, unsigned int iRow, unsigned int iCollumn, float iValue) 
{
    unsigned int iCollumnMemloc = iCollumn * 4;
    unsigned int memlocOffset = iRow + iCollumnMemloc;
    //TODO Maybe add if statement if shit breaks X)
    //if(memlocOffset > 15) return *iMat4; 

    float* iMat4Entry = (float*)iMat4 + memlocOffset;
    *iMat4Entry = iValue;

    return *iMat4;
}


mat4 TransposeMat4(mat4* iMat4Ptr)
{
    mat4 iMat4 = *iMat4Ptr;

    mat4 tMat4{};

    tMat4.X.X = iMat4.X.X;
    tMat4.X.Y = iMat4.Y.X;
    tMat4.X.Z = iMat4.Z.X;
    tMat4.X.W = iMat4.W.X;

    tMat4.Y.X = iMat4.X.Y;
    tMat4.Y.Y = iMat4.Y.Y;
    tMat4.Y.Z = iMat4.Z.Y;
    tMat4.Y.W = iMat4.W.Y;

    tMat4.Z.X = iMat4.X.Z;
    tMat4.Z.Y = iMat4.Y.Z;
    tMat4.Z.Z = iMat4.Z.Z;
    tMat4.Z.W = iMat4.W.Z;

    tMat4.W.X = iMat4.X.W;
    tMat4.W.Y = iMat4.Y.W;
    tMat4.W.Z = iMat4.Z.W;
    tMat4.W.W = iMat4.W.W;

    return tMat4;
}

mat4 MultiplyMat4_Mat4(mat4* iMat4Ptr1, mat4* iMat4Ptr2) 
{
    mat4 iMat1 = TransposeMat4(iMat4Ptr1);
    mat4 iMat2 = *iMat4Ptr2;
    
    mat4 oMat4{};
    __m128 simdVec4X = _mm_setr_ps(iMat1.X.X, iMat1.X.Y, iMat1.X.Z, iMat1.X.W);
    __m128 simdVec4Y = _mm_setr_ps(iMat1.Y.X, iMat1.Y.Y, iMat1.Y.Z, iMat1.Y.W);
    __m128 simdVec4Z = _mm_setr_ps(iMat1.Z.X, iMat1.Z.Y, iMat1.Z.Z, iMat1.Z.W);
    __m128 simdVec4W = _mm_setr_ps(iMat1.W.X, iMat1.W.Y, iMat1.W.Z, iMat1.W.W);

    __m128 simdVec4X2 = _mm_setr_ps(iMat2.X.X, iMat2.X.Y, iMat2.X.Z, iMat2.X.W);
    __m128 simdVec4Y2 = _mm_setr_ps(iMat2.Y.X, iMat2.Y.Y, iMat2.Y.Z, iMat2.Y.W);
    __m128 simdVec4Z2 = _mm_setr_ps(iMat2.Z.X, iMat2.Z.Y, iMat2.Z.Z, iMat2.Z.W);
    __m128 simdVec4W2 = _mm_setr_ps(iMat2.W.X, iMat2.W.Y, iMat2.W.Z, iMat2.W.W);

    __m128 v4XXResult = _mm_mul_ps(simdVec4X, simdVec4X2);
    __m128 v4YXResult = _mm_mul_ps(simdVec4X, simdVec4Y2);
    __m128 v4ZXResult = _mm_mul_ps(simdVec4X, simdVec4Z2);
    __m128 v4WXResult = _mm_mul_ps(simdVec4X, simdVec4W2);

    __m128 v4XYResult = _mm_mul_ps(simdVec4Y, simdVec4X2);
    __m128 v4YYResult = _mm_mul_ps(simdVec4Y, simdVec4Y2);
    __m128 v4ZYResult = _mm_mul_ps(simdVec4Y, simdVec4Z2);
    __m128 v4WYResult = _mm_mul_ps(simdVec4Y, simdVec4W2);

    __m128 v4XZResult = _mm_mul_ps(simdVec4Z, simdVec4X2);
    __m128 v4YZResult = _mm_mul_ps(simdVec4Z, simdVec4Y2);
    __m128 v4ZZResult = _mm_mul_ps(simdVec4Z, simdVec4Z2);
    __m128 v4WZResult = _mm_mul_ps(simdVec4Z, simdVec4W2);

    __m128 v4XWResult = _mm_mul_ps(simdVec4W, simdVec4X2);
    __m128 v4YWResult = _mm_mul_ps(simdVec4W, simdVec4Y2);
    __m128 v4ZWResult = _mm_mul_ps(simdVec4W, simdVec4Z2);
    __m128 v4WWResult = _mm_mul_ps(simdVec4W, simdVec4W2);

    float XXResult = v4XXResult.m128_f32[0] + v4XXResult.m128_f32[1] + v4XXResult.m128_f32[2] + v4XXResult.m128_f32[3];
    float YXResult = v4YXResult.m128_f32[0] + v4YXResult.m128_f32[1] + v4YXResult.m128_f32[2] + v4YXResult.m128_f32[3];
    float ZXResult = v4ZXResult.m128_f32[0] + v4ZXResult.m128_f32[1] + v4ZXResult.m128_f32[2] + v4ZXResult.m128_f32[3];
    float WXResult = v4WXResult.m128_f32[0] + v4WXResult.m128_f32[1] + v4WXResult.m128_f32[2] + v4WXResult.m128_f32[3];

    float XYResult = v4XYResult.m128_f32[0] + v4XYResult.m128_f32[1] + v4XYResult.m128_f32[2] + v4XYResult.m128_f32[3];
    float YYResult = v4YYResult.m128_f32[0] + v4YYResult.m128_f32[1] + v4YYResult.m128_f32[2] + v4YYResult.m128_f32[3];
    float ZYResult = v4ZYResult.m128_f32[0] + v4ZYResult.m128_f32[1] + v4ZYResult.m128_f32[2] + v4ZYResult.m128_f32[3];
    float WYResult = v4WYResult.m128_f32[0] + v4WYResult.m128_f32[1] + v4WYResult.m128_f32[2] + v4WYResult.m128_f32[3];

    float XZResult = v4XZResult.m128_f32[0] + v4XZResult.m128_f32[1] + v4XZResult.m128_f32[2] + v4XZResult.m128_f32[3];
    float YZResult = v4YZResult.m128_f32[0] + v4YZResult.m128_f32[1] + v4YZResult.m128_f32[2] + v4YZResult.m128_f32[3];
    float ZZResult = v4ZZResult.m128_f32[0] + v4ZZResult.m128_f32[1] + v4ZZResult.m128_f32[2] + v4ZZResult.m128_f32[3];
    float WZResult = v4WZResult.m128_f32[0] + v4WZResult.m128_f32[1] + v4WZResult.m128_f32[2] + v4WZResult.m128_f32[3];

    float XWResult = v4XWResult.m128_f32[0] + v4XWResult.m128_f32[1] + v4XWResult.m128_f32[2] + v4XWResult.m128_f32[3];
    float YWResult = v4YWResult.m128_f32[0] + v4YWResult.m128_f32[1] + v4YWResult.m128_f32[2] + v4YWResult.m128_f32[3];
    float ZWResult = v4ZWResult.m128_f32[0] + v4ZWResult.m128_f32[1] + v4ZWResult.m128_f32[2] + v4ZWResult.m128_f32[3];
    float WWResult = v4WWResult.m128_f32[0] + v4WWResult.m128_f32[1] + v4WWResult.m128_f32[2] + v4WWResult.m128_f32[3];

    oMat4.X.X = XXResult;
    oMat4.Y.X = YXResult;
    oMat4.Z.X = ZXResult;
    oMat4.W.X = WXResult;

    oMat4.X.Y = XYResult;
    oMat4.Y.Y = YYResult;
    oMat4.Z.Y = ZYResult;
    oMat4.W.Y = WYResult;

    oMat4.X.Z = XZResult;
    oMat4.Y.Z = YZResult;
    oMat4.Z.Z = ZZResult;
    oMat4.W.Z = WZResult;

    oMat4.X.W = XWResult;
    oMat4.Y.W = YWResult;
    oMat4.Z.W = ZWResult;
    oMat4.W.W = WWResult;

    return oMat4;
}


