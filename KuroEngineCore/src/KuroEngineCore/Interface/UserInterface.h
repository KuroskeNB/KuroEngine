#pragma once

struct GLFWwindow;

namespace KuroEngine
{

	class UserInterface
	{
	public:

		static void ShowExampleAppDockSpace(bool* p_open);

		static void On_window_create(GLFWwindow* window);
		static void On_window_close();
		static void On_draw_begin();
		static void On_draw_end();

	private:

	};

}