#include "Rendering_Opengl.h"
#include <glad/glad.h>
#include "VertexArray.h"
#include <GLFW/glfw3.h>
#include "KuroEngineCore/Log.h"

namespace KuroEngine
{
	bool Rendering_OpengGl::init(GLFWwindow* window)
	{
		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
		{
			LOG_CRITICAL("Failed to initialize GLAD");
			return false;
		}

		return true;
	}
	void Rendering_OpengGl::Draw(VertexArray& vertex_array)
	{
		vertex_array.bind();
		glDrawElements(GL_TRIANGLES, vertex_array.Get_indices_count(), GL_UNSIGNED_INT, nullptr);
	}
	void Rendering_OpengGl::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Rendering_OpengGl::Set_clear_color(const float r, const float g, const float b, const float a)
	{
		glClearColor(r, g, b, a);
	}

	void Rendering_OpengGl::Set_viewport(int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void Rendering_OpengGl::Enable_depth()
	{
		glEnable(GL_DEPTH_TEST);
	}

	void Rendering_OpengGl::Disable_depth()
	{
		glDisable(GL_DEPTH_TEST);
	}

}