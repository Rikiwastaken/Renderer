#include "Bindable/Topology.h"

Topology::Topology(Graphics &gfx, D3D11_PRIMITIVE_TOPOLOGY topology)
    : topology(topology) // Initialize the topology member variable with the provided topology
{
}

void Topology::Bind(Graphics &gfx) noexcept
{
    GetContext(gfx)->IASetPrimitiveTopology(topology); // Set the primitive topology in the input assembler stage of the graphics pipeline
}