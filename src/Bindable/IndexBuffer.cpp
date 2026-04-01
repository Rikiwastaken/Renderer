#include "Bindable/IndexBuffer.h"

IndexBuffer::IndexBuffer(Graphics &gfx, const std::vector<unsigned short> &indices)
    : count(static_cast<UINT>(indices.size()))
{
    INFOMAN(gfx);
    D3D11_BUFFER_DESC ibd = {};                       // Buffer description for index buffer
    ibd.Usage = D3D11_USAGE_DEFAULT;                  // Default usage
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;          // Bind as an index
    ibd.ByteWidth = sizeof(indices);                  // Size of the index buffer in bytes
    ibd.CPUAccessFlags = 0;                           // No CPU access needed
    ibd.MiscFlags = 0;                                // No miscellaneous flags
    ibd.StructureByteStride = sizeof(unsigned short); // Size of each index
    D3D11_SUBRESOURCE_DATA indexData = {};            // Initial data for the index
    indexData.pSysMem = indices.data();               // Pointer to the index data
    GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&ibd, &indexData, &pIndexBuffer));
}

void IndexBuffer::Bind(Graphics &gfx) noexcept
{
    GetContext(gfx)->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0); // Bind the index buffer to the input assembler stage with 16-bit unsigned integer format
}

UINT IndexBuffer::GetCount() const noexcept
{
    return count; // Return the count of indices in the buffer
}