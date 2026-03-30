#include "Graphics.h"
#include "Exceptions.h"
#include <cmath>
#include <DirectXMath.h>

namespace wrl = Microsoft::WRL; // Alias for Microsoft::WRL namespace to simplify code
namespace dx = DirectX;         // Alias for DirectX namespace to simplify code

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
    wrl::ComPtr<ID3D11Resource> pBackBuffer;
    GFX_THROW_FAILED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));                // Pointer to the back buffer resource
    GFX_THROW_FAILED(pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pRenderTargetView)); // Create a render target view for the back buffer                                                                           // Release the back buffer resource as it's no longer needed after creating the render target view
}

void Graphics::ClearBuffer(float r, float g, float b) noexcept // Function to clear the back buffer with a specified color
{
    const float color[] = {r, g, b, 1.0f};                                 // RGBA color array
    pDeviceContext->ClearRenderTargetView(pRenderTargetView.Get(), color); // Clear the render target view with the specified color
}

void Graphics::DrawTestTriangle(float angle, float x, float y, float windowWidth, float windowHeight)
{

    struct Vertex
    {
        struct
        {
            float x, y; // Position
        } pos;
        struct
        {
            unsigned char r, g, b, a; // Color
        } color;
    };

    const Vertex vertices[] =
        {
            {0.0f, 0.5f, 255, 0, 0, 255},
            {0.5f, -0.5f, 0, 255, 0, 255},
            {-0.5f, -0.5f, 0, 0, 255, 255},
            {-0.3f, 0.3f, 0, 0, 255, 255},
            {0.3f, 0.3f, 0, 255, 0, 255},
            {0.0f, -0.8f, 255, 0, 0, 255},
        };

    wrl::ComPtr<ID3D11Buffer> pVertexBuffer; // Pointer to the vertex buffer
    D3D11_BUFFER_DESC bd = {};               // Buffer description
    bd.Usage = D3D11_USAGE_DEFAULT;          // Default usage
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // Bind as a vertex buffer
    bd.ByteWidth = sizeof(vertices);         // Size of the buffer in bytes
    bd.CPUAccessFlags = 0;                   // No CPU access needed
    bd.MiscFlags = 0;                        // No miscellaneous flags
    bd.StructureByteStride = sizeof(Vertex); // Size of each vertex structure
    D3D11_SUBRESOURCE_DATA initData = {};    // Initial data for the buffer
    initData.pSysMem = vertices;             // Pointer to the vertex data
    GFX_THROW_FAILED(pDevice->CreateBuffer(&bd, &initData, &pVertexBuffer));

    // Create index buffer
    const unsigned short indices[] =
        {
            0, 1, 2,
            0, 2, 3,
            0, 4, 1,
            2, 1, 5};

    wrl::ComPtr<ID3D11Buffer> pIndexBuffer;           // Pointer to the index buffer
    D3D11_BUFFER_DESC ibd = {};                       // Buffer description for index buffer
    ibd.Usage = D3D11_USAGE_DEFAULT;                  // Default usage
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;          // Bind as an index
    ibd.ByteWidth = sizeof(indices);                  // Size of the index buffer in bytes
    ibd.CPUAccessFlags = 0;                           // No CPU access needed
    ibd.MiscFlags = 0;                                // No miscellaneous flags
    ibd.StructureByteStride = sizeof(unsigned short); // Size of each index
    D3D11_SUBRESOURCE_DATA indexData = {};            // Initial data for the index
    indexData.pSysMem = indices;                      // Pointer to the index data
    GFX_THROW_FAILED(pDevice->CreateBuffer(&ibd, &indexData, &pIndexBuffer));

    // Bind Index Buffer to pipeline
    pDeviceContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0); // Set the index buffer to the input assembler stage (using 16-bit unsigned integers for indices)

    // Bind Vertex to pipeline
    const UINT stride = sizeof(Vertex);                                                       // Size of each vertex structure
    const UINT offset = 0;                                                                    // Offset in the vertex buffer
    pDeviceContext->IASetVertexBuffers(0, 1, pVertexBuffer.GetAddressOf(), &stride, &offset); // Set the vertex buffer to the input assembler stage

    // Create constant buffer for transformation matrices (e.g., world-view-projection matrix)

    struct ConstantBuffer
    {
        dx::XMMATRIX transform; // Transformation matrix (e.g., world-view-projection matrix)
    };
    const ConstantBuffer cb = {
        dx::XMMatrixTranspose( // Transpose the matrix for HLSL (row-major to column-major)
            dx::XMMatrixRotationZ(angle) *
            dx::XMMatrixScaling(3.0f / 4.0f, 1.0f, 1.0f) *
            dx::XMMatrixTranslation(x, y, 0.0f)) // Example transformation: rotate around Z-axis and scale down;
    };

    wrl::ComPtr<ID3D11Buffer> pConstantBuffer;   // Pointer to the constant buffer
    D3D11_BUFFER_DESC cbd = {};                  // Buffer description for constant buffer
    cbd.Usage = D3D11_USAGE_DYNAMIC;             // Default usage
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;  // Bind as a constant buffer
    cbd.ByteWidth = sizeof(ConstantBuffer);      // Size of the constant buffer in bytes
    cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // Allow CPU to update the buffer
    cbd.MiscFlags = 0;                           // No miscellaneous flags
    cbd.StructureByteStride = 0;                 // Not a structured buffer
    D3D11_SUBRESOURCE_DATA cbData = {};          // Initial data for the constant buffer
    cbData.pSysMem = &cb;                        // Pointer to the constant buffer data
    GFX_THROW_FAILED(pDevice->CreateBuffer(&cbd, &cbData, &pConstantBuffer));

    // Bind constant buffer to vertex shader
    pDeviceContext->VSSetConstantBuffers(0, 1, pConstantBuffer.GetAddressOf()); // Set the constant buffer to the vertex shader stage (register b0)

    // Create pixel shader
    wrl::ComPtr<ID3DBlob> pVSBlob;
    wrl::ComPtr<ID3D11PixelShader> pPixelShader;
    GFX_THROW_FAILED(D3DReadFileToBlob(L"PixelShader.cso", &pVSBlob));                                                           // Load the compiled pixel shader bytecode from a file (reusing pVSBlob for simplicity)
    GFX_THROW_FAILED(pDevice->CreatePixelShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &pPixelShader)); // Create the pixel shader from the compiled bytecode

    // bind pixel shader to pipeline
    pDeviceContext->PSSetShader(pPixelShader.Get(), nullptr, 0); // Set the pixel shader to the pipeline

    // create vertex shader
    wrl::ComPtr<ID3D11VertexShader> pVertexShader;                                                                                 // Pointer to the vertex shader
                                                                                                                                   // Pointer to the compiled vertex shader bytecode
    GFX_THROW_FAILED(D3DReadFileToBlob(L"VertexShader.cso", &pVSBlob));                                                            // Load the compiled vertex shader bytecode from a file
    GFX_THROW_FAILED(pDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &pVertexShader)); // Create the vertex shader from the compiled bytecode

    // bind vertex shader to pipeline
    pDeviceContext->VSSetShader(pVertexShader.Get(), nullptr, 0); // Set the vertex shader to the pipeline

    // input (vertex) layout
    wrl::ComPtr<ID3D11InputLayout> pInputLayout;
    const D3D11_INPUT_ELEMENT_DESC layout[] =
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}, // Define the input layout for the vertex shader (position attribute)
            {"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0}   // Define the input layout for the vertex shader (color attribute)
        };

    GFX_THROW_FAILED(pDevice->CreateInputLayout(layout, ARRAYSIZE(layout), pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &pInputLayout)); // Create the input layout

    // bind vertex layout to pipeline
    pDeviceContext->IASetInputLayout(pInputLayout.Get()); // Set the input layout to the input assembler stage

    // Bind Render Target View to the output merger stage
    pDeviceContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), nullptr); // Set the render target view to the output merger stage (no depth/stencil view)

    // Set primitive topology
    pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // Set the primitive topology to triangle list (each group of 3 vertices forms a triangle)

    // Configure Viewport
    D3D11_VIEWPORT vp = {};
    vp.Width = windowWidth;                 // Set the viewport width (should match the window width)
    vp.Height = windowHeight;               // Set the viewport height (should match the window height)
    vp.MinDepth = 0.0f;                     // Set the minimum depth
    vp.MaxDepth = 1.0f;                     // Set the maximum depth
    vp.TopLeftX = 0.0f;                     // Set the top-left X coordinate of the viewport
    vp.TopLeftY = 0.0f;                     // Set the top-left Y coordinate of the viewport
    pDeviceContext->RSSetViewports(1, &vp); // Set the viewport to the rasterizer stage

    pDeviceContext->DrawIndexed((UINT)std::size(indices), 0, 0);
    // Draw indexed triangles using the currently set vertex buffer and index buffer (this is just a placeholder, actual implementation would require setting up vertex buffers, index buffers, shaders, etc.)
}

void Graphics::EndFrame()
{
    pSwapChain->Present(1u, 0u); // Present the back buffer to the screen with vsync enabled (1 for vsync, 0 for no vsync)
}
