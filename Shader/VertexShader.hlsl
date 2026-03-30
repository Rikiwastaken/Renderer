struct VSOutput
{
    float3 color : COLOR;          // Output color to the pixel shader stage
    float4 position : SV_POSITION; // Output position to the rasterizer stage
    
};

cbuffer CBuf
{
    matrix transform; // Transformation matrix for vertex positions (e.g., world-view-projection matrix)
};

VSOutput main(float2 pos : POSITION, float3 color : COLOR)
{
    VSOutput output;
    output.position = mul(float4(pos.x, pos.y, 0.0f, 1.0f), transform); // Transform the vertex position using the provided transformation matrix
    output.color = color;
    return output;
}