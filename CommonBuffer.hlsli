#ifndef _BUFFERS_HLSLI_
#define _BUFFERS_HLSLI_

cbuffer CommonMatrixBuffer : register(b0)
{
    row_major matrix viewMatrix;
    row_major matrix projectionMatrix;
}
#endif