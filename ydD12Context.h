#pragma once
#include <Windows.h>
#include <assert.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl/client.h>
#include "ydSingleton.h"
#include "ydVertex.h"

using namespace Microsoft::WRL;

namespace dx12
{
    class Context : public Singleton<Context>
    {
    public:
        // default window size
        static constexpr int Window_Width = 800;
        static constexpr int Window_Height = 800;
        ComPtr<ID3D12Device> dev;
        ComPtr<ID3D12CommandQueue> commandQueue; // deviceContext가 빠지고 CommandQueue가 생김
        ComPtr<IDXGISwapChain3> swapChain; // 이제 스왑체인이 swapChain3라고 불림
        ComPtr<ID3D12Resource> renderTargets[2];		// 렌더타겟 객체가 더블 버퍼링을 위해 둘 존재함.
        ComPtr<ID3D12DescriptorHeap> rtvHeap;		// 렌더 타겟 뷰 힙
        UINT rtvDescriptorSize;

        int Init()
        {
            HINSTANCE hInstance = GetModuleHandle(NULL);
            WNDCLASSEX wc = { 0 };
            wc.cbSize = sizeof(WNDCLASSEX);
            wc.style = CS_HREDRAW | CS_VREDRAW; // 윈도우 크기가 가로 / 세로로 바뀌면 다시 그리기
            wc.lpfnWndProc = WindowProc; // 윈도우 프로시저 콜백 함수 설정
            wc.hInstance = hInstance; // 어플리케이션 인스턴스에 대한 핸들
            wc.hCursor = LoadCursor(NULL, IDC_ARROW); // 표준 마우스 커서 사용 
            wc.lpszClassName = L"DirectX12 Tutorial"; // 윈도우 클래스 이름 설정
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
                dev.GetAddressOf(),
                &featureLevelSupported,
                deviceContext.GetAddressOf());

            //hr = D3D11CreateDevice()
            assert(SUCCEEDED(hr));

            // 렌더 타겟 뷰 생성
            ID3D11Texture2D* backBuffer;
            // 스왑체인의 버퍼를 backBuffer로 지정
            swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
            // backBuffer를 기반으로 렌더타겟 뷰 생성, 이렇게 하면 backBuffer 자원에 대한 소유권을 device가 획득
            dev->CreateRenderTargetView(backBuffer, nullptr, renderTargetView.GetAddressOf());
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
            return true;
        };
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
            static float resetColor[]{ 1,0,0,1 };
            deviceContext->ClearRenderTargetView(renderTargetView.Get(), resetColor);
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
    };
}
