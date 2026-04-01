#include "Bindable/VertexShader.h"

namespace wrl = Microsoft::WRL;

void VertexShader::Bind(Graphics &gfx) noexcept
{
    GetContext(gfx)->VSSetShader(pVertexShader.Get(), nullptr, 0); // Set the vertex shader to the pipeline
}

VertexShader::VertexShader(Graphics &gfx, const std::wstring &path)
{
    INFOMAN(gfx);
    GFX_THROW_INFO(D3DReadFileToBlob(path.c_str(), &pVSBlob));                                                                          // Load the compiled vertex shader bytecode from the specified file
    GFX_THROW_INFO(GetDevice(gfx)->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &pVertexShader)); // Create the vertex shader from the compiled bytecode
}

ID3DBlob *VertexShader::GetBytecode() const noexcept
{
    return pVSBlob.Get(); // Return a pointer to the compiled vertex shader bytecode (used for input layout creation)
}