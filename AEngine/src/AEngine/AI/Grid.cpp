#include "Grid.h"
#include "AEngine/Render/RenderCommand.h"

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

	SharedPtr<Grid> Grid::Create(int gridSize, float tileSize)
	{
		return MakeShared<Grid>(gridSize, tileSize);
	}

	Grid::Grid(int gridSize, float tileSize)
	: m_gridSize(gridSize), m_tileSize(tileSize)
	{
		m_debugShader = Shader::Create(debug_shader);
		GenerateGrid();
	}

	void Grid::GenerateGrid()
	{
		if(m_debugGrid != nullptr)
			m_debugGrid->~VertexArray();

		m_debugGrid = VertexArray::Create();

		m_grid.clear();
		m_grid.resize(m_gridSize, std::vector<Node>(m_gridSize));

		std::vector<float> vertices;
		std::vector<unsigned int> indices;

		for(int x = 0; x < m_gridSize; x++)
		{
			for(int y = 0; y < m_gridSize; y++)
			{
		        float xPos = x * m_tileSize;
				float yPos = 0.0f;
				float zPos = y * m_tileSize;

				vertices.push_back(xPos);
				vertices.push_back(yPos);
				vertices.push_back(zPos);

				if (m_grid[x][y].isActive) 
				{
					vertices.push_back(0.0f);
					vertices.push_back(1.0f);
					vertices.push_back(0.0f);
				} 
				else 
				{
					vertices.push_back(1.0f);
					vertices.push_back(0.0f);
					vertices.push_back(0.0f);
				}

				if (x < m_gridSize - 1 && y < m_gridSize - 1) {
					int current = y + x * m_gridSize;
					int right = y + (x + 1) * m_gridSize;
					int down = (y + 1) + x * m_gridSize;
					int diag = (y + 1) + (x + 1) * m_gridSize;

					indices.push_back(current);
					indices.push_back(diag);
					indices.push_back(right);

					indices.push_back(current);
					indices.push_back(down);
					indices.push_back(diag);
				}

				m_grid[x][y] = Node({ xPos, zPos, 0, 0, 0, nullptr, false, Math::vec3(0.0f) });		
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

	void Grid::SetGridSize(int size)
	{
		m_gridSize = size;

		GenerateGrid();
	}

	void Grid::SetTileSize(float size)
	{
		m_tileSize = size;

		GenerateGrid();
	}

	int Grid::GetGridSize()
	{
		return m_gridSize;
	}

    float Grid::GetTileSize()
	{
		return m_tileSize;
	}

}