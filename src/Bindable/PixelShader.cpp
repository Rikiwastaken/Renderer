#include "Bindable/PixelShader.h"
namespace wrl = Microsoft::WRL;

void PixelShader::Bind(Graphics &gfx) noexcept
{
    GetContext(gfx)->PSSetShader(pPixelShader.Get(), nullptr, 0); // Set the pixel shader to the pipeline
}

PixelShader::PixelShader(Graphics &gfx, const std::wstring &path)
{
    INFOMAN(gfx);
    wrl::ComPtr<ID3DBlob> pPSBlob;                                                                                                    // Pointer to hold the compiled pixel shader bytecode
    GFX_THROW_INFO(D3DReadFileToBlob(path.c_str(), &pPSBlob));                                                                        // Load the compiled pixel shader bytecode from the specified file
    GFX_THROW_INFO(GetDevice(gfx)->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &pPixelShader)); // Create the pixel shader from the compiled bytecode
}