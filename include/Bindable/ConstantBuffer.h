#pragma once
#include "Bindable.h"

template <class C>
class ConstantBuffer : public Bindable
{
public:
    void Update(Graphics &gfx, const C &consts)
    {

        INFOMAN(gfx);
        D3D11_MAPPED_SUBRESOURCE msr;                                                                     // Structure to hold the mapped resource data
        GFX_THROW_INFO(GetContext(gfx)->Map(pConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr)); // Map the constant buffer to gain write access
        memcpy(msr.pData, &consts, sizeof(consts));                                                       // Copy the new constant data into the mapped resource
        GetContext(gfx)->Unmap(pConstantBuffer.Get(), 0);                                                 // Unmap the constant buffer to apply the changes
    }
    ConstantBuffer(Graphics &gfx, const C &consts)
    {
        INFOMAN(gfx);
        D3D11_BUFFER_DESC cbd = {};                                                    // Buffer description for constant buffer
        cbd.Usage = D3D11_USAGE_DYNAMIC;                                               // Default usage
        cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;                                    // Bind as a constant buffer
        cbd.ByteWidth = sizeof(consts);                                                // Size of the constant buffer in bytes
        cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;                                   // Allow CPU to update the buffer
        cbd.MiscFlags = 0;                                                             // No miscellaneous flags
        cbd.StructureByteStride = 0;                                                   // Not a structured buffer
        D3D11_SUBRESOURCE_DATA cbData = {};                                            // Initial data for the constant buffer
        cbData.pSysMem = &consts;                                                      // Pointer to the constant buffer data
        GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&cbd, &cbData, &pConstantBuffer)); // Create the constant buffer with the initial data

        // Bind constant buffer to vertex shader
        GetContext(gfx)->VSSetConstantBuffers(0, 1, pConstantBuffer.GetAddressOf()); // Set the constant buffer to the vertex shader stage (register b0)
    }
    ConstantBuffer(Graphics &gfx)
    {
        INFOMAN(gfx);
        D3D11_BUFFER_DESC cbd = {};                                                    // Buffer description for constant buffer
        cbd.Usage = D3D11_USAGE_DYNAMIC;                                               // Default usage
        cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;                                    // Bind as a constant buffer
        cbd.ByteWidth = sizeof(C);                                                     // Size of the constant buffer in bytes
        cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;                                   // Allow CPU to update the buffer
        cbd.MiscFlags = 0;                                                             // No miscellaneous flags
        cbd.StructureByteStride = 0;                                                   // Not a structured buffer                                                      // No initial data
        GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&cbd, nullptr, &pConstantBuffer)); // Create the constant buffer without initial data

        // Bind constant buffer to vertex shader
        GetContext(gfx)->VSSetConstantBuffers(0, 1, pConstantBuffer.GetAddressOf()); // Set the constant buffer to the vertex shader stage (register b0)
    }

protected:
    Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer; // Pointer to the Direct3D constant buffer
};

template <class C>
class VertexConstantBuffer : public ConstantBuffer<C>
{
    using ConstantBuffer<C>::pConstantBuffer; // Inherit constructors from the base class
    using Bindable::GetContext;

public:
    using ConstantBuffer<C>::ConstantBuffer; // Inherit constructors from the base class
    void Bind(Graphics &gfx) noexcept override
    {
        GetContext(gfx)->VSSetConstantBuffers(0, 1, pConstantBuffer.GetAddressOf()); // Bind the constant buffer to the vertex shader stage (register b0)
    }
};

template <class C>
class PixelConstantBuffer : public ConstantBuffer<C>
{
    using ConstantBuffer<C>::pConstantBuffer; // Inherit constructors from the base class
    using Bindable::GetContext;

public:
    using ConstantBuffer<C>::ConstantBuffer; // Inherit constructors from the base class
    void Bind(Graphics &gfx) noexcept override
    {
        GetContext(gfx)->PSSetConstantBuffers(0, 1, pConstantBuffer.GetAddressOf()); // Bind the constant buffer to the pixel shader stage (register b0)
    }
};