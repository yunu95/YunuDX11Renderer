module;
#include "DX11Dependency.h"
//import <iostream>;
export module globalInstances;


// COM
// COM 객체는 DirectX의 프로그래밍 언어 독립성과 하위 호환성을 가능하게 하는 기술이다.
// C++에서는 COM 객체를 C++ 클래스로 간주하고 사용한다.
export namespace globalInstances
{
    bool Init();
    struct Desc
    {
        HWND hWnd{};
        UINT clientWidth{ 1920 };
        UINT clientHeight{ 1080 };
        bool enable4xMsaa{ true };
    } desc;
    // ID3D11Device와 ID3D11DeviceContext는 Dirct3D의 주된 인터페이스로, 물리적인 그래픽 장치 하드웨어에 대한 소프트웨어 제어기로 생각할 수 있다.
    // ID3D11Device 인터페이스는 기능 지원 점검과 자원 할당에 쓰인다.
    Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice{};
    // ID3D11DeviceContext 인터페이스는 렌더 대상을 설정하고, 자원을 그래픽 파이프라인에 묶고, GPU가 수행할 렌더링 명령들을 지시하는데 쓰인다.
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dImmediateContext{};
    // IDXGISwapChain 인터페이스는 백 버퍼를 관리하는데 쓰인다.
    Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain{};
    // 자원을 파이프라인의 단계에 묶으려면 자원에 대한 뷰를 생성해야 한다.
    // 렌더타겟 뷰는 렌더링 결과를 출력하는 데 사용되는 뷰이다.
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView{};
    // 픽셀의 깊이 정보를 저장하는 2차원 텍스처를 뎁스 스텐실 버퍼라고 한다.
    Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer{};
    // 뎁스 스텐실 버퍼를 파이프라인의 뎁스 스텐실 단계에 묶으려면 뎁스 스텐실 뷰를 생성해야 한다.
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView{};
}
bool globalInstances::Init()
{
    // D3D11CreateDevice 함수는 Direct3D 장치를 생성한다.

    // D3D_DRIVER_TYPE_HARDWARE : Direct3D 장치를 생성할 때 하드웨어 가속을 사용한다.
    // nullptr : Direct3D 장치를 생성할 때 사용할 소프트웨어 렌더러를 지정한다. nullptr을 지정하면 기본값으로 설정된다.
    // D3D11_CREATE_DEVICE_DEBUG : 디버그 레이어를 사용한다.
    // nullptr : Direct3D 장치를 생성할 때 사용할 특성 수준을 지정한다. nullptr을 지정하면 기본값으로 설정된다.
    // 0 : Direct3D 장치를 생성할 때 사용할 특성 수준의 개수를 지정한다.
    // D3D11_SDK_VERSION : Direct3D SDK의 버전을 지정한다.
    // &d3dDevice : 생성된 Direct3D 장치를 받을 변수의 주소를 지정한다.
    // nullptr : 생성된 Direct3D 장치의 특성 수준을 받을 변수의 주소를 지정한다.
    // &d3dImmediateContext : 생성된 Direct3D 장치 컨텍스트를 받을 변수의 주소를 지정한다.
    const D3D_FEATURE_LEVEL d3dFeatureLevels[]{ D3D_FEATURE_LEVEL_11_0 };
    HRESULT HR;
    HR = D3D11CreateDevice(
        nullptr, // pAdapter : Direct3D 장치를 생성할 때 사용할 어댑터를 지정한다. nullptr을 지정하면 기본값으로 설정된다.
        D3D_DRIVER_TYPE_HARDWARE, // DriverType : Direct3D 장치를 생성할 때 하드웨어 가속을 사용한다.
        nullptr, // Software : Direct3D 장치를 생성할 때 사용할 소프트웨어 렌더러를 지정한다. nullptr을 지정하면 기본값으로 설정된다.
        D3D11_CREATE_DEVICE_DEBUG, // Flags : 디버그 레이어를 사용한다.
        d3dFeatureLevels, // pFeatureLevels : Direct3D 장치를 생성할 때 사용할 특성 수준(D3D11)을 지정한다. nullptr을 지정하면 기본값으로 설정된다.
        1, // FeatureLevels : Direct3D 장치를 생성할 때 사용할 특성 수준의 개수를 지정한다.
        D3D11_SDK_VERSION, // SDKVersion : Direct3D SDK의 버전을 지정한다.
        d3dDevice.GetAddressOf(), // ppDevice : 생성된 Direct3D 장치를 받을 변수의 주소를 지정한다.
        nullptr, // pFeatureLevel : 생성된 Direct3D 장치의 특성 수준을 받을 변수의 주소를 지정한다.
        d3dImmediateContext.GetAddressOf()); // ppImmediateContext : 생성된 Direct3D 장치 컨텍스트를 받을 변수의 주소를 지정한다.
    if (FAILED(HR))
    {
        MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);
        return false;
    }
    UINT msaaQulality4x{};
    if (globalInstances::desc.enable4xMsaa)
    {
        HR = d3dDevice->CheckMultisampleQualityLevels(
            DXGI_FORMAT_R8G8B8A8_UNORM, // Format : 멀티샘플링을 지원하는 텍스처의 형식을 지정한다.
            4, // SampleCount : 멀티샘플링을 지원하는 텍스처의 샘플링 수를 지정한다.
            &msaaQulality4x); // pNumQualityLevels : 멀티샘플링을 지원하는 텍스처의 품질 수준을 받을 변수의 주소를 지정한다.

        // 4X MSAA가 항상 지원되므로, 반환된 품질 수준은 반드시 0보다 커야 한다. 
        assert(msaaQulality4x > 0);
    }

    DXGI_SWAP_CHAIN_DESC sd;
    sd.BufferDesc.Width = 1920; // 백 버퍼의 너비를 지정한다.
    sd.BufferDesc.Height = 1080; // 백 버퍼의 높이를 지정한다.
    sd.BufferDesc.RefreshRate.Numerator = 60; // 백 버퍼의 주사율의 분자를 지정한다.
    sd.BufferDesc.RefreshRate.Denominator = 1; // 백 버퍼의 주사율의 분모를 지정한다.
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 백 버퍼의 형식을 지정한다.
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; // 백 버퍼의 스캔라인 순서를 지정한다.
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; // 백 버퍼의 크기 조정 방법을 지정한다.

    // 멀티샘플링
    if (globalInstances::desc.enable4xMsaa)
    {
        sd.SampleDesc.Count = 4; // 멀티샘플링을 지원하는 텍스처의 샘플링 수를 지정한다.
        sd.SampleDesc.Quality = msaaQulality4x;
    }
    else
    {
        sd.SampleDesc.Count = 1; // 멀티샘플링을 지원하는 텍스처의 샘플링 수를 지정한다.
        sd.SampleDesc.Quality = 0;
    }

    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 백 버퍼의 사용 방법을 지정한다.
    sd.BufferCount = 1; // 백 버퍼의 개수를 지정한다.
    sd.OutputWindow = globalInstances::desc.hWnd; // 출력할 창의 핸들을 지정한다.
    sd.Windowed = true; // 창 모드로 실행할지 여부를 지정한다.
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // 스왑 체인의 동작을 지정한다. DISCARD를 지정하면 가장 효율적인 동작으로 알아서 진행한다.
    sd.Flags = 0; // 스왑 체인의 플래그를 지정한다.

    {
        Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice{};
        Microsoft::WRL::ComPtr<IDXGIAdapter> dxgiAdapter{};
        Microsoft::WRL::ComPtr<IDXGIFactory> dxgiFactory{};
        HR = d3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(dxgiDevice.GetAddressOf())); // IDXGIDevice 인터페이스를 얻는다.
        assert(SUCCEEDED(HR));
        HR = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(dxgiAdapter.GetAddressOf())); // IDXGIAdapter 인터페이스를 얻는다.
        assert(SUCCEEDED(HR));
        HR = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(dxgiFactory.GetAddressOf())); // IDXGIFactory 인터페이스를 얻는다.
        assert(SUCCEEDED(HR));
        HR = dxgiFactory->CreateSwapChain(d3dDevice.Get(), &sd, swapChain.GetAddressOf()); // 스왑 체인을 생성한다.
        assert(SUCCEEDED(HR));
    }

    {
        Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer{};
        HR = globalInstances::swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf())); // 백 버퍼의 텍스처를 얻는다.
        assert(SUCCEEDED(HR));
        HR = d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, globalInstances::renderTargetView.GetAddressOf()); // 백 버퍼의 텍스처를 렌더타겟 뷰로 생성한다.
        assert(SUCCEEDED(HR));
    }

    {
        D3D11_TEXTURE2D_DESC depthStencilDesc;
        depthStencilDesc.Width = globalInstances::desc.clientWidth; // 텍스처의 너비를 지정한다.
        depthStencilDesc.Height = globalInstances::desc.clientHeight; // 텍스처의 높이를 지정한다.
        depthStencilDesc.MipLevels = 1; // 텍스처의 밉맵 레벨의 개수를 지정한다.
        depthStencilDesc.ArraySize = 1; // 텍스처 배열의 크기를 지정한다.
        depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // 깊이 스텐실 버퍼의 형식을 지정한다.

        if (globalInstances::desc.enable4xMsaa)
        {
            depthStencilDesc.SampleDesc.Count = 4;
            depthStencilDesc.SampleDesc.Quality = msaaQulality4x - 1;
        }
        else
        {
            depthStencilDesc.SampleDesc.Count = 1;
            depthStencilDesc.SampleDesc.Quality = 0;
        }

        depthStencilDesc.Usage = D3D11_USAGE_DEFAULT; // 텍스처의 사용 방법을 지정한다.
        depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL; // 텍스처를 파이프라인의 단계에 묶는 방법을 지정한다.
        depthStencilDesc.CPUAccessFlags = 0; // 텍스처를 CPU에서 접근하는 방법을 지정한다.
        depthStencilDesc.MiscFlags = 0; // 텍스처의 기타 플래그를 지정한다.

        HR = d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, globalInstances::depthStencilBuffer.GetAddressOf()); // 텍스처를 생성한다.
        assert(SUCCEEDED(HR));
        HR = d3dDevice->CreateDepthStencilView(globalInstances::depthStencilBuffer.Get(), nullptr, globalInstances::depthStencilView.GetAddressOf()); // 텍스처를 뎁스 스텐실 뷰로 생성한다.
        assert(SUCCEEDED(HR));
    }

    // 렌더타겟 뷰와 뎁스 스텐실 뷰를 출력 병합 단계에 묶는다.
    // OM의 뜻은 Output Merger, 출력 병합 단계를 의미한다.
    d3dImmediateContext->OMSetRenderTargets(1, globalInstances::renderTargetView.GetAddressOf(), globalInstances::depthStencilView.Get());

    return false;
}
