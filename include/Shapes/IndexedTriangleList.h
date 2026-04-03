#pragma once
#include <vector>
#include <DirectXMath.h>

template <class T>
class IndexedTriangleList
{
public:
    IndexedTriangleList() = default;
    IndexedTriangleList(std::vector<T> vertices_in, std::vector<unsigned short> indices_in)
        : vertices(std::move(vertices_in)),
          indices(std::move(indices_in))
    {
        assert(indices.size() % 3 == 0); // Ensure that the number of indices is a multiple of 3, as each triangle is defined by 3 vertices
        assert(vertices.size() > 2);     // Ensure that there is at least three vertices in the list
    }
    void Transform(DirectX::FXMMATRIX matrix_in)
    {
        for (auto &v : vertices)
        {
            const DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&v.pos);                 // Load the vertex position into an XMVECTOR for transformation
            DirectX::XMStoreFloat3(&v.pos, DirectX::XMVector3Transform(pos, matrix_in)); // Transform the vertex position by the provided matrix and store the result back in the vertex
        }
    }

public:
    std::vector<T> vertices;             // Vector to hold the vertex data for the triangle list
    std::vector<unsigned short> indices; // Vector to hold the index data for the triangle list (each group of 3 indices defines a triangle)
};