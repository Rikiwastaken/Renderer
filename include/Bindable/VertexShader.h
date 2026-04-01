#pragma once
#include "Bindable.h"

class VertexShader : public Bindable
{
public:
    VertexShader(Graphics &gfx, const std::wstring &path);
    void Bind(Graphics &gfx) noexcept override; // Override the Bind function to bind the vertex shader to the graphics pipeline
    ID3DBlob *GetBytecode() const noexcept;     // Function to get the compiled vertex shader bytecode (used for input layout creation)
protected:
    Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader; // Pointer to the Direct3D vertex shader
    Microsoft::WRL::ComPtr<ID3DBlob> pVSBlob;                 // Pointer to hold the compiled vertex shader bytecode
};