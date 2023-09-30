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

        void GenerateGrid();
	    void ResizeGrid(int gridSize, float tileSize);

        int GetGridSize();
        float GetTileSize();

        bool IsActive(int row, int coloumn);
        void SetActive(int row, int coloumn);

        std::vector<Node> GetPath(const Node start, const Node end);
        void DebugRender(const PerspectiveCamera* camera);

        static SharedPtr<Grid> Create(int gridSize, float tileSize);

    private:

        int m_gridSize;
        float m_tileSize;
        std::vector<std::vector<Node>> m_grid;
        SharedPtr<VertexArray> m_debugGrid;
        SharedPtr<Shader> m_debugShader;

    };
}