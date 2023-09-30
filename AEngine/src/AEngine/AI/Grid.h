#pragma once

#include "AEngine/Render/VertexArray.h"
#include "AEngine/Math/Math.h"
#include "AEngine/Render/Shader.h"
#include "AEngine/Core/PerspectiveCamera.h"
#include <vector>

namespace AEngine
{
    struct Node
    {
        float x, y;
        float f, g, h;
        Node* parent;
        bool isActive;
        Math::vec3 color;
    };

    class Grid
    {
    public:
        Grid(int gridSize, float tileSize);
        ~Grid() = default;
        void SetGridSize(int size);
        void SetTileSize(float size);

        int GetGridSize();
        float GetTileSize();

        std::vector<Node> GetPath(const Node start, const Node end);
        void DebugRender(const PerspectiveCamera* camera);

        static SharedPtr<Grid> Create(int gridSize, float tileSize);

    private:
        void GenerateGrid();

        int m_gridSize;
        float m_tileSize;
        std::vector<std::vector<Node>> m_grid;
        SharedPtr<VertexArray> m_debugGrid;
        SharedPtr<Shader> m_debugShader;

    };
}