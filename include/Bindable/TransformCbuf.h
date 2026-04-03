#pragma once
#include "Drawable/Drawable.h"
#include "Bindable/ConstantBuffer.h"
#include <DirectXMath.h>

class TransformCbuf : public Bindable
{
public:
    TransformCbuf(Graphics &gfx, const Drawable &parent);
    void Bind(Graphics &gfx) noexcept override;

private:
    static std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> pConstantBuffer; // Constant buffer to hold the transformation matrix for the drawable object
    const Drawable &parent;                                                          // Reference to the parent drawable object to access its transformation matrix
};