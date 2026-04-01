#pragma once
#include "Bindable.h"

class VertexBuffer : public Bindable
{
public:
    template <class V>
    VertexBuffer(Graphics &gfx, const std::vector<V> &vertices)
        : stride(sizeof(V))
    {
        INFOMAN(gfx);
        D3D11_BUFFER_DESC bd = {};                                     // Buffer description
        bd.Usage = D3D11_USAGE_DEFAULT;                                // Default usage
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;                       // Bind as a vertex buffer
        bd.ByteWidth = sizeof(V) * static_cast<UINT>(vertices.size()); // Size of the buffer in bytes
        bd.CPUAccessFlags = 0;                                         // No CPU access needed
        bd.MiscFlags = 0;                                              // No miscellaneous flags
        bd.StructureByteStride = sizeof(V);                            // Size of each vertex structure
        D3D11_SUBRESOURCE_DATA initData = {};                          // Initial data for the buffer
        initData.pSysMem = vertices.data();                            // Pointer to the vertex data
        GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&bd, &initData, &pVertexBuffer));
    }
    void Bind(Graphics &gfx) noexcept override; // Override the Bind function to bind the vertex buffer to the graphics pipeline
protected:
    UINT stride;                                        // Stride of each vertex in the buffer
    Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer; // Pointer to the Direct3D vertex buffer
};