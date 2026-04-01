#include "Bindable/InputLayout.h"

InputLayout::InputLayout(Graphics &gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC> &ied, ID3DBlob *pShaderBytecode)
{
    INFOMAN(gfx);
    GFX_THROW_INFO(GetDevice(gfx)->CreateInputLayout(ied.data(), static_cast<UINT>(ied.size()), pShaderBytecode->GetBufferPointer(), pShaderBytecode->GetBufferSize(), &pInputLayout)); // Create the input layout based on the provided input element descriptions and shader bytecode
}

void InputLayout::Bind(Graphics &gfx) noexcept
{
    GetContext(gfx)->IASetInputLayout(pInputLayout.Get()); // Bind the input layout to the input assembler stage of the graphics pipeline
}
