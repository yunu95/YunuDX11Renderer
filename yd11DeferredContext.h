#pragma once
#include "yd11DXDependency.h"
#include "ydSingleton.h"
#include "yd11DeferredDebugVertex.h"

namespace dx11
{
    class DeferredContext : public Singleton<DeferredContext>
    {
    public:
        static constexpr int gBufferCount{ 4 };
        static constexpr float gBufferDebugViewportScale{ 0.3f };
        bool Init();
        // 화면 크기 재조정시 크기에 알맞게 텍스처들을 재생성
        bool OnResize();
        // 스태틱 메시들의 정보들을 g버퍼에 그린다.
        void RenderStaticMeshes();
        // g버퍼에 대해 라이팅 연산이 끝난 최종 출력 이미지를 스왑체인의 백 버퍼에 그린다.
        void RenderToBackBuffer();
        // 화면 상단에 디퍼드 이미지들을 그림
        bool DrawDeferredImagesOnTop();
        void TestDrawNodeRecursively(aiNode* node, const aiScene* scene);
        // 현재 창의 크기에 맞게 G버퍼를 생성해준다.
        ComPtr<ID3D11Texture2D> CreateGBuffer(DXGI_FORMAT format);

        ID3D11RenderTargetView* gBufferRTVs[gBufferCount]{};
        ID3D11ShaderResourceView* gBufferSRVs[gBufferCount]{};
        ComPtr<ID3D11Texture2D> gBufferPosition;		// 디퍼드용 위치 버퍼
        ComPtr<ID3D11Texture2D> gBufferNormal;		// 디퍼드용 노말 버퍼
        ComPtr<ID3D11Texture2D> gBufferAlbedo;		// 디퍼드용 알베도 버퍼
        ComPtr<ID3D11Texture2D> gBufferARM;		// 디퍼드용 ARM 버퍼
        ComPtr<ID3D11RenderTargetView> gBufferViewPosition;		// 디퍼드용 위치 버퍼 뷰
        ComPtr<ID3D11RenderTargetView> gBufferViewNormal;		// 디퍼드용 노말 버퍼 뷰
        ComPtr<ID3D11RenderTargetView> gBufferViewAlbedo;		// 디퍼드용 알베도 버퍼 뷰
        ComPtr<ID3D11RenderTargetView> gBufferViewARM;		// 디퍼드용 메탈릭 버퍼 뷰
        ComPtr<ID3D11ShaderResourceView> gBufferSRVPosition;		// 디퍼드용 위치 버퍼 SRV
        ComPtr<ID3D11ShaderResourceView> gBufferSRVNormal;		// 디퍼드용 노말 버퍼 SRV
        ComPtr<ID3D11ShaderResourceView> gBufferSRVAlbedo;		// 디퍼드용 알베도 버퍼 SRV
        ComPtr<ID3D11ShaderResourceView> gBufferSRVARM;		// 디퍼드용 ARM SRV

        //ComPtr<ID3D11DepthStencilView> depthStencilView;		// 뎁스 스탠실 뷰
        //ComPtr<ID3D11DepthStencilState> depthStencilState;        // 뎁스 스탠실 스테이트

        ComPtr<ID3D11Buffer> debugGbufferVerticesBuffer;
        //ComPtr<ID3D11Buffer> lightVerticesBuffer;
        //ComPtr<ID3D11SamplerState> samplerState;
    };
}
