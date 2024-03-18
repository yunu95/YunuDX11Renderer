#pragma once
#include <Windows.h>
#include <assert.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <fstream>
#include <wrl/client.h>
#include "ydSingleton.h"
#include "yd11ResourceManager.h"
#include "yd11ColoredVertex.h"
#include "yd11RoamingCamera.h"
#include "yd11CommonMatrixBuffer.h"
#include "ydGrid.h"
#include "yd11DeferredDebugVertex.h"
#include "yd11DeferredContext.h"
#include "yd11ScreenPosUVVertex.h"
#include "yd11CubeMap.h"


namespace dx11
{
    using namespace Microsoft::WRL;
    using namespace primitives;
    class Context : public Singleton<Context>
    {
    public:

        // default window size
        static unsigned long Window_Width;
        static unsigned long Window_Height;
        static constexpr size_t indexStride = sizeof(unsigned int);
        ComPtr<ID3D11Device> device;
        ComPtr<ID3D11DeviceContext> deviceContext;
        ComPtr<IDXGIFactory> factory;
        ComPtr<IDXGISwapChain> swapChain;
        ComPtr<ID3D11RenderTargetView> renderTargetView;		// 랜더 타겟 뷰
        ComPtr<ID3D11Texture2D> depthStencilBuffer;		// 뎁스 스탠실 버퍼

        ComPtr<ID3D11DepthStencilView> depthStencilView;		// 뎁스 스탠실 뷰
        ComPtr<ID3D11RasterizerState> defaultRasterizerState; // 기본 래스터라이저 스테이트
        ComPtr<ID3D11RasterizerState> nonCullingRasterizerState; // 후면 컬링을 하지 않는 래스터라이저 스테이트
        ComPtr<ID3D11DepthStencilState> defaultDepthStencilState; // 기본 뎁스 스텐실 스테이트
        ComPtr<ID3D11DepthStencilState> lessEqualDepthStencilState; // 뎁스 테스트를 레스 이퀄로 하는 뎁스 스텐실 스테이트
        DXGI_SAMPLE_DESC multiSamplingDesc{ .Count = 1,.Quality = 0 };

        ComPtr<ID3D11Buffer> screenPosUVVertexBuffer;
        ComPtr<ID3D11Buffer> screenPosUVIndexBuffer;
        ComPtr<ID3D11SamplerState> defaultSamplerState;

        bool Init();
        // 처음 창이 생성될때, 혹은 창의 크기가 변경될때 호출되는 함수
        // 스왑체인의 버퍼 크기를 조정하고, 렌더타겟 뷰를 다시 생성해야함.
        bool OnResize();
        bool UpdateCameraCbuffer();
        bool Update();
        // Window procedure
        static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
        void TestInitCode();
        // 삼각형을 그리는데 쓰였던 테스트 코드
        void TestUpdateCodeTriangle();
    private:

        std::unordered_set<std::unique_ptr<Updatable>> updatables;
        ComPtr<ID3D11Buffer> cbufferCommonMatrix;
        std::unique_ptr<Grid> grid;
        std::unique_ptr<CubeMap> cubemap;
        HWND hWnd{ 0 };
    };
    unsigned long Context::Window_Width = 800;
    unsigned long Context::Window_Height = 800;
}

