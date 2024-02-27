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


namespace dx11
{
    using namespace Microsoft::WRL;
    using namespace primitives;
    class Context : public Singleton<Context>
    {
    public:
        // default window size
        static constexpr int Window_Width = 800;
        static constexpr int Window_Height = 800;
        ComPtr<ID3D11Device> device;
        ComPtr<ID3D11DeviceContext> deviceContext;
        ComPtr<IDXGISwapChain> swapChain;
        ComPtr<ID3D11RenderTargetView> renderTargetView;		// 랜더 타겟 뷰
        ComPtr<ID3D11Texture2D> depthStencilBuffer;		// 뎁스 스탠실 버퍼
        ComPtr<ID3D11DepthStencilView> depthStencilView;		// 뎁스 스탠실 뷰

        //static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
        int Init()
        {
            HINSTANCE hInstance = GetModuleHandle(NULL);
            WNDCLASSEX wc = { 0 };
            wc.cbSize = sizeof(WNDCLASSEX);
            wc.style = CS_HREDRAW | CS_VREDRAW; // 윈도우 크기가 가로 / 세로로 바뀌면 다시 그리기
            wc.lpfnWndProc = WindowProc; // 윈도우 프로시저 콜백 함수 설정
            wc.hInstance = hInstance; // 어플리케이션 인스턴스에 대한 핸들
            wc.hCursor = LoadCursor(NULL, IDC_ARROW); // 표준 마우스 커서 사용 
            wc.lpszClassName = L"DirectX11 Tutorial"; // 윈도우 클래스 이름 설정
            RegisterClassEx(&wc); // 윈도우 클래스를 시스템에 등록

            RECT wr = { 0,0,Window_Width,Window_Height }; // 윈도우 사각형 범위 정의
            AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE); // adjust the window size based on the desired window style
            HWND hWnd = CreateWindowEx(NULL, L"DirectX11 Tutorial", L"DirectX 11 Red Screen", WS_OVERLAPPEDWINDOW, 100, 100, wr.right - wr.left, wr.bottom - wr.top, NULL, NULL, hInstance, NULL); // Create the window

            ShowWindow(hWnd, SW_SHOWNORMAL);
            UpdateWindow(hWnd);

            // Direct X11 초기화

            // swap chain description
            DXGI_SWAP_CHAIN_DESC scd
            {
                .BufferDesc
                {
                    .Width = Window_Width,      // 텍스처 너비
                    .Height = Window_Height,    // 텍스처 높이
                    .RefreshRate // 새로고침 정보
                    {
                        .Numerator = 60, // fps
                        .Denominator = 1  // fps를 분수 꼴로 나타내고 싶을때 쓰이는 분모
                    },
                    .Format = DXGI_FORMAT_R8G8B8A8_UNORM, // 픽셀 포맷
                },
                .SampleDesc
                {
                    .Count = 1, // 픽셀당 멀티샘플링 갯수
                    .Quality = 0, // 멀티샘플링 레벨
                },
                .BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT, // 버퍼의 용도
                .BufferCount = 1, // 백 버퍼의 갯수
                .OutputWindow = hWnd, // 출력 대상 윈도우
                .Windowed = true, // 윈도우 여부
                .SwapEffect = DXGI_SWAP_EFFECT_DISCARD // 버퍼 스왑시 효과
            };

            // 디바이스, 디바이스 콘텍스트, 스왑 체인 생성
            // 가능한 피처 레벨 목록
            D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0,D3D_FEATURE_LEVEL_10_1,D3D_FEATURE_LEVEL_10_0 };
            D3D_FEATURE_LEVEL featureLevelSupported; // 지원가능한 피처 레벨 저장

            //D3D11CreateDevice();
            HRESULT hr;
            hr = D3D11CreateDeviceAndSwapChain(
                NULL,
                D3D_DRIVER_TYPE_HARDWARE,
                NULL,
                NULL,
                featureLevels, 3,
                D3D11_SDK_VERSION,
                &scd,
                swapChain.GetAddressOf(),
                device.GetAddressOf(),
                &featureLevelSupported,
                deviceContext.GetAddressOf());

            //hr = D3D11CreateDevice()
            assert(SUCCEEDED(hr));

            // 렌더 타겟 뷰 생성
            ID3D11Texture2D* backBuffer;
            // 스왑체인의 버퍼를 backBuffer로 지정
            swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
            // backBuffer를 기반으로 렌더타겟 뷰 생성, 이렇게 하면 backBuffer 자원에 대한 소유권을 device가 획득
            device->CreateRenderTargetView(backBuffer, nullptr, renderTargetView.GetAddressOf());
            //dev->CreateDepthStencilView(depthStencilBuffer, 0, &mDepthStencilView);
            // backBuffer Release로 레퍼런스 카운터를 1 감소시켜야함.
            backBuffer->Release();

            D3D11_TEXTURE2D_DESC depthStencilDesc
            {
                .Width = scd.BufferDesc.Width,
                .Height = scd.BufferDesc.Height,
                .MipLevels = 1,
                .ArraySize = 1,
                .Format = DXGI_FORMAT_D24_UNORM_S8_UINT,
                .SampleDesc = scd.SampleDesc,
                .Usage = D3D11_USAGE_DEFAULT,
                .BindFlags = D3D11_BIND_DEPTH_STENCIL,
                .CPUAccessFlags = 0,
                .MiscFlags = 0
            };

            // 렌더 타겟 설정
            deviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.Get());

            // 뷰포트 설정, 여기선 윈도우 크기와 똑같음.
            // 뷰포트는 정규장치 좌표계(NDC)와 렌더타겟 표면과의 관계를 정의한다.
            D3D11_VIEWPORT viewport
            {
                .TopLeftX = 0,
                .TopLeftY = 0,
                .Width = Window_Width,
                .Height = Window_Height,
            };
            // 레스터라이저는 프리미티브한 애들을 픽셀로 변환해준다. 
            // NDC 공간에서 렌더타겟 표면으로 좌표 사상을 해야 레스터라이징이 가능하기에 뷰포트를 지정해 주는 것.
            deviceContext->RSSetViewports(1, &viewport);

            // 루프에서는 아래 코드 돌리면 됨.
            //while (GetMessage(&msg, NULL, 0, 0)) { // Get and dispatch messages until a WM_QUIT message is received
            //    TranslateMessage(&msg); // Translate virtual-key messages into character messages
            //    DispatchMessage(&msg); // Dispatch a message to a window procedure
            //};
            // 공용 상수 버퍼 초기화
            D3D11_BUFFER_DESC bufferDesc
            {
                .ByteWidth = sizeof(CommonMatrixBuffer),
                .Usage = D3D11_USAGE_DEFAULT,
                .BindFlags = D3D11_BIND_CONSTANT_BUFFER
            };
            hr = device->CreateBuffer(&bufferDesc, nullptr, cbufferCommonMatrix.GetAddressOf());
            assert(SUCCEEDED(hr));
            TestInitCode();
            return true;
        };
        bool UpdateCameraCbuffer()
        {
            assert(Camera::GetMainCam());
            const DirectX::XMFLOAT4X4& viewMatrix = Camera::GetMainCam()->GetVTM();
            CommonMatrixBuffer cbufferStruct
            {
                .viewMatrix{Camera::GetMainCam()->GetVTM()},
                .projectionMatrix{Camera::GetMainCam()->GetPTM()},
            };
            //pDstResource: Pointer to the destination resource, which is the buffer or texture you want to update.
            //DstSubresource: Subresource index for the destination resource. For buffers, this is usually 0. For textures with multiple mipmaps or array slices, you specify which subresource to update.
            //pDstBox: Pointer to a box structure that defines the subregion within the destination resource to update. You can specify nullptr to update the entire resource.
            //pSrcData: Pointer to the source data that contains the new content to copy into the destination resource.
            //SrcRowPitch: Distance in bytes between each row of source data. For textures, this is typically the width of the texture multiplied by the size of each texel (pixel) format.
            //SrcDepthPitch: Distance in bytes between each depth slice of source data. This is used for 3D textures and array textures with multiple slices. For 2D textures, this parameter is usually ignored and set to 0.
            deviceContext->UpdateSubresource(cbufferCommonMatrix.Get(), 0, nullptr, &cbufferStruct, 0, 0);
            //StartSlot: Index of the first constant buffer slot to bind. This specifies the starting point in the constant buffer slots of the vertex shader where the binding will occur.
            //NumBuffers: Number of constant buffers to bind. This specifies how many constant buffers you want to bind starting from the StartSlot.
            //ppConstantBuffers: An array of pointers to constant buffer interfaces. These are the constant buffers you want to bind to the vertex shader. The array size should be equal to NumBuffers.
            deviceContext->VSSetConstantBuffers(0, 1, cbufferCommonMatrix.GetAddressOf());
            return true;
        }
        bool Update()
        {
            // 루프에서는 아래 코드 돌리면 됨.
            MSG msg{};
            //while (PeekMessage(&msg, NULL, 0, 0)) { // Get and dispatch messages until a WM_QUIT message is received
            //    TranslateMessage(&msg); // Translate virtual-key messages into character messages
            //    DispatchMessage(&msg); // Dispatch a message to a window procedure
            //};
            while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
            {
                ::TranslateMessage(&msg);
                ::DispatchMessage(&msg);
                if (msg.message == WM_QUIT)
                    return false;
            }
            static float resetColor[]{ 0.3f,0.3f,0.3f,1 };
            deviceContext->ClearRenderTargetView(renderTargetView.Get(), resetColor);

            for (auto& each : updatables)
                each->Update();

            // 카메라 버퍼정보 업데이트
            UpdateCameraCbuffer();
            // 그리드 출력
            grid->DrawGridAndGuizmo();
            TestUpdateCode();
            swapChain->Present(0, 0);
            return true;
        }
        // Window procedure
        static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
            switch (message) {
            case WM_DESTROY: // If the window is destroyed
                PostQuitMessage(0); // Post a quit message to the message queue
                return 0; // Return 0 to indicate that the message was processed
            case WM_PAINT: // If the window needs to be painted
                // Redraw the window here
                // In this case, we don't have anything to draw, so we leave it empty for now
                break; // Break out of the switch statement
            }
            return DefWindowProc(hWnd, message, wParam, lParam); // Call the default window procedure for messages that were not handled
        }
        void TestInitCode()
        {
            auto cam = std::make_unique<RoamingCamera>();
            cam->SetAsMain();
            updatables.insert(std::move(cam));
            grid = std::make_unique<Grid>();
        }
        void TestUpdateCode()
        {
            using namespace dx11;
            using namespace primitives;
            static constexpr ColoredVertex vertices[]
            {
                {.x=0.0f,.y= 0.5f,.z= 0.0f,.color= {1.0f, 0.0f, 0.0f, 1.0f}},  // Top vertex (red)
                {.x=0.5f,.y= -0.5f,.z= 0.0f,.color= {0.0f, 1.0f, 0.0f, 1.0f}}, // Bottom-right vertex (green)
                {.x=-0.5f,.y= -0.5f,.z= 0.0f,.color= {0.0f, 0.0f, 1.0f, 1.0f}} // Bottom-left vertex (blue)
            };
            // Create vertex buffer
            D3D11_BUFFER_DESC bufferDesc = {};
            bufferDesc.Usage = D3D11_USAGE_DEFAULT;
            bufferDesc.ByteWidth = sizeof(vertices);
            bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            bufferDesc.CPUAccessFlags = 0;
            D3D11_SUBRESOURCE_DATA initData = {};
            initData.pSysMem = vertices;

            ComPtr<ID3D11Buffer> vertexBuffer;
            device->CreateBuffer(&bufferDesc, &initData, &vertexBuffer);

            // Set vertex buffer
            UINT stride = sizeof(ColoredVertex);
            UINT offset = 0;
            deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

            // Set primitive topology
            deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

            static auto vertexShader = ResourceManager::Instance().LoadVertexShaderFromFile(L"DefaultVertexShader.cso");
            static auto pixelShader = ResourceManager::Instance().LoadPixelShaderFromFile(L"DefaultPixelShader.cso");

            //D3D11_INPUT_ELEMENT_DESC layout[]
            //{
            //    {"POSITION",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
            //    {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
            //};

            ID3D11InputLayout* inputLayout = ResourceManager::Instance().GetInputLayout(vertexShader);


            deviceContext->IASetInputLayout(inputLayout);

            deviceContext->VSSetShader(vertexShader, nullptr, 0);
            deviceContext->PSSetShader(pixelShader, nullptr, 0);

            deviceContext->Draw(3, 0);

        }
    private:
        std::unordered_set<std::unique_ptr<Updatable>> updatables;
        ComPtr<ID3D11Buffer> cbufferCommonMatrix;
        std::unique_ptr<Grid> grid;
    };
}

