#pragma once
#include <CustomWindows.h>
#include <d3d11.h>
#include <wrl/client.h>  // For Microsoft::WRL::ComPtr
#include <d3dcompiler.h> // For D3DCompile function
#include "Exceptions.h"
#include <DirectXMath.h>
#include <memory>
#include "GraphicsThrowMacros.h"
#include "DxgiInfoManager.h"
#include "dxerr.h"

class Graphics
{
public:
    class Exception : public RikiException
    {
        using RikiException::RikiException;
    };
    class HrException : public Exception
    {
    public:
        HrException(int line, const char *file, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept;
        const char *what() const noexcept override;
        const char *GetType() const noexcept override;
        HRESULT GetErrorCode() const noexcept;
        std::string GetErrorString() const noexcept;
        std::string GetErrorDescription() const noexcept;
        std::string GetErrorInfo() const noexcept;

    private:
        HRESULT hr;
        std::string info;
    };
    class InfoException : public Exception
    {
    public:
        InfoException(int line, const char *file, std::vector<std::string> infoMsgs) noexcept;
        const char *what() const noexcept override;
        const char *GetType() const noexcept override;
        std::string GetErrorInfo() const noexcept;

    private:
        std::string info;
    };
    class DeviceRemovedException : public HrException
    {
        using HrException::HrException;

    public:
        const char *GetType() const noexcept override;

    private:
        std::string reason;
    };
    Graphics(HWND hWnd);
    Graphics(const Graphics &) = delete;            // Delete copy constructor
    Graphics &operator=(const Graphics &) = delete; // Delete copy assignment operator
    ~Graphics();
    void EndFrame();                                                                                               // Function to present the back buffer to the screen
    void BeginFrame(float r, float g, float b) noexcept;                                                           // Function to clear the back buffer with a specified color
    void DrawTestCube(float angle, float x, float y, float z, float scale, float windowWidth, float windowHeight); // Function to draw a test triangle
    void DrawD10(float angle, float x, float y, float z, float scale, float windowWidth, float windowHeight);
    void DrawIndexed(UINT indexCount);                         // Function to draw indexed geometry using the currently bound vertex and index buffers
    void SetProjection(DirectX::XMMATRIX projection) noexcept; // Function to set the projection matrix for the graphics context
    DirectX::XMMATRIX GetProjection() const noexcept;          // Function to get the current projection matrix
    void EnableImGui() noexcept;                               // Function to enable ImGui rendering (initializes ImGui for DirectX 11)
    void DisableImGui() noexcept;                              // Function to disable ImGui rendering (shuts down ImGui for DirectX 11)
    bool IsImGuiEnabled() const noexcept;                      // Function to check if ImGui rendering is currently enabled
    ID3D11Device *GetDevice() const noexcept
    {
        return pDevice.Get();
    }

    ID3D11DeviceContext *GetContext() const noexcept
    {
        return pDeviceContext.Get();
    }

public:
    DxgiInfoManager infoManager;

private:
    Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
    Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderTargetView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> pRasterizerState;
    DirectX::XMMATRIX projection; // Projection matrix for the graphics context
    bool imguiEnabled = true;     // Flag to track whether ImGui rendering is enabled
};