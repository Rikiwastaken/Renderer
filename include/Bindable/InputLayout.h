#pragma once
#include "Bindable/Bindable.h"

class InputLayout : public Bindable
{
public:
    InputLayout(Graphics &gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC> &ied, ID3DBlob *pShaderBytecode);
    void Bind(Graphics &gfx) noexcept override; // Override the Bind function to bind the input layout to the graphics pipeline

protected:
    Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout; // Pointer to the Direct3D input layout
};