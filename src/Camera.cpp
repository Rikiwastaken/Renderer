#include "Camera.h"
#include "imgui/imgui.h"

namespace dx = DirectX;

DirectX::XMMATRIX Camera::GetMatrix() const noexcept
{
    const auto pos = dx::XMVector3Transform(
        dx::XMVectorSet(0.0f, 0.0f, -r, 1.0f),
        dx::XMMatrixRotationRollPitchYaw(phi, theta, 0.0f));
    return dx::XMMatrixLookAtLH(pos, dx::XMVectorZero(), dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)) * dx::XMMatrixRotationRollPitchYaw(pitch, -yaw, roll);
}

void Camera::Reset() noexcept
{
    r = 20.0f;
    phi = 0.0f;
    theta = 0.0f;
    pitch = 0.0f;
    yaw = 0.0f;
    roll = 0.0f;
}

void Camera::SpawnControlWindow() noexcept
{
    if (ImGui::Begin("Camera Controls"))
    {
        ImGui::Text("Position");
        ImGui::SliderFloat("Radius", &r, 0.0f, 80.0f, "%.1f");
        ImGui::SliderFloat("Theta", &theta, -3.14159f, 3.14159f);
        ImGui::SliderFloat("Phi", &phi, -3.14159f / 2.0f, 3.14159f / 2.0f);
        ImGui::Text("Orientation");
        ImGui::SliderFloat("Pitch", &pitch, -3.14159f, 3.14159f);
        ImGui::SliderFloat("Yaw", &yaw, -3.14159f, 3.14159f);
        ImGui::SliderFloat("Roll", &roll, -3.14159f, 3.14159f);
        if (ImGui::Button("Reset"))
        {
            Reset();
        }
    }
    ImGui::End();
}