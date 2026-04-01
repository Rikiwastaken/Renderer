#include "Bindable/Bindable.h"
#include "Graphics.h"

ID3D11DeviceContext *Bindable::GetContext(Graphics &gfx) noexcept
{
    return gfx.GetContext(); // Return the device context from the Graphics object
}

ID3D11Device *Bindable::GetDevice(Graphics &gfx) noexcept
{
    return gfx.GetDevice(); // Return the device from the Graphics object
}

DxgiInfoManager &Bindable::GetInfoManager(Graphics &gfx) noexcept
{
    return gfx.infoManager;
}
