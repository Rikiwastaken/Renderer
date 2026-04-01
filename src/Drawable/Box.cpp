#include "Drawable/Box.h"
#include "Bindable/BindableBase.h"

Box::Box(Graphics &gfx, std::mt19937 &rng,
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
    struct Vertex
    {
        struct
        {
            float x;
            float y;
            float z;
        } pos;
    };
    const std::vector<Vertex> vertices =
        {
            {-1.0f, -1.0f, -1.0f},
            {1.0f, -1.0f, -1.0f},
            {-1.0f, 1.0f, -1.0f},
            {1.0f, 1.0f, -1.0f},
            {-1.0f, -1.0f, 1.0f},
            {1.0f, -1.0f, 1.0f},
            {-1.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, 1.0f}};
    AddBind(std::make_unique<VertexBuffer>(gfx, vertices));
    auto pVertexShader = std::make_unique<VertexShader>(gfx, L"VertexShader.cso");
    auto pVertexShaderBlob = pVertexShader->GetBytecode(); // Get the compiled vertex shader bytecode for input layout creation
    AddBind(std::move(pVertexShader));
    AddBind(std::make_unique<PixelShader>(gfx, L"PixelShader.cso"));

    const std::vector<unsigned short> indices =
        {
            0, 2, 1, 2, 3, 1, // back face
            1, 3, 5, 3, 7, 5, // right face
            2, 6, 3, 3, 6, 7, // front face
            4, 5, 7, 4, 7, 6, // left face
            0, 4, 2, 2, 4, 6, // bottom face
            0, 1, 4, 1, 5, 4  // top face
        };
    AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));

    struct ConstantBuffer2
    {
        struct
        {
            float r;
            float g;
            float b;
            float a = 1.0f;
        } face_colors[6];
    };
    const ConstantBuffer2 cb2 = {
        {
            {1.0f, 0.0f, 0.0f}, // Red
            {0.0f, 1.0f, 0.0f}, // Green
            {0.0f, 0.0f, 1.0f}, // Blue
            {1.0f, 1.0f, 0.0f}, // Yellow
            {1.0f, 0.0f, 1.0f}, // Magenta
            {0.0f, 1.0f, 1.0f}  // Cyan
        }};
    AddBind(std::make_unique<PixelConstantBuffer<ConstantBuffer2>>(gfx, cb2));

    const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
        {
            {"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}};
    AddBind(std::make_unique<InputLayout>(gfx, ied, pVertexShaderBlob));

    AddBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

    AddBind(std::make_unique<TransformCbuf>(gfx, *this));
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
