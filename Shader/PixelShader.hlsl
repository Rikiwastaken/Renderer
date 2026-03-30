cbuffer CBuf
{
    float3 face_colors[6];
};


float4 main(uint TriangleID : SV_PrimitiveID) : SV_Target
{
    return float4(face_colors[TriangleID/2], 1.0f); // Output the color for testing
}