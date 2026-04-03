#pragma once
#include "Graphics.h"
#include "Bindable/IndexBuffer.h"

class Bindable;

class Drawable
{
    template <class T>
    friend class DrawableBase;

public:
    Drawable() = default;
    Drawable(const Drawable &) = delete;                           // Delete copy constructor
    virtual ~Drawable() = default;                                 // Virtual destructor for proper cleanup of derived classes
    virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0; // Pure virtual function to get the transformation matrix for the drawable object

    virtual void Update(float dt) noexcept = 0; // Pure virtual function to update the drawable object (e.g., update constant buffers, etc.)
    void Draw(Graphics &gfx) const noexcept;    // Function to draw the drawable object using the graphics context
protected:
    void AddBind(std::unique_ptr<Bindable> bind) noexcept; // Function to add a bindable resource to the drawable object

    void AddIndexBuffer(std::unique_ptr<Bindable> IndexBuf) noexcept; // Function to add an index buffer to the drawable object (special handling for index buffers)
private:
    virtual const std::vector<std::unique_ptr<Bindable>> &GetStaticBinds() const noexcept = 0; // Pure virtual function to get the static binds associated with the drawable object (used for accessing shared resources)

private:
    const class IndexBuffer *pIndexBuffer = nullptr; // Pointer to the index buffer (if any)
    std::vector<std::unique_ptr<Bindable>> binds;    // Vector to hold the bindable resources associated with the drawable object
};