#pragma once
#include <iostream>
#include "KuroEngineCore/Events.h"
#include "KuroEngineCore/Camera.h"

namespace KuroEngine
{

	class Application
	{
	private:
		class Window* AppWindow;

		EventDispatcher Event_dispatcher;
		bool bCloseWindow = false;
	public:
		Application();

		virtual ~Application();

		virtual int start(unsigned int window_width, unsigned int window_height, const char* title);
		virtual void On_update() {};
		virtual void On_draw_interface() {};
		virtual void On_button_updated(const MouseButtons button_code,const double x_pos,const double y_pos,const bool pressed) {};
		
		glm::vec2 Get_cursor_position();

		Application(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(const Application&) = delete;
		Application& operator=(Application&&) = delete;

		float camera_position[3] = { 0.f, 0.f, 1.f };
		float camera_rotation[3] = { 0.f, 0.f, 0.f };
		bool perspective_camera = false;
		Camera camera{ glm::vec3(-5.f, 0.f, 0.f) };
	};
}