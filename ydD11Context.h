#pragma once
#include <Windows.h>
#include <assert.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <fstream>
#include <wrl/client.h>
#include "ydSingleton.h"
#include "ydD11ResourceManager.h"
#include "ydColoredVertex.h"
#include "ydRoamingCamera.h"
#include "ydCommonMatrixBuffer.h"
#include "ydGrid.h"
#include "ydDeferredDebugVertex.h"
#include "yd11DeferredContext.h"


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
        ComPtr<ID3D11Device> device;
        ComPtr<ID3D11DeviceContext> deviceContext;
        ComPtr<IDXGIFactory> factory;
        ComPtr<IDXGISwapChain> swapChain;
        ComPtr<ID3D11RenderTargetView> renderTargetView;		// 랜더 타겟 뷰
        ComPtr<ID3D11Texture2D> depthStencilBuffer;		// 뎁스 스탠실 버퍼

        ComPtr<ID3D11DepthStencilView> depthStencilView;		// 뎁스 스탠실 뷰
        ComPtr<ID3D11RasterizerState> rasterizerState;
        ComPtr<ID3D11DepthStencilState> depthStencilState;
        DXGI_SAMPLE_DESC multiSamplingDesc{ .Count = 1,.Quality = 0 };

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
        HWND hWnd{ 0 };
    };
    unsigned long Context::Window_Width = 800;
    unsigned long Context::Window_Height = 800;
}

