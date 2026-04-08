#include "Graphics.h"
#include <cmath>
#include <DirectXMath.h>
#include <vector>
#include <sstream>
#include "GraphicsThrowMacros.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

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

    GFX_THROW_INFO_ONLY(D3D11CreateDeviceAndSwapChain(
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
    GFX_THROW_INFO_ONLY(pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));                // Pointer to the back buffer resource
    GFX_THROW_INFO_ONLY(pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pRenderTargetView)); // Create a render target view for the back buffer                                                                           // Release the back buffer resource as it's no longer needed after creating the render target view

    // Z Buffer
    D3D11_DEPTH_STENCIL_DESC dsDesc = {};                                      // Depth stencil state description
    dsDesc.DepthEnable = TRUE;                                                 // Enable depth testing
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;                        // Allow writing to the depth buffer
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS;                                  // Use less comparison function for depth testing
    wrl::ComPtr<ID3D11DepthStencilState> pDSState;                             // Pointer to the depth stencil state
    GFX_THROW_INFO_ONLY(pDevice->CreateDepthStencilState(&dsDesc, &pDSState)); // Create the depth stencil state
    pDeviceContext->OMSetDepthStencilState(pDSState.Get(), 1);                 // Set the depth stencil state to the output merger stage (using a stencil reference value of 1)

    // creeate depth stensil texture
    wrl::ComPtr<ID3D11Texture2D> pDepthStencil;            // Pointer to the depth stencil texture
    D3D11_TEXTURE2D_DESC depthStencilDesc = {};            // Depth stencil texture description
    depthStencilDesc.Width = 800.0f;                       // Set the width of the depth stencil texture (should match the window's client area width)
    depthStencilDesc.Height = 600.0f;                      // Set the height of the depth stencil texture (should match the window's client area height)
    depthStencilDesc.MipLevels = 1;                        // Set the number of mipmap levels
    depthStencilDesc.ArraySize = 1;                        // Set the size of the texture array
    depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;       // Set the format of the depth stencil texture
    depthStencilDesc.SampleDesc.Count = 1;                 // Set the number of samples for multisampling
    depthStencilDesc.SampleDesc.Quality = 0;               // Set the quality level for multisampling
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;          // Set the usage of the texture
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL; // Set the bind flags for the depth stencil texture
    depthStencilDesc.CPUAccessFlags = 0;                   // Set the CPU access flags
    depthStencilDesc.MiscFlags = 0;                        // Set the miscellaneous flags

    GFX_THROW_INFO_ONLY(pDevice->CreateTexture2D(&depthStencilDesc, nullptr, &pDepthStencil)); // Create the depth stencil texture

    // create depth stencil view
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {}; // Depth stencil view description
    dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Texture2D.MipSlice = 0;
    GFX_THROW_INFO_ONLY(pDevice->CreateDepthStencilView(pDepthStencil.Get(), &dsvDesc, &pDSV)); // Create a depth stencil view for the depth stencil texture

    // bind depth stencil view to output merger stage
    pDeviceContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), pDSV.Get()); // Bind the render target view and depth stencil view to the output merger stage

    D3D11_RASTERIZER_DESC rasterDesc = {};
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.CullMode = D3D11_CULL_NONE;
    rasterDesc.DepthClipEnable = TRUE;
    GFX_THROW_INFO_ONLY(pDevice->CreateRasterizerState(&rasterDesc, &pRasterizerState));
    pDeviceContext->RSSetState(pRasterizerState.Get());

    RECT clientRect;
    GetClientRect(hWnd, &clientRect);
    D3D11_VIEWPORT vp = {};
    vp.Width = static_cast<float>(clientRect.right - clientRect.left);
    vp.Height = static_cast<float>(clientRect.bottom - clientRect.top);
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0.0f;
    vp.TopLeftY = 0.0f;
    pDeviceContext->RSSetViewports(1, &vp);
}

void Graphics::BeginFrame(float r, float g, float b) noexcept // Function to clear the back buffer with a specified color
{
    // imgui begin frame
    if (imguiEnabled)
    {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }
    const float color[] = {r, g, b, 1.0f};                                         // RGBA color array
    pDeviceContext->ClearRenderTargetView(pRenderTargetView.Get(), color);         // Clear the render target view with the specified color
    pDeviceContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0); // Clear the depth stencil view (clearing only the depth buffer with a value of 1.0f)
}

void Graphics::DrawTestCube(float angle, float x, float y, float z, float scale, float windowWidth, float windowHeight)
{

    struct Vertex
    {
        struct
        {
            float x, y, z; // Position
        } pos;
    };

    // verteces for a cube
    const Vertex vertices[] =
        {
            {-1.0f, -1.0f, -1.0f},
            {1.0f, -1.0f, -1.0f},
            {-1.0f, 1.0f, -1.0f},
            {1.0f, 1.0f, -1.0f},
            {-1.0f, -1.0f, 1.0f},
            {1.0f, -1.0f, 1.0f},
            {-1.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, 1.0f}};

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
    GFX_THROW_INFO_ONLY(pDevice->CreateBuffer(&bd, &initData, &pVertexBuffer));

    // Create index buffer
    const unsigned short indices[] =
        {
            0, 2, 1, 2, 3, 1, // back face
            1, 3, 5, 3, 7, 5, // right face
            2, 6, 3, 3, 6, 7, // front face
            4, 5, 7, 4, 7, 6, // left face
            0, 4, 2, 2, 4, 6, // bottom face
            0, 1, 4, 1, 5, 4  // top face
        };

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
    GFX_THROW_INFO_ONLY(pDevice->CreateBuffer(&ibd, &indexData, &pIndexBuffer));

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
            dx::XMMatrixScaling(scale, scale, scale) *
            dx::XMMatrixRotationZ(angle) *
            dx::XMMatrixRotationY(angle) *
            dx::XMMatrixTranslation(x, y, z) *
            dx::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 10.0f)) // Example transformation: rotate around Z-axis and scale down;
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
    GFX_THROW_INFO_ONLY(pDevice->CreateBuffer(&cbd, &cbData, &pConstantBuffer));

    // Bind constant buffer to vertex shader
    pDeviceContext->VSSetConstantBuffers(0, 1, pConstantBuffer.GetAddressOf()); // Set the constant buffer to the vertex shader stage (register b0)

    struct ConstantBuffer2
    {
        struct
        {
            float r, g, b, a; // Color
        } face_colors[6];
    };

    const ConstantBuffer2 cb2 = {
        {
            {1.0f, 0.0f, 0.0f, 1.0f}, // Red
            {0.0f, 1.0f, 0.0f, 1.0f}, // Green
            {0.0f, 0.0f, 1.0f, 1.0f}, // Blue
            {1.0f, 1.0f, 0.0f, 1.0f}, // Yellow
            {1.0f, 0.0f, 1.0f, 1.0f}, // Magenta
            {0.0f, 1.0f, 1.0f, 1.0f}  // Cyan
        }};

    wrl::ComPtr<ID3D11Buffer> pConstantBuffer2;  // Pointer to the second constant buffer
    D3D11_BUFFER_DESC cbd2 = {};                 // Buffer description for the second constant
    cbd2.Usage = D3D11_USAGE_DEFAULT;            // Default usage
    cbd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // Bind as a constant buffer
    cbd2.ByteWidth = sizeof(cb2);                // Size of the second constant buffer in bytes
    cbd2.CPUAccessFlags = 0;                     // No CPU access needed
    cbd2.MiscFlags = 0;                          // No miscellaneous flags
    cbd2.StructureByteStride = 0;                // Not a structured buffer
    D3D11_SUBRESOURCE_DATA cbData2 = {};         // Initial data for the second constant buffer
    cbData2.pSysMem = &cb2;                      // Pointer to the second constant buffer data
    GFX_THROW_INFO_ONLY(pDevice->CreateBuffer(&cbd2, &cbData2, &pConstantBuffer2));

    // Bind second constant buffer to pixel shader
    pDeviceContext->PSSetConstantBuffers(0, 1, pConstantBuffer2.GetAddressOf()); // Set the second constant buffer to the pixel shader stage (register b0)

    // Create pixel shader
    wrl::ComPtr<ID3DBlob>
        pVSBlob;
    wrl::ComPtr<ID3D11PixelShader> pPixelShader;
    GFX_THROW_INFO_ONLY(D3DReadFileToBlob(L"PixelShader.cso", &pVSBlob));                                                           // Load the compiled pixel shader bytecode from a file (reusing pVSBlob for simplicity)
    GFX_THROW_INFO_ONLY(pDevice->CreatePixelShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &pPixelShader)); // Create the pixel shader from the compiled bytecode

    // bind pixel shader to pipeline
    pDeviceContext->PSSetShader(pPixelShader.Get(), nullptr, 0); // Set the pixel shader to the pipeline

    // create vertex shader
    wrl::ComPtr<ID3D11VertexShader> pVertexShader;                                                                                    // Pointer to the vertex shader
                                                                                                                                      // Pointer to the compiled vertex shader bytecode
    GFX_THROW_INFO_ONLY(D3DReadFileToBlob(L"VertexShader.cso", &pVSBlob));                                                            // Load the compiled vertex shader bytecode from a file
    GFX_THROW_INFO_ONLY(pDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &pVertexShader)); // Create the vertex shader from the compiled bytecode

    // bind vertex shader to pipeline
    pDeviceContext->VSSetShader(pVertexShader.Get(), nullptr, 0); // Set the vertex shader to the pipeline

    // input (vertex) layout
    wrl::ComPtr<ID3D11InputLayout> pInputLayout;
    const D3D11_INPUT_ELEMENT_DESC layout[] =
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}, // Define the input layout for the vertex shader (position attribute)
        };

    GFX_THROW_INFO_ONLY(pDevice->CreateInputLayout(layout, ARRAYSIZE(layout), pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &pInputLayout)); // Create the input layout

    // bind vertex layout to pipeline
    pDeviceContext->IASetInputLayout(pInputLayout.Get()); // Set the input layout to the input assembler stage

    // Bind Render Target View to the output merger stage
    pDeviceContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), pDSV.Get()); // Set the render target view to the output merger stage (no depth/stencil view)

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

void Graphics::DrawIndexed(UINT indexCount)
{
    GFX_THROW_INFO_ONLY(pDeviceContext->DrawIndexed(indexCount, 0, 0)); // Draw indexed geometry using the currently bound vertex and index buffers (indexCount specifies the number of indices to draw)
}

void Graphics::DrawD10(float angle, float x, float y, float z, float scale, float windowWidth, float windowHeight)
{
    struct Vertex
    {
        struct
        {
            float x, y, z; // Position
        } pos;
    };

    const float PI = 3.1415926535f;

    Vertex vertices[7]; // Array to hold the vertices of the D10 (trapezohedron)

    // verteces for a cube
    // Top pole
    vertices[0] = {0.0f, 1.0f, 0.0f};

    // Bottom pole
    vertices[1] = {0.0f, -1.0f, 0.0f};

    // Ring vertices (5 points)
    for (int i = 0; i < 5; i++)
    {
        float angle = i * 2.0f * PI / 5.0f; // 72 degrees per vertex

        float x = cosf(angle);
        float z = sinf(angle);

        vertices[i + 2] = {x, 0.0f, z};
    }

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
    GFX_THROW_INFO_ONLY(pDevice->CreateBuffer(&bd, &initData, &pVertexBuffer));

    unsigned short indices[60]; // Array to hold the indices of the D10 (trapezohedron)
    int indexCount = 0;

    for (int i = 0; i < 5; i++)
    {
        int next = (i + 1) % 5;

        indices[indexCount++] = 0; // top pole
        indices[indexCount++] = 2 + i;
        indices[indexCount++] = 2 + next;
    }

    for (int i = 0; i < 5; i++)
    {
        int next = (i + 1) % 5;

        indices[indexCount++] = 1; // bottom pole
        indices[indexCount++] = 2 + next;
        indices[indexCount++] = 2 + i;
    }

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
    GFX_THROW_INFO_ONLY(pDevice->CreateBuffer(&ibd, &indexData, &pIndexBuffer));

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
            dx::XMMatrixScaling(scale, scale, scale) *
            dx::XMMatrixRotationZ(angle) *
            dx::XMMatrixRotationY(angle) *
            dx::XMMatrixTranslation(x, y, z) *
            dx::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 10.0f))};

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
    GFX_THROW_INFO_ONLY(pDevice->CreateBuffer(&cbd, &cbData, &pConstantBuffer));

    // Bind constant buffer to vertex shader
    pDeviceContext->VSSetConstantBuffers(0, 1, pConstantBuffer.GetAddressOf()); // Set the constant buffer to the vertex shader stage (register b0)

    struct ConstantBuffer2
    {
        struct
        {
            float r, g, b, a; // Color
        } face_colors[10];
    };

    const ConstantBuffer2 cb2 = {
        {
            {1.0f, 0.0f, 0.0f, 1.0f}, // Red
            {0.0f, 1.0f, 0.0f, 1.0f}, // Green
            {0.0f, 0.0f, 1.0f, 1.0f}, // Blue
            {1.0f, 1.0f, 0.0f, 1.0f}, // Yellow
            {1.0f, 0.0f, 1.0f, 1.0f}, // Magenta
            {0.0f, 1.0f, 1.0f, 1.0f}, // Cyan
            {1.0f, 0.5f, 0.0f, 1.0f}, // Orange
            {0.5f, 0.0f, 1.0f, 1.0f}, // Purple
            {0.5f, 0.5f, 0.5f, 1.0f}, // Gray
            {1.0f, 1.0f, 1.0f, 1.0f}  // White
        }};

    wrl::ComPtr<ID3D11Buffer> pConstantBuffer2;  // Pointer to the second constant buffer
    D3D11_BUFFER_DESC cbd2 = {};                 // Buffer description for the second constant
    cbd2.Usage = D3D11_USAGE_DEFAULT;            // Default usage
    cbd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // Bind as a constant buffer
    cbd2.ByteWidth = sizeof(cb2);                // Size of the second constant buffer in bytes
    cbd2.CPUAccessFlags = 0;                     // No CPU access needed
    cbd2.MiscFlags = 0;                          // No miscellaneous flags
    cbd2.StructureByteStride = 0;                // Not a structured buffer
    D3D11_SUBRESOURCE_DATA cbData2 = {};         // Initial data for the second constant buffer
    cbData2.pSysMem = &cb2;                      // Pointer to the second constant buffer data
    GFX_THROW_INFO_ONLY(pDevice->CreateBuffer(&cbd2, &cbData2, &pConstantBuffer2));

    // Bind second constant buffer to pixel shader
    pDeviceContext->PSSetConstantBuffers(0, 1, pConstantBuffer2.GetAddressOf()); // Set the second constant buffer to the pixel shader stage (register b0)

    // Create pixel shader
    wrl::ComPtr<ID3DBlob>
        pVSBlob;
    wrl::ComPtr<ID3D11PixelShader> pPixelShader;
    GFX_THROW_INFO_ONLY(D3DReadFileToBlob(L"PixelShader.cso", &pVSBlob));                                                           // Load the compiled pixel shader bytecode from a file (reusing pVSBlob for simplicity)
    GFX_THROW_INFO_ONLY(pDevice->CreatePixelShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &pPixelShader)); // Create the pixel shader from the compiled bytecode

    // bind pixel shader to pipeline
    pDeviceContext->PSSetShader(pPixelShader.Get(), nullptr, 0); // Set the pixel shader to the pipeline

    // create vertex shader
    wrl::ComPtr<ID3D11VertexShader> pVertexShader;                                                                                    // Pointer to the vertex shader
                                                                                                                                      // Pointer to the compiled vertex shader bytecode
    GFX_THROW_INFO_ONLY(D3DReadFileToBlob(L"VertexShader.cso", &pVSBlob));                                                            // Load the compiled vertex shader bytecode from a file
    GFX_THROW_INFO_ONLY(pDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &pVertexShader)); // Create the vertex shader from the compiled bytecode

    // bind vertex shader to pipeline
    pDeviceContext->VSSetShader(pVertexShader.Get(), nullptr, 0); // Set the vertex shader to the pipeline

    // input (vertex) layout
    wrl::ComPtr<ID3D11InputLayout> pInputLayout;
    const D3D11_INPUT_ELEMENT_DESC layout[] =
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}, // Define the input layout for the vertex shader (position attribute)
        };

    GFX_THROW_INFO_ONLY(pDevice->CreateInputLayout(layout, ARRAYSIZE(layout), pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &pInputLayout)); // Create the input layout

    // bind vertex layout to pipeline
    pDeviceContext->IASetInputLayout(pInputLayout.Get()); // Set the input layout to the input assembler stage

    // Bind Render Target View to the output merger stage
    pDeviceContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), pDSV.Get()); // Set the render target view to the output merger stage (no depth/stencil view)

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
}

void Graphics::SetProjection(DirectX::XMMATRIX proj) noexcept
{
    projection = proj;
}

DirectX::XMMATRIX Graphics::GetProjection() const noexcept
{
    return projection;
}

void Graphics::EndFrame()
{
    if (imguiEnabled)
    {
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }
    infoManager.Set();
    pSwapChain->Present(1u, 0u); // Present the back buffer to the screen with vsync enabled (1 for vsync, 0 for no vsync)
}

// Graphics exception stuff
Graphics::HrException::HrException(int line, const char *file, HRESULT hr, std::vector<std::string> infoMsgs) noexcept
    : Exception(line, file),
      hr(hr)
{
    // join all info messages with newlines into single string
    for (const auto &m : infoMsgs)
    {
        info += m;
        info.push_back('\n');
    }
    // remove final newline if exists
    if (!info.empty())
    {
        info.pop_back();
    }
}

const char *Graphics::HrException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << std::endl
        << "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
        << std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
        << "[Error String] " << GetErrorString() << std::endl
        << "[Description] " << GetErrorDescription() << std::endl;
    if (!info.empty())
    {
        oss << "\n[Error Info]\n"
            << GetErrorInfo() << std::endl
            << std::endl;
    }
    oss << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char *Graphics::HrException::GetType() const noexcept
{
    return "Chili Graphics Exception";
}

HRESULT Graphics::HrException::GetErrorCode() const noexcept
{
    return hr;
}

std::string Graphics::HrException::GetErrorString() const noexcept
{
    return DXGetErrorString(hr);
}

std::string Graphics::HrException::GetErrorDescription() const noexcept
{
    char buf[512];
    DXGetErrorDescription(hr, buf, sizeof(buf));
    return buf;
}

std::string Graphics::HrException::GetErrorInfo() const noexcept
{
    return info;
}

const char *Graphics::DeviceRemovedException::GetType() const noexcept
{
    return "Chili Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
}
Graphics::InfoException::InfoException(int line, const char *file, std::vector<std::string> infoMsgs) noexcept
    : Exception(line, file)
{
    // join all info messages with newlines into single string
    for (const auto &m : infoMsgs)
    {
        info += m;
        info.push_back('\n');
    }
    // remove final newline if exists
    if (!info.empty())
    {
        info.pop_back();
    }
}

const char *Graphics::InfoException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << std::endl
        << "\n[Error Info]\n"
        << GetErrorInfo() << std::endl
        << std::endl;
    oss << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char *Graphics::InfoException::GetType() const noexcept
{
    return "Chili Graphics Info Exception";
}

std::string Graphics::InfoException::GetErrorInfo() const noexcept
{
    return info;
}

void Graphics::EnableImGui() noexcept
{
    imguiEnabled = true;
}

void Graphics::DisableImGui() noexcept
{
    imguiEnabled = false;
}

bool Graphics::IsImGuiEnabled() const noexcept
{
    return imguiEnabled;
}

Graphics::~Graphics()
{
    if (imguiEnabled)
    {
        ImGui_ImplDX11_Shutdown();
    }
}

void Graphics::SetCamera(DirectX::XMMATRIX cam) noexcept
{
    camera = cam;
}

DirectX::XMMATRIX Graphics::GetCamera() const noexcept
{
    return camera;
}