#include "Drawable/Pyramid.h"
#include "Bindable/BindableBase.h"
#include "Shapes/Cone.h"
#include "GraphicsThrowMacros.h"

Pyramid::Pyramid(Graphics &gfx, std::mt19937 &rng,
                 std::uniform_real_distribution<float> &adist,
                 std::uniform_real_distribution<float> &ddist,
                 std::uniform_real_distribution<float> &odist,
                 std::uniform_real_distribution<float> &rdist)
    : r(rdist(rng)),
      droll(ddist(rng)),
      dpitch(ddist(rng)),
      dyaw(ddist(rng)),
      dtheta(ddist(rng)),
      dphi(ddist(rng)),
      dchi(ddist(rng)),
      theta(adist(rng)),
      phi(adist(rng)),
      chi(adist(rng))
{
    namespace dx = DirectX;
    if (!isStaticInitialized())
    {
        struct Vertex
        {
            dx::XMFLOAT3 pos;
            struct
            {
                unsigned char r;
                unsigned char g;
                unsigned char b;
                unsigned char a;
            } color;
        };
        auto model = Cone::MakeTesselated<Vertex>(4);

        model.vertices[0].color = {255, 255, 0};
        model.vertices[1].color = {255, 255, 0};
        model.vertices[2].color = {255, 255, 0};
        model.vertices[3].color = {255, 255, 0};
        model.vertices[4].color = {255, 255, 80};
        model.vertices[5].color = {255, 10, 0};

        model.Transform(dx::XMMatrixScaling(1.0f, 1.0f, 0.7f));

        auto pvs = std::make_unique<VertexShader>(gfx, L"ColorBlendVertexShader.cso");
        auto pvsbc = pvs->GetBytecode();
        AddStaticBind(std::move(pvs));

        AddStaticBind(std::make_unique<PixelShader>(gfx, L"ColorBlendPixelShader.cso"));

        AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

        const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
            {
                {"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
                {"Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
            };
        AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

        AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
    }
    else
    {
        SetIndexFromStatic();
    }

    AddBind(std::make_unique<TransformCbuf>(gfx, *this));
}

void Pyramid::Update(float dt) noexcept
{
    roll += droll * dt;
    pitch += dpitch * dt;
    yaw += dyaw * dt;
    theta += dtheta * dt;
    phi += dphi * dt;
    chi += dchi * dt;
}

DirectX::XMMATRIX Pyramid::GetTransformXM() const noexcept
{
    namespace dx = DirectX;
    return dx::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
           dx::XMMatrixTranslation(r, 0.0f, 0.0f) *
           dx::XMMatrixRotationRollPitchYaw(theta, phi, chi) *
           dx::XMMatrixTranslation(0.0f, 0.0f, 20.0f);
}
