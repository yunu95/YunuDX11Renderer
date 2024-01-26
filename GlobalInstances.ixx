module;
#include "DX11Dependency.h"
export module globalInstances;

import std;

// COM
// COM ��ü�� DirectX�� ���α׷��� ��� �������� ���� ȣȯ���� �����ϰ� �ϴ� ����̴�.
// C++������ COM ��ü�� C++ Ŭ������ �����ϰ� ����Ѵ�.
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
    // ID3D11Device�� ID3D11DeviceContext�� Dirct3D�� �ֵ� �������̽���, �������� �׷��� ��ġ �ϵ��� ���� ����Ʈ���� ������ ������ �� �ִ�.
    // ID3D11Device �������̽��� ��� ���� ���˰� �ڿ� �Ҵ翡 ���δ�.
    Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice{};
    // ID3D11DeviceContext �������̽��� ���� ����� �����ϰ�, �ڿ��� �׷��� ���������ο� ����, GPU�� ������ ������ ��ɵ��� �����ϴµ� ���δ�.
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dImmediateContext{};
    // IDXGISwapChain �������̽��� �� ���۸� �����ϴµ� ���δ�.
    Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain{};
    // �ڿ��� ������������ �ܰ迡 �������� �ڿ��� ���� �並 �����ؾ� �Ѵ�.
    // ����Ÿ�� ��� ������ ����� ����ϴ� �� ���Ǵ� ���̴�.
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView{};
    // �ȼ��� ���� ������ �����ϴ� 2���� �ؽ�ó�� ���� ���ٽ� ���۶�� �Ѵ�.
    Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer{};
    // ���� ���ٽ� ���۸� ������������ ���� ���ٽ� �ܰ迡 �������� ���� ���ٽ� �並 �����ؾ� �Ѵ�.
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView{};
}
bool globalInstances::Init()
{
    // D3D11CreateDevice �Լ��� Direct3D ��ġ�� �����Ѵ�.

    // D3D_DRIVER_TYPE_HARDWARE : Direct3D ��ġ�� ������ �� �ϵ���� ������ ����Ѵ�.
    // nullptr : Direct3D ��ġ�� ������ �� ����� ����Ʈ���� �������� �����Ѵ�. nullptr�� �����ϸ� �⺻������ �����ȴ�.
    // D3D11_CREATE_DEVICE_DEBUG : ����� ���̾ ����Ѵ�.
    // nullptr : Direct3D ��ġ�� ������ �� ����� Ư�� ������ �����Ѵ�. nullptr�� �����ϸ� �⺻������ �����ȴ�.
    // 0 : Direct3D ��ġ�� ������ �� ����� Ư�� ������ ������ �����Ѵ�.
    // D3D11_SDK_VERSION : Direct3D SDK�� ������ �����Ѵ�.
    // &d3dDevice : ������ Direct3D ��ġ�� ���� ������ �ּҸ� �����Ѵ�.
    // nullptr : ������ Direct3D ��ġ�� Ư�� ������ ���� ������ �ּҸ� �����Ѵ�.
    // &d3dImmediateContext : ������ Direct3D ��ġ ���ؽ�Ʈ�� ���� ������ �ּҸ� �����Ѵ�.
    const D3D_FEATURE_LEVEL d3dFeatureLevels[]{ D3D_FEATURE_LEVEL_11_0 };
    HRESULT HR;
    HR = D3D11CreateDevice(
        nullptr, // pAdapter : Direct3D ��ġ�� ������ �� ����� ����͸� �����Ѵ�. nullptr�� �����ϸ� �⺻������ �����ȴ�.
        D3D_DRIVER_TYPE_HARDWARE, // DriverType : Direct3D ��ġ�� ������ �� �ϵ���� ������ ����Ѵ�.
        nullptr, // Software : Direct3D ��ġ�� ������ �� ����� ����Ʈ���� �������� �����Ѵ�. nullptr�� �����ϸ� �⺻������ �����ȴ�.
        D3D11_CREATE_DEVICE_DEBUG, // Flags : ����� ���̾ ����Ѵ�.
        d3dFeatureLevels, // pFeatureLevels : Direct3D ��ġ�� ������ �� ����� Ư�� ����(D3D11)�� �����Ѵ�. nullptr�� �����ϸ� �⺻������ �����ȴ�.
        1, // FeatureLevels : Direct3D ��ġ�� ������ �� ����� Ư�� ������ ������ �����Ѵ�.
        D3D11_SDK_VERSION, // SDKVersion : Direct3D SDK�� ������ �����Ѵ�.
        d3dDevice.GetAddressOf(), // ppDevice : ������ Direct3D ��ġ�� ���� ������ �ּҸ� �����Ѵ�.
        nullptr, // pFeatureLevel : ������ Direct3D ��ġ�� Ư�� ������ ���� ������ �ּҸ� �����Ѵ�.
        d3dImmediateContext.GetAddressOf()); // ppImmediateContext : ������ Direct3D ��ġ ���ؽ�Ʈ�� ���� ������ �ּҸ� �����Ѵ�.
    if (FAILED(HR))
    {
        MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);
        return false;
    }
    UINT msaaQulality4x{};
    if (globalInstances::desc.enable4xMsaa)
    {
        HR = d3dDevice->CheckMultisampleQualityLevels(
            DXGI_FORMAT_R8G8B8A8_UNORM, // Format : ��Ƽ���ø��� �����ϴ� �ؽ�ó�� ������ �����Ѵ�.
            4, // SampleCount : ��Ƽ���ø��� �����ϴ� �ؽ�ó�� ���ø� ���� �����Ѵ�.
            &msaaQulality4x); // pNumQualityLevels : ��Ƽ���ø��� �����ϴ� �ؽ�ó�� ǰ�� ������ ���� ������ �ּҸ� �����Ѵ�.

        // 4X MSAA�� �׻� �����ǹǷ�, ��ȯ�� ǰ�� ������ �ݵ�� 0���� Ŀ�� �Ѵ�. 
        assert(msaaQulality4x > 0);
    }

    DXGI_SWAP_CHAIN_DESC sd;
    sd.BufferDesc.Width = 1920; // �� ������ �ʺ� �����Ѵ�.
    sd.BufferDesc.Height = 1080; // �� ������ ���̸� �����Ѵ�.
    sd.BufferDesc.RefreshRate.Numerator = 60; // �� ������ �ֻ����� ���ڸ� �����Ѵ�.
    sd.BufferDesc.RefreshRate.Denominator = 1; // �� ������ �ֻ����� �и� �����Ѵ�.
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // �� ������ ������ �����Ѵ�.
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; // �� ������ ��ĵ���� ������ �����Ѵ�.
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; // �� ������ ũ�� ���� ����� �����Ѵ�.

    // ��Ƽ���ø�
    if (globalInstances::desc.enable4xMsaa)
    {
        sd.SampleDesc.Count = 4; // ��Ƽ���ø��� �����ϴ� �ؽ�ó�� ���ø� ���� �����Ѵ�.
        sd.SampleDesc.Quality = msaaQulality4x;
    }
    else
    {
        sd.SampleDesc.Count = 1; // ��Ƽ���ø��� �����ϴ� �ؽ�ó�� ���ø� ���� �����Ѵ�.
        sd.SampleDesc.Quality = 0;
    }

    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // �� ������ ��� ����� �����Ѵ�.
    sd.BufferCount = 1; // �� ������ ������ �����Ѵ�.
    sd.OutputWindow = globalInstances::desc.hWnd; // ����� â�� �ڵ��� �����Ѵ�.
    sd.Windowed = true; // â ���� �������� ���θ� �����Ѵ�.
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // ���� ü���� ������ �����Ѵ�. DISCARD�� �����ϸ� ���� ȿ������ �������� �˾Ƽ� �����Ѵ�.
    sd.Flags = 0; // ���� ü���� �÷��׸� �����Ѵ�.

    {
        Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice{};
        Microsoft::WRL::ComPtr<IDXGIAdapter> dxgiAdapter{};
        Microsoft::WRL::ComPtr<IDXGIFactory> dxgiFactory{};
        HR = d3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(dxgiDevice.GetAddressOf())); // IDXGIDevice �������̽��� ��´�.
        assert(SUCCEEDED(HR));
        HR = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(dxgiAdapter.GetAddressOf())); // IDXGIAdapter �������̽��� ��´�.
        assert(SUCCEEDED(HR));
        HR = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(dxgiFactory.GetAddressOf())); // IDXGIFactory �������̽��� ��´�.
        assert(SUCCEEDED(HR));
        HR = dxgiFactory->CreateSwapChain(d3dDevice.Get(), &sd, swapChain.GetAddressOf()); // ���� ü���� �����Ѵ�.
        assert(SUCCEEDED(HR));
    }

    {
        Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer{};
        HR = globalInstances::swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf())); // �� ������ �ؽ�ó�� ��´�.
        assert(SUCCEEDED(HR));
        HR = d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, globalInstances::renderTargetView.GetAddressOf()); // �� ������ �ؽ�ó�� ����Ÿ�� ��� �����Ѵ�.
        assert(SUCCEEDED(HR));
    }

    {
        D3D11_TEXTURE2D_DESC depthStencilDesc;
        depthStencilDesc.Width = globalInstances::desc.clientWidth; // �ؽ�ó�� �ʺ� �����Ѵ�.
        depthStencilDesc.Height = globalInstances::desc.clientHeight; // �ؽ�ó�� ���̸� �����Ѵ�.
        depthStencilDesc.MipLevels = 1; // �ؽ�ó�� �Ӹ� ������ ������ �����Ѵ�.
        depthStencilDesc.ArraySize = 1; // �ؽ�ó �迭�� ũ�⸦ �����Ѵ�.
        depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // ���� ���ٽ� ������ ������ �����Ѵ�.

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

        depthStencilDesc.Usage = D3D11_USAGE_DEFAULT; // �ؽ�ó�� ��� ����� �����Ѵ�.
        depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL; // �ؽ�ó�� ������������ �ܰ迡 ���� ����� �����Ѵ�.
        depthStencilDesc.CPUAccessFlags = 0; // �ؽ�ó�� CPU���� �����ϴ� ����� �����Ѵ�.
        depthStencilDesc.MiscFlags = 0; // �ؽ�ó�� ��Ÿ �÷��׸� �����Ѵ�.

        HR = d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, globalInstances::depthStencilBuffer.GetAddressOf()); // �ؽ�ó�� �����Ѵ�.
        assert(SUCCEEDED(HR));
        HR = d3dDevice->CreateDepthStencilView(globalInstances::depthStencilBuffer.Get(), nullptr, globalInstances::depthStencilView.GetAddressOf()); // �ؽ�ó�� ���� ���ٽ� ��� �����Ѵ�.
        assert(SUCCEEDED(HR));
    }

    // ����Ÿ�� ��� ���� ���ٽ� �並 ��� ���� �ܰ迡 ���´�.
    // OM�� ���� Output Merger, ��� ���� �ܰ踦 �ǹ��Ѵ�.
    d3dImmediateContext->OMSetRenderTargets(1, globalInstances::renderTargetView.GetAddressOf(), globalInstances::depthStencilView.Get());

    return false;
}
