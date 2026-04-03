#pragma once
#include <cassert>
#include "Drawable/Drawable.h"
#include "Bindable/IndexBuffer.h"

template <class T>

class DrawableBase : public Drawable
{
public:
    bool isStaticInitialized() const noexcept
    {
        return !staticBinds.empty(); // Check if the static binds vector is not empty, indicating that static resources have been initialized
    }
    void AddStaticBind(std::unique_ptr<Bindable> bind) noexcept
    {
        assert("*Must* use AddIndexBuffer for index buffers" && typeid(*bind) != typeid(IndexBuffer)); // Ensure that the bind being added is not an index buffer (index buffers should be added using AddIndexBuffer)
        staticBinds.push_back(std::move(bind));                                                        // Add a bindable resource to the static binds vector (used for resources that are shared across all instances of the drawable)
    }
    void AddStaticIndexBuffer(std::unique_ptr<IndexBuffer> IndexBuf) noexcept
    {
        assert(pIndexBuffer == nullptr);            // Ensure that the bind being added is an index buffer (index buffers should be added using AddIndexBuffer)
        pIndexBuffer = IndexBuf.get();              // Set the static index buffer pointer to the provided index buffer
        staticBinds.push_back(std::move(IndexBuf)); // Add the index buffer to the static binds vector
    }
    void SetIndexFromStatic() noexcept
    {
        assert("Attempting to add index buffer a second time" && pIndexBuffer == nullptr); // Ensure that the static index buffer has been set before trying to use it
        for (const auto &b : staticBinds)
        {
            if (const auto p = dynamic_cast<IndexBuffer *>(b.get()))
            {
                pIndexBuffer = p;
                return;
            }
        }
        assert("Static index buffer not found in static binds vector" && false); // If we reach this point, it means the static index buffer was not found in the static binds vector, which is an error
    }

private:
    const std::vector<std::unique_ptr<Bindable>> &GetStaticBinds() const noexcept
    {
        return staticBinds; // Return a reference to the static binds vector (used for accessing shared resources)
    }

private:
    static std::vector<std::unique_ptr<Bindable>> staticBinds; // Static vector to hold bindable resources that are shared across all instances of the drawable (e.g., shaders, input layouts, etc.)
};
template <class T>
std::vector<std::unique_ptr<Bindable>> DrawableBase<T>::staticBinds;