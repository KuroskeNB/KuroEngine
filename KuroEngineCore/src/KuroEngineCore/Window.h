#pragma once
#include <iostream>
#include <string>
#include "KuroEngineCore/Events.h"
#include <functional>
#include <glm/ext/vector_float2.hpp>

struct GLFWwindow;

namespace KuroEngine
{

	class Window
	{
    public:
		using EventCallbackFn = std::function<void(BaseEvent&)>;

		Window(const unsigned int width, const unsigned int height, std::string title);

		virtual ~Window();

		virtual void on_update();
		unsigned int GetWindowWidth() const { return ThisWindowData.WindowWidth; };
		unsigned int GetWindowHeight() const { return ThisWindowData.WindowHeight; };
		std::string GetWindowTitle()const { return ThisWindowData.WindowTitle;}
		glm::vec2 Get_cursor_position();

		Window(const Window&) = delete;
		Window(Window&&) = delete;
		Window& operator=(const Window&) = delete;
		Window& operator=(Window&&) = delete;
		
		void SetEventCallback(const EventCallbackFn& newCallback) { ThisWindowData.EventCallback = newCallback; };
		GLFWwindow* Get_window_ptr() const { return window; };
	private:
		int init();
		void ShutDown();

		void create_vertex(const char* vertex_shader, const char* fragment_shader);
		struct WindowData
		{
			unsigned int WindowWidth;
			unsigned int WindowHeight;
			std::string WindowTitle;

			EventCallbackFn EventCallback;
		};
		WindowData ThisWindowData;

		GLFWwindow* window;


	};
}