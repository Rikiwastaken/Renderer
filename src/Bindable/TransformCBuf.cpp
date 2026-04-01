#include "Bindable/TransformCbuf.h"

TransformCbuf::TransformCbuf(Graphics &gfx, const Drawable &parent)
    : pConstantBuffer(gfx),
      parent(parent) // Initialize the parent reference with the provided drawable object
{
}

void TransformCbuf::Bind(Graphics &gfx) noexcept
{
    pConstantBuffer.Update(gfx, DirectX::XMMatrixTranspose(parent.GetTransformXM() * gfx.GetProjection()));
    pConstantBuffer.Bind(gfx); // Update the constant buffer with the transposed transformation matrix and bind it to the graphics pipeline
}