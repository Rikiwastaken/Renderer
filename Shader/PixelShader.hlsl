cbuffer CBuf
{
    float4 face_colors[6];
};


float4 main(uint TriangleID : SV_PrimitiveID) : SV_Target
{
    return face_colors[TriangleID/2]; // Output the color for testing
}