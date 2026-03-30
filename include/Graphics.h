#pragma once
#include <CustomWindows.h>
#include <d3d11.h>

class Graphics
{
public:
    Graphics(HWND hWnd);
    Graphics(const Graphics &) = delete;            // Delete copy constructor
    Graphics &operator=(const Graphics &) = delete; // Delete copy assignment operator
    ~Graphics();
    void EndFrame();                                     // Function to present the back buffer to the screen
    void ClearBuffer(float r, float g, float b) noexcept // Function to clear the back buffer with a specified color
    {
        const float color[] = {r, g, b, 1.0f};                           // RGBA color array
        pDeviceContext->ClearRenderTargetView(pRenderTargetView, color); // Clear the render target view with the specified color
    }

private:
    ID3D11Device *pDevice = nullptr;                     // Pointer to the Direct3D device
    ID3D11DeviceContext *pDeviceContext = nullptr;       // Pointer to the Direct3D device context
    IDXGISwapChain *pSwapChain = nullptr;                // Pointer to the swap chain
    ID3D11RenderTargetView *pRenderTargetView = nullptr; // Pointer to the render target view
};