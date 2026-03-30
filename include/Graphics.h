#pragma once
#include <CustomWindows.h>
#include <d3d11.h>
#include <wrl/client.h>  // For Microsoft::WRL::ComPtr
#include <d3dcompiler.h> // For D3DCompile function

class Graphics
{
public:
    Graphics(HWND hWnd);
    Graphics(const Graphics &) = delete;            // Delete copy constructor
    Graphics &operator=(const Graphics &) = delete; // Delete copy assignment operator
    ~Graphics() = default;
    void EndFrame();                                                                                      // Function to present the back buffer to the screen
    void ClearBuffer(float r, float g, float b) noexcept;                                                 // Function to clear the back buffer with a specified color
    void DrawTestTriangle(float angle, float x, float y, float z, float windowWidth, float windowHeight); // Function to draw a test triangle (implementation not shown here)

private:
    Microsoft::WRL::ComPtr<ID3D11Device> pDevice;                     // Pointer to the Direct3D device
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext;       // Pointer to the Direct3D device context
    Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;                // Pointer to the swap chain
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderTargetView; // Pointer to the render target view
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;              // Pointer to the depth stencil texture
};