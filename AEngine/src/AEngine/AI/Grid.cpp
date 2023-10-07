#include "Grid.h"
#include "AEngine/Render/RenderCommand.h"
#include "AEngine/Core/Logger.h"

#include <queue>
#include <cmath>
#include <set>
#include <unordered_set>
#include <fstream>
#include <sstream>
#include <utility>
#include <functional>

namespace AEngine
{

	struct pair_hash 
	{
		template <class T1, class T2>
		std::size_t operator() (const std::pair<T1, T2>& p) const {
			auto h1 = std::hash<T1>{}(p.first);
			auto h2 = std::hash<T2>{}(p.second);

			return h1 ^ h2;
		}
	};

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

	SharedPtr<Grid> Grid::Create(const std::string& ident, const std::string& path)
	{
		return MakeShared<Grid>(ident, path);
	}

	Grid::Grid(Math::ivec2 gridSize, float tileSize, Math::vec3 position)
	: Asset("runtime", {})
	{
		m_debugShader = Shader::Create(debug_shader);
		ResizeGrid(gridSize, tileSize, position);
	}

	Grid::Grid(const std::string& ident, const std::string& path)
	: Asset(ident, path)
	{
		m_debugShader = Shader::Create(debug_shader);
		LoadFromFile(path);
	}

    void Grid::LoadFromFile(const std::string& path)
	{
		std::ifstream file(path);

		if (!file.is_open())
		{
			AE_LOG_ERROR("Grid::LoadFromFile -> Could not read file: {}", path);
			return;
		}

		std::string line;
		std::getline(file, line);

		std::getline(file, line);
		std::stringstream ss(line);
		ss >> m_gridSize.x; ss.ignore();
		ss >> m_gridSize.y; ss.ignore(); 
		ss >> m_tileSize;   ss.ignore(); 
		ss >> m_position.x; ss.ignore(); 
		ss >> m_position.y; ss.ignore();
		ss >> m_position.z;

		m_grid.resize(m_gridSize.x, std::vector<Node>(m_gridSize.y));

		for (int x = 0; x < m_gridSize.x; x++)
		{
			std::getline(file, line);
			std::stringstream ssLine(line);
			for (int y = 0; y < m_gridSize.y; y++)
			{
				bool active;
				ssLine >> active; 
				ssLine.ignore();
				m_grid[x][y] = Node({ x, y, 0, 0, nullptr, active });
			}
		}

		file.close();

		GenerateGrid();
	}

	void Grid::SaveToFile(const std::string& path)
	{
		std::ofstream file(path);

		if(!file.is_open())
		{
			AE_LOG_ERROR("Grid::SaveToFile -> Could not save to file: {}", path);
			return;
		}
		
		file << "Grid" << std::endl;
		file << m_gridSize.x << "," << m_gridSize.y << "," << m_tileSize << "," << m_position.x << "," << m_position.y << "," << m_position.z << std::endl;

		for(int x = 0; x < m_gridSize.x; x++)
		{
			for(int y = 0; y < m_gridSize.y; y++)
			{
				file << m_grid[x][y].isActive << ",";
			}
			file << std::endl;
		}

		file.close();
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
				float halfTileSize = m_tileSize / 2.0f;
				float xPos = x * (m_tileSize + m_tileOffset) + m_position.x - halfTileSize;
				float yPos = 0.0f + m_position.y;
				float zPos = y * (m_tileSize + m_tileOffset) + m_position.z - halfTileSize;

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

				m_grid[x][y] = Node({ x, y, 0, 0, nullptr, m_grid[x][y].isActive });		
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
	std::vector<float> Grid::GetPath(Math::vec3 start, Math::vec3 end)
	{
		float halfTileSize = m_tileSize / 2.0f;
		Math::vec3 localStart = start - m_position + Math::vec3(halfTileSize, 0.0f, halfTileSize);;
		Math::vec3 localEnd = end - m_position + Math::vec3(halfTileSize, 0.0f, halfTileSize);;

		int startX = static_cast<int>(localStart.x / (m_tileSize + m_tileOffset));
		int startY = static_cast<int>(localStart.z / (m_tileSize + m_tileOffset));

		int endX = static_cast<int>(localEnd.x / (m_tileSize + m_tileOffset));
		int endY = static_cast<int>(localEnd.z / (m_tileSize + m_tileOffset));

		if(startX < 0 || startX >= m_gridSize.x || 
		startY < 0 || startY >= m_gridSize.y || 
		endX < 0 || endX >= m_gridSize.x || 
		endY < 0 || endY >= m_gridSize.y)
			return std::vector<float>();

		return AStar(m_grid[startX][startY], m_grid[endX][endY]);
	}

	std::vector<float> Grid::AStar(Node start, Node end)
	{
		std::vector<Node> path; // Final path
		std::vector<float> waypoints; // Simplified path

		if(!start.isActive || !end.isActive) // If the start or end node is not walkable
			return waypoints;

		for(int x = 0; x < m_gridSize.x; x++)
		{
			for(int y = 0; y < m_gridSize.y; y++)
			{
				m_grid[x][y].parent = nullptr;
			}
		}

		auto cmp = [this](const std::pair<int, int>& a, const std::pair<int, int>& b) {
			const Node& nodeA = m_grid[a.first][a.second];
			const Node& nodeB = m_grid[b.first][b.second];
			return (nodeA.gCost + nodeA.hCost) > (nodeB.gCost + nodeB.hCost); // lowest fCost has the highest priority
		};

		std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, decltype(cmp)> openList(cmp); // Nodes to be evaluated (green in video)
		std::set<std::pair<int, int>> closedList; // Nodes already evaluated (red in video)
		std::unordered_set<std::pair<int, int>, pair_hash> openSetLookup; // Quick existence checks for the open list

		openList.push({start.x, start.y});

		while (!openList.empty())
		{
			auto coord = openList.top(); // Get the node with the lowest fCost
			Node& current = m_grid[coord.first][coord.second];
			openList.pop(); // Remove the node from the openList with the lowest fCost
			openSetLookup.erase({current.x, current.y}); // Remove the node from the openSetLookup
			closedList.insert({current.x, current.y}); // Insert into the closed list

			if (current == end) // If we reached the target node
			{
				Node* backtrackNode = &current;
				while (backtrackNode != nullptr) 
				{
					path.push_back(*backtrackNode);
					backtrackNode = backtrackNode->parent;
				}
				std::vector<Math::vec3> Vec3waypoints = SimplifyPath(path);
				std::reverse(Vec3waypoints.begin(), Vec3waypoints.end());

				for (Math::vec3 vec : Vec3waypoints)
					AE_LOG_DEBUG("vec: {},{},{}", vec.x, vec.y, vec.z);

					// Lua doesnt support vec3 at the moment

				for(Math::vec3 positions : Vec3waypoints)
				{
					waypoints.push_back(positions.x);
					waypoints.push_back(positions.y);
					waypoints.push_back(positions.z);
				}

				return waypoints;
			}

			for(auto& neighbour : GetNeighbours(current))
			{
				if (!neighbour.isActive || closedList.find({neighbour.x, neighbour.y}) != closedList.end())
					continue;

				int costToNeighbour = current.gCost + GetDistance(current, neighbour); // Calculate the cost to the neighbour

				if(costToNeighbour < neighbour.gCost || openSetLookup.find({neighbour.x, neighbour.y}) == openSetLookup.end())
				{
					int nX = neighbour.x;
					int nY = neighbour.y;

					m_grid[nX][nY].parent = &m_grid[current.x][current.y];
					m_grid[nX][nY].gCost = costToNeighbour;
					m_grid[nX][nY].hCost = GetDistance(neighbour, end);

					if(openSetLookup.find({nX, nY}) == openSetLookup.end())
					{
						openList.push({nX, nY});
						openSetLookup.insert({nX, nY});
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
				Math::vec3 pos;
				pos.x = path[i].x * (m_tileSize + m_tileOffset) + m_position.x;
				pos.y = 0.0f + m_position.y;
				pos.z = path[i].y * (m_tileSize + m_tileOffset) + m_position.z;
				waypoints.push_back(pos);
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