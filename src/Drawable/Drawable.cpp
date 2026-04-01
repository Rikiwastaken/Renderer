#include "Drawable/Drawable.h"
#include <cassert>
#include <typeinfo>

void Drawable::AddBind(std::unique_ptr<Bindable> bind) noexcept
{
    assert("Trying to add a null bind" && bind != nullptr); // Ensure that the bind being added is not null
    binds.push_back(std::move(bind));                       // Add the bind to the vector of binds, transferring ownership
}

void Drawable::Draw(Graphics &gfx) noexcept
{
    for (auto &b : binds)
    {
        b->Bind(gfx); // Bind each resource in the vector of binds to the graphics pipeline
    }
    gfx.DrawIndexed(pIndexBuffer->GetCount());
}

void Drawable::AddIndexBuffer(std::unique_ptr<Bindable> IndexBuf) noexcept
{
    assert("Attempting to add index buffer a second time" && pIndexBuffer == nullptr); // Ensure that an index buffer is not being added more than once
    pIndexBuffer = dynamic_cast<const IndexBuffer *>(IndexBuf.get());                  // Set the index buffer pointer to the provided index buffer
    binds.push_back(std::move(IndexBuf));                                              // Add the index buffer to the vector of binds, transferring ownership
}