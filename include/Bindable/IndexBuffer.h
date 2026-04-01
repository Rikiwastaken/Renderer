#pragma once
#include "Bindable.h"

class IndexBuffer : public Bindable
{
public:
    IndexBuffer(Graphics &gfx, const std::vector<unsigned short> &indices); // Constructor to create an index buffer from a vector of indices
    void Bind(Graphics &gfx) noexcept override;                             // Override the Bind function to bind the index buffer to the graphics pipeline
    UINT GetCount() const noexcept;                                         // Function to get the count of indices in the buffer
protected:
    UINT count;                                        // Count of indices in the buffer
    Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer; // Pointer to the Direct3D index buffer
};