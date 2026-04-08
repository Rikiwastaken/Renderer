#include "Bindable/TransformCbuf.h"

TransformCbuf::TransformCbuf(Graphics &gfx, const Drawable &parent)
    : parent(parent) // Initialize the parent reference with the provided drawable object
{
    if (!pConstantBuffer)
    {
        pConstantBuffer = std::make_unique<VertexConstantBuffer<DirectX::XMMATRIX>>(gfx); // Create the constant buffer for the transformation matrix if it hasn't been created yet
    }
}

void TransformCbuf::Bind(Graphics &gfx) noexcept
{
    pConstantBuffer->Update(gfx, DirectX::XMMatrixTranspose(parent.GetTransformXM() * gfx.GetCamera() * gfx.GetProjection()));
    pConstantBuffer->Bind(gfx); // Update the constant buffer with the transposed transformation matrix and bind it to the graphics pipeline
}

std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> TransformCbuf::pConstantBuffer; // Initialize the static constant buffer pointer to nullptr