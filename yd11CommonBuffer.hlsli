#ifndef _BUFFERS_HLSLI_
#define _BUFFERS_HLSLI_

cbuffer CommonMatrixBuffer : register(b0)
{
    // 뷰
    row_major matrix cam_view;
    // 카메라의 월드는 뷰의 역행렬
    row_major matrix cam_world;
    // 투영 변환 행렬
    row_major matrix cam_projection;
    // 뷰 * 투영 변환 행렬
    row_major matrix cam_viewProjection;
}
#endif