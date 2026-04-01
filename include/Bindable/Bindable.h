#pragma once
#include "Graphics.h"
#include "Exceptions.h"
#include <vector>
#include "GraphicsThrowMacros.h"

class Bindable
{
public:
    virtual ~Bindable() = default;                 // Virtual destructor for proper cleanup of derived classes
    virtual void Bind(Graphics &gfx) noexcept = 0; // Pure virtual function to bind the resource to the graphics pipeline
protected:
    static ID3D11DeviceContext *GetContext(Graphics &gfx) noexcept; // Helper function to get the device context from the Graphics object
    static ID3D11Device *GetDevice(Graphics &gfx) noexcept;         // Helper function to get the device from the Graphic object
    static DxgiInfoManager &GetInfoManager(Graphics &gfx) noexcept; // Helper function to get the DxgiInfoManager from the Graphics object for error handling and debugging
};
