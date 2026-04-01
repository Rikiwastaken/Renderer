#pragma once
#include "Bindable.h"

class PixelShader : public Bindable
{
public:
    PixelShader(Graphics &gfx, const std::wstring &path);
    void Bind(Graphics &gfx) noexcept override; // Override the Bind function to bind the pixel shader to the graphics pipeline
protected:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
};