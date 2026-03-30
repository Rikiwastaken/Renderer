#include "Graphics.h"
#include "Exceptions.h"

Graphics::Graphics(HWND hWnd)
{
    // Initialize Direct3D 11 here using the provided window handle (hWnd)
    // This typically involves creating a device, device context, swap chain, etc.

    DXGI_SWAP_CHAIN_DESC sd = {};                                          // Swap chain description
    sd.BufferDesc.Width = 0;                                               // Set to 0 to use the window's client area width
    sd.BufferDesc.Height = 0;                                              // Set to 0 to use the window's client area height
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;                     // Common format for back buffer
    sd.BufferDesc.RefreshRate.Numerator = 0;                               // Set to 0 to use the default refresh rate
    sd.BufferDesc.RefreshRate.Denominator = 0;                             // Set to 0 to use the default refresh rate
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;                 // Set to unspecified for default scaling
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; // Set to unspecified for default scanline ordering
    sd.SampleDesc.Count = 1;                                               // No multisampling
    sd.SampleDesc.Quality = 0;                                             // No multisampling
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;                      // Use the back buffer as a render target
    sd.BufferCount = 1;                                                    // One back buffer
    sd.OutputWindow = hWnd;                                                // The window handle to associate with the swap chain
    sd.Windowed = TRUE;                                                    // Start in windowed mode
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;                              // Discard the back buffer contents after presenting
    sd.Flags = 0;                                                          // No additional flags

    HRESULT hr;

    GFX_THROW_FAILED(D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &sd,
        &pSwapChain,
        &pDevice,
        nullptr,
        &pDeviceContext));

    // gain access to the back buffer and create a render target view here if needed
    ID3D11Resource *pBackBuffer = nullptr;
    GFX_THROW_FAILED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void **>(&pBackBuffer))); // Pointer to the back buffer resource
    GFX_THROW_FAILED(pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pRenderTargetView));                   // Create a render target view for the back buffer
    pBackBuffer->Release();                                                                                        // Release the back buffer resource as it's no longer needed after creating the render target view
}

Graphics::~Graphics()
{
    // Clean up Direct3D resources
    if (pDeviceContext != nullptr)
    {
        pDeviceContext->Release();
        pDeviceContext = nullptr;
    }
    if (pSwapChain != nullptr)
    {
        pSwapChain->Release();
        pSwapChain = nullptr;
    }
    if (pDevice != nullptr)
    {
        pDevice->Release();
        pDevice = nullptr;
    }
    if (pRenderTargetView != nullptr)
    {
        pRenderTargetView->Release();
        pRenderTargetView = nullptr;
    }
}

void Graphics::EndFrame()
{
    pSwapChain->Present(1u, 0u); // Present the back buffer to the screen with vsync enabled (1 for vsync, 0 for no vsync)
}
