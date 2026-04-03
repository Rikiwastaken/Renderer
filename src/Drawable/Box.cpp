#include "Drawable/Box.h"
#include "Bindable/BindableBase.h"
#include "Shapes/Cube.h"

Box::Box(Graphics &gfx, std::mt19937 &rng,
         std::uniform_real_distribution<float> &adist,
         std::uniform_real_distribution<float> &ddist,
         std::uniform_real_distribution<float> &odist,
         std::uniform_real_distribution<float> &rdist,
         std::uniform_real_distribution<float> &bist)
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
        };
        auto model = Cube::Make<Vertex>();

        AddStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));
        auto pVertexShader = std::make_unique<VertexShader>(gfx, L"ColorIndexVertexShader.cso");
        auto pVertexShaderBlob = pVertexShader->GetBytecode(); // Get the compiled vertex shader bytecode for input layout creation
        AddStaticBind(std::move(pVertexShader));
        AddStaticBind(std::make_unique<PixelShader>(gfx, L"ColorIndexPixelShader.cso"));

        AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

        struct ConstantBuffer2
        {
            struct
            {
                float r;
                float g;
                float b;
                float a = 1.0f;
            } face_colors[8];
        };
        const ConstantBuffer2 cb2 = {
            {
                {1.0f, 1.0f, 1.0f}, // White
                {1.0f, 0.0f, 0.0f}, // Red
                {0.0f, 1.0f, 0.0f}, // Green
                {0.0f, 0.0f, 1.0f}, // Blue
                {1.0f, 1.0f, 0.0f}, // Yellow
                {1.0f, 0.0f, 1.0f}, // Magenta
                {0.0f, 1.0f, 1.0f}, // Cyan
                {0.5f, 0.5f, 0.5f}  // Gray
            }};
        AddStaticBind(std::make_unique<PixelConstantBuffer<ConstantBuffer2>>(gfx, cb2));

        const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
            {
                {"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}};
        AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pVertexShaderBlob));

        AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
    }
    else
    {
        SetIndexFromStatic(); // If the static resources have already been initialized, set the index buffer pointer from the static index buffer
    }
    AddBind(std::make_unique<TransformCbuf>(gfx, *this));

    // model deformation transform
    dx::XMStoreFloat3x3(&modelTransform, dx::XMMatrixScaling(bist(rng), bist(rng), bist(rng)));
};

void Box::Update(float dt) noexcept
{
    theta += dtheta * dt;
    phi += dphi * dt;
    chi += dchi * dt;
    roll += droll * dt;
    pitch += dpitch * dt;
    yaw += dyaw * dt;
}

DirectX::XMMATRIX Box::GetTransformXM() const noexcept
{
    return DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
           DirectX::XMMatrixTranslation(r, 0.0f, 0.0f) *
           DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi) *
           DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f);
}
