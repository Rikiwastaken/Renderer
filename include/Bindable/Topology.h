#pragma once
#include "Bindable/Bindable.h"

class Topology : public Bindable
{
public:
    Topology(Graphics &gfx, D3D11_PRIMITIVE_TOPOLOGY topology);
    void Bind(Graphics &gfx) noexcept override; // Override the Bind function to set the primitive topology in the graphics pipeline
protected:
    D3D11_PRIMITIVE_TOPOLOGY topology; // The primitive topology to be set in the graphics pipeline (e.g., D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, D3D11_PRIMITIVE_TOPOLOGY_LINELIST, etc.)
};
