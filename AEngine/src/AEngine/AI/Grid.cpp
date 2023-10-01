#include "Grid.h"
#include "AEngine/Render/RenderCommand.h"

#include <queue>
#include <cmath>
#include <set>
#include <unordered_set>

#include <functional>

namespace std {
	template<>
	struct hash<AEngine::Node> {
		size_t operator()(const AEngine::Node& node) const noexcept 
		{
			size_t h1 = std::hash<int>{}(node.x);
			size_t h2 = std::hash<int>{}(node.y);
			return h1 ^ (h2 << 1);
		}
	};
}

namespace AEngine
{

	static constexpr char* debug_shader = R"(
        #type vertex
		#version 330 core

		layout (location = 0) in vec3 aPos;
		layout (location = 1) in vec3 aColor;

		out vec3 color;

        uniform mat4 u_transform;

		void main()
		{
			gl_Position = u_transform * vec4(aPos, 1.0);
			color = aColor;
		}

        #type fragment
		#version 330 core

		in vec3 color;

        out vec4 FragColor;

		void main()
		{
			FragColor = vec4(color, 1.0f);
		}
	)";

	SharedPtr<Grid> Grid::Create(Math::ivec2 gridSize, float tileSize, Math::vec3 position)
	{
		return MakeShared<Grid>(gridSize, tileSize, position);
	}

	Grid::Grid(Math::ivec2 gridSize, float tileSize, Math::vec3 position)
	{
		m_debugShader = Shader::Create(debug_shader);
		ResizeGrid(gridSize, tileSize, position);
	}

	void Grid::ResizeGrid(Math::ivec2 gridSize, float tileSize, Math::vec3 position)
	{
		m_gridSize = gridSize;
		m_tileSize = tileSize;
		m_position = position;

		m_grid.clear();
		m_grid.resize(m_gridSize.x, std::vector<Node>(m_gridSize.y));

		GenerateGrid();
	}

	void Grid::GenerateGrid()
	{

		if(m_debugGrid != nullptr)
			m_debugGrid->~VertexArray();

		m_debugGrid = VertexArray::Create();

		std::vector<float> vertices;
		std::vector<unsigned int> indices;

		for(int x = 0; x < m_gridSize.x; x++)
		{
			for(int y = 0; y < m_gridSize.y; y++)
			{
				float xPos = x * (m_tileSize + m_tileOffset) + m_position.x;
				float yPos = 0.0f + m_position.y;
				float zPos = y * (m_tileSize + m_tileOffset) + m_position.z;

				Math::vec3 color;

				if (m_grid[x][y].isActive)
					color = Math::vec3(0.0f, 1.0f, 0.0f);
				else
					color = Math::vec3(1.0f, 0.0f, 0.0f);

				std::vector<float> boxVertices = {
					xPos, yPos, zPos,
					color.r, color.g, color.b,
					xPos + m_tileSize, yPos, zPos,
					color.r, color.g, color.b,
					xPos + m_tileSize, yPos, zPos + m_tileSize,
					color.r, color.g, color.b,
					xPos, yPos, zPos + m_tileSize,
					color.r, color.g, color.b,
				};

				vertices.insert(vertices.end(), boxVertices.begin(), boxVertices.end());

				int indexBase = (x * m_gridSize.y + y) * 4;
				std::vector<int> boxIndices = {
					indexBase, indexBase + 2, indexBase + 1,
					indexBase, indexBase + 3, indexBase + 2,
				};

				indices.insert(indices.end(), boxIndices.begin(), boxIndices.end());

				m_grid[x][y] = Node({ x, y, 0, 0, nullptr, false, Math::vec3(0.0f) });		
			}
		}

        SharedPtr<IndexBuffer> indexBuffer = IndexBuffer::Create();
        indexBuffer->SetData(indices.data(), static_cast<Uint32>(indices.size()), BufferUsage::StaticDraw);
		m_debugGrid->SetIndexBuffer(indexBuffer);

		SharedPtr<VertexBuffer> positionAndTextureBuffer = VertexBuffer::Create();
		positionAndTextureBuffer->SetData(vertices.data(), static_cast<Intptr_t>(vertices.size() * sizeof(float)), BufferUsage::StaticDraw);
		positionAndTextureBuffer->SetLayout({ { BufferElementType::Float3, false }, { BufferElementType::Float3, false } });
		m_debugGrid->AddVertexBuffer(positionAndTextureBuffer);
	}


	void Grid::DebugRender(const PerspectiveCamera* camera)
	{
		m_debugShader->Bind();
		m_debugGrid->Bind();

		RenderCommand::PolygonMode(PolygonFace::Front, PolygonDraw::Line);

		m_debugShader->SetUniformMat4("u_transform", camera->GetProjectionViewMatrix());
		RenderCommand::DrawIndexed(Primitive::Triangles, m_debugGrid->GetIndexBuffer()->GetCount(), 0);

		RenderCommand::PolygonMode(PolygonFace::Front, PolygonDraw::Fill);

		m_debugGrid->Unbind();
		m_debugShader->Unbind();
	}

	// Convert from world coordinates to grid coordinates
	std::vector<Math::vec3> Grid::GetPath(Math::vec3 start, Math::vec3 end)
	{
		Math::vec3 localStart = start - m_position;
		Math::vec3 localEnd = end - m_position;

		int startX = static_cast<int>(localStart.x / (m_tileSize + m_tileOffset));
		int startY = static_cast<int>(localStart.z / (m_tileSize + m_tileOffset));

		int endX = static_cast<int>(localEnd.x / (m_tileSize + m_tileOffset));
		int endY = static_cast<int>(localEnd.z / (m_tileSize + m_tileOffset));

		if(startX < 0 || startX >= m_gridSize.x || 
		startY < 0 || startY >= m_gridSize.y || 
		endX < 0 || endX >= m_gridSize.x || 
		endY < 0 || endY >= m_gridSize.y)
			return std::vector<Math::vec3>();

		return AStar(m_grid[startX][startY], m_grid[endX][endY]);
	}

	std::vector<Math::vec3> Grid::AStar(Node start, Node end)
	{
		std::vector<Node> path; // Final path
		std::vector<Math::vec3> waypoints; // Simplified path

		if(!start.isActive || !end.isActive) // If the start or end node is not walkable
			return waypoints;

		std::priority_queue<Node> openList; // Nodes to be evaluated (green in video)
		std::set<Node> closedList; // Nodes already evaluated (red in video)
		std::unordered_set<Node> openSetLookup; // Quick existence checks for the open list

		openList.push(start);

		while (!openList.empty())
		{
			Node current = openList.top(); // Get the node with the lowest fCost
			openList.pop(); // Remove the node from the openList with the lowest fCost
			openSetLookup.erase(current); // Remove the node from the openSetLookup
			closedList.insert(current); // Insert into the closed list

			if (current == end) // If we reached the target node
			{
				Node* backtrackNode = &current;
				while (backtrackNode != nullptr) 
				{
					path.push_back(*backtrackNode);
					backtrackNode = backtrackNode->parent;
				}
				waypoints = SimplifyPath(path);
				std::reverse(waypoints.begin(), waypoints.end());
			}

			for(auto& neighbour : GetNeighbours(current))
			{
				if(!neighbour.isActive || closedList.find(neighbour) != closedList.end()) // Skip this neighbour if its not active or already evaluated
					continue;

				int costToNeighbour = current.gCost + GetDistance(current, neighbour); // Calculate the cost to the neighbour

				if(costToNeighbour < neighbour.gCost || openSetLookup.find(neighbour) == openSetLookup.end())
				{
					neighbour.parent = &current;
					neighbour.gCost = costToNeighbour;
					neighbour.hCost = GetDistance(neighbour, end);

					if(openSetLookup.find(neighbour) == openSetLookup.end())
					{
						openList.push(neighbour);
						openSetLookup.insert(neighbour);
					}
				}
			}
		}

		return waypoints;
	}

	std::vector<Math::vec3> Grid::SimplifyPath(std::vector<Node>& path)
	{
		std::vector<Math::vec3> waypoints;

		Math::vec2 directionOld = Math::vec2(0.0f);

		for(int i = 1; i < path.size(); i++)
		{
			Math::vec2 directionNew = Math::vec2(path[i - 1].x - path[i].x, path[i - 1].y - path[i].y);
			if(directionNew != directionOld)
			{
				waypoints.push_back(Math::vec3(path[i].x * (m_tileSize + m_tileOffset) + m_position.x, 0.0f + m_position.y, path[i].y * (m_tileSize + m_tileOffset) + m_position.z));
			}
			directionOld = directionNew;
		}

		return waypoints;
	}

	std::vector<Node> Grid::GetNeighbours(Node& current)
	{
		std::vector<Node> neighbours;

		for(int x = -1; x <= 1; x++)
		{
			for(int y = -1; y <= 1; y++)
			{
				if(x == 0 && y == 0) // Skip the current node
					continue;

				int checkX = current.x + x;
				int checkY = current.y + y;

				if(checkX >= 0 && checkX < m_gridSize.x && checkY >= 0 && checkY < m_gridSize.y) // Check if the neighbour are within the grid
					neighbours.push_back(m_grid[checkX][checkY]);
			}
		}

		return neighbours;
	}

	int Grid::GetDistance(Node nodeA, Node nodeB)
	{
		int dstx = std::abs(nodeA.x - nodeB.x);
		int dsty = std::abs(nodeA.y - nodeB.y);

		if(dstx > dsty)
			return 14 * dsty + 10 * (dstx - dsty);
		else
			return 14 * dstx + 10 * (dsty - dstx);
	}

	bool Grid::IsActive(int row, int coloumn)
	{
		return m_grid[row][coloumn].isActive;
	}

    void Grid::SetActive(int row, int coloumn)
	{
		m_grid[row][coloumn].isActive = !m_grid[row][coloumn].isActive;
	}
	
	Math::ivec2 Grid::GetGridSize()
	{
		return m_gridSize;
	}

    float Grid::GetTileSize()
	{
		return m_tileSize;
	}

	Math::vec3 Grid::GetPosition()
	{
		return m_position;
	}
}