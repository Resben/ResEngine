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
        int x, y;
        float gCost = 0; // Distance from start node
        float hCost = 0; // Distance from end node
        Node* parent = nullptr;
        bool isActive = false;
        Math::vec3 color = Math::vec3(1.0f);

        bool operator<(const Node& other) const
        {
                // Get the lower h cost if f cost is the same
            if(this->gCost + this->hCost == other.gCost + other.hCost)
                return this->hCost > other.hCost;
            else
                return (this->gCost + this->hCost) > (other.gCost + other.hCost); // Get the lower f cost
        }

        bool operator==(const Node& other) const
        {
            return this->x == other.x && this->y == other.y;
        }
    };

    class Grid
    {
    public:
        Grid(int gridSize, float tileSize, Math::vec3 position);
        ~Grid() = default;

        void GenerateGrid();
	    void ResizeGrid(int gridSize, float tileSize, Math::vec3 position);

        int GetGridSize();
        float GetTileSize();
        Math::vec3 GetPosition();

        bool IsActive(int row, int coloumn);
        void SetActive(int row, int coloumn);

        std::vector<Node> GetPath(Node start, Node end);
        void DebugRender(const PerspectiveCamera* camera);

        static SharedPtr<Grid> Create(int gridSize, float tileSize, Math::vec3 position);

    private:
	    std::vector<Node> GetNeighbours(Node& current);
	    int GetDistance(Node nodeA, Node nodeB);

        int m_gridSize;
        float m_tileSize;
        Math::vec3 m_position;
        std::vector<std::vector<Node>> m_grid;
        SharedPtr<VertexArray> m_debugGrid;
        SharedPtr<Shader> m_debugShader;

    };
}