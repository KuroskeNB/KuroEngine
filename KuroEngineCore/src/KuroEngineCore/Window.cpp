#include "KuroEngineCore/Window.h"
#include "KuroEngineCore/Log.h"
#include "KuroEngineCore/Interface/UserInterface.h"
#include "KuroEngineCore/Render/OpenGL/Rendering_Opengl.h"
#include "KuroEngineCore/Inputs.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

namespace KuroEngine
{
    static bool s_GLFW_initialized = false;


	Window::Window(const unsigned int width, const unsigned int height, std::string title)
		:ThisWindowData({width,height,title})
	{
		int resultCode=init();

	}

	Window::~Window()
	{
		ShutDown();
	}

	int Window::init()
	{
        LOG_INFO("Creating window {0} with size {1} x {2} ", ThisWindowData.WindowTitle, ThisWindowData.WindowWidth, ThisWindowData.WindowHeight);

        glfwSetErrorCallback([](int error_code, const char* description)
            {
        LOG_CRITICAL("GLFW error: {0}", description);
    });

        /* Initialize the library */
        if (!glfwInit()) 
        {
            return -1;
        }
        
        /* Create a windowed mode window and its OpenGL context */
        window = glfwCreateWindow(ThisWindowData.WindowWidth, ThisWindowData.WindowHeight, ThisWindowData.WindowTitle.c_str(), NULL, NULL);
        if (!window)
        {
            LOG_CRITICAL("Cant create window");
            glfwTerminate();
            return -1;
        }

        /* Make the window's context current */
        if (!Rendering_OpengGl::init(window))
        {
            LOG_CRITICAL("Failed to initialize GLAD");
            return -1; 
        }

        glfwSetWindowUserPointer(window, &ThisWindowData);

        glfwSetWindowSizeCallback(window, [](GLFWwindow* pwindow, int width, int height)
            {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pwindow));
                data.WindowWidth = width;
                data.WindowHeight = height;

                EventWindowResize event(width,height);
                data.EventCallback(event);
            });

        glfwSetCursorPosCallback(window, [](GLFWwindow* pwindow, double xpos, double ypos)
            {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pwindow));
                EventMouseMoved event(xpos, ypos);
                data.EventCallback(event);
            }
        );
        
        glfwSetMouseButtonCallback(window, [](GLFWwindow* ptrWindow, int button, int action, int mods) {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(ptrWindow));
            double x_pos;
            double y_pos;
            glfwGetCursorPos(ptrWindow, &x_pos, &y_pos);
            switch (action)
            {
            case GLFW_PRESS:
            {
                EventMouseButtonPressed event(static_cast<MouseButtons>(button), x_pos, y_pos);
                data.EventCallback(event);
                break;
            }
            case GLFW_RELEASE:
            {
                EventMouseButtonReleased event(static_cast<MouseButtons>(button), x_pos,y_pos);
                data.EventCallback(event);
                break;
            }
            }
            });

        glfwSetWindowCloseCallback(window, [](GLFWwindow* pwindow)
            {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pwindow));
                EventWindowClose event;
                data.EventCallback(event);
            });

        glfwSetFramebufferSizeCallback(window, [](GLFWwindow* pwindow, int width, int height)
            {
                Rendering_OpengGl::Set_viewport(width, height);
            });

        glfwSetKeyCallback(window, [](GLFWwindow* ptrWindow, int key, int scancode, int action, int mods)
            {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(ptrWindow));
                switch (action)
                {
                case GLFW_PRESS:
                {
                    EventKeyPressed event(static_cast<InputKeys>(key), false);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    EventKeyReleased event(static_cast<InputKeys>(key), false);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    EventKeyPressed event(static_cast<InputKeys>(key), true);
                    data.EventCallback(event);
                    break;
                }
                }
            });
        
        UserInterface::On_window_create(window);

       // create_vertex(vertex_shader, fragment_shader);

        return 0;
      
	}

    void Window::create_vertex(const char* newvertex_shader, const char* newfragment_shader)
    {
       
    }

	void Window::ShutDown()
	{
        if (window)
        {
            UserInterface::On_window_close();
            glfwDestroyWindow(window);
            glfwTerminate();
        }
	}

	void Window::on_update()
	{
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
       
	}
    glm::vec2 Window::Get_cursor_position()
    {
        double x_pos;
        double y_pos;
        glfwGetCursorPos(window, &x_pos, &y_pos);
        return { x_pos,y_pos };
    }
}