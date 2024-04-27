#pragma once


struct GLFWwindow;
namespace KuroEngine
{
	class VertexArray;

	class Rendering_OpengGl
	{
	public:
		static bool init(GLFWwindow* window);

		static void Draw(VertexArray& vertex_array);
		static void Clear();
		static void Set_clear_color(const float r, const float g, const float b, const float a);
		static void Set_viewport(int width, int height);
		static void Enable_depth();
		static void Disable_depth();

	private:

	};

}