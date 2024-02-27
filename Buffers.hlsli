#ifndef _BUFFERS_HLSLI_
#define _BUFFERS_HLSLI_

/*
In Direct3D HLSL (High-Level Shading Language), the register keyword is used to bind shader variables to specific hardware resources.
When you see register(b0) or similar in HLSL code, it specifies which constant buffer slot the shader variable should be bound to.
*/
// 모든 쉐이더 연산에 쓰이는 매트릭스 버퍼들의 목록
cbuffer CommonMatrixBuffer : register(b0)
{
    row_major matrix viewMatrix;
    //row_major matrix VTM;
    //row_major matrix PTM;
    //row_major matrix WVP;
    //row_major matrix WorldInvTrans;
    //row_major matrix VTMInv;
    //row_major matrix lightVP;
}
#endif
