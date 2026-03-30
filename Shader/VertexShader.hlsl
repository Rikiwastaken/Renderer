struct VSOutput
{
    float4 position : SV_POSITION; // Output position to the rasterizer stage
    
};

cbuffer CBuf
{
    matrix transform; // Transformation matrix for vertex positions (e.g., world-view-projection matrix)
};

VSOutput main(float3 pos : POSITION)
{
    VSOutput output;
    output.position = mul(float4(pos.x, pos.y, pos.z, 1.0f), transform); // Transform the vertex position using the provided transformation matrix
    return output;
}