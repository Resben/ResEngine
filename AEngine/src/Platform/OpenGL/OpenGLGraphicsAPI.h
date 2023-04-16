#include "AEngine/Render/GraphicsAPI.h"

namespace AEngine
{
	class OpenGLGraphicsAPI : public GraphicsAPI
	{
	public:
		OpenGLGraphicsAPI();
		void SetClearColor(Math::vec4 color) override;
		void Clear() override;
		virtual void DrawIndexed(unsigned int size) override;
		virtual void PolygonMode(enum AE_TYPES type) override;
		virtual void EnableDepth(bool set) override;
		virtual void SetViewport(int x, int y, int width, int height) override;
	private:

	};
}