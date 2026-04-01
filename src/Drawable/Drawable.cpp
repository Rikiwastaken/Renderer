#include "Drawable/Drawable.h"
#include <cassert>
#include <typeinfo>
#include <iostream>

void Drawable::AddBind(std::unique_ptr<Bindable> bind) noexcept
{
    assert("*Must* use AddIndexBuffer to bind index buffer" && typeid(*bind) != typeid(IndexBuffer));
    binds.push_back(std::move(bind)); // Add the bind to the vector of binds, transferring ownership
}

void Drawable::Draw(Graphics &gfx) const noexcept
{
    int i = 0;
    for (auto &b : binds)
    {
        std::cout << "Drawable::Draw() - Binding resource " << i++ << " of type " << typeid(*b).name() << std::endl;
        b->Bind(gfx); // Bind each resource in the vector of binds to the graphics pipeline
    }
    gfx.DrawIndexed(pIndexBuffer->GetCount());
}

void Drawable::AddIndexBuffer(std::unique_ptr<Bindable> IndexBuf) noexcept
{
    assert("Attempting to add index buffer a second time" && pIndexBuffer == nullptr);
    pIndexBuffer = dynamic_cast<const IndexBuffer *>(IndexBuf.get()); // Set the index buffer pointer to the provided index buffer
    binds.push_back(std::move(IndexBuf));                             // Add the index buffer to the vector of binds, transferring ownership
}