#include "Bindable/VertexBuffer.h"

void VertexBuffer::Bind(Graphics &gfx) noexcept
{

    const UINT offset = 0;                                                                     // Offset in the vertex buffer (starting point for binding)
    GetContext(gfx)->IASetVertexBuffers(0, 1, pVertexBuffer.GetAddressOf(), &stride, &offset); // Bind the vertex buffer to the input assembler stage with the specified stride and offset
}