#include "KuroEngineCore/Application.h"
#include "KuroEngineCore/Log.h"
#include "KuroEngineCore/Window.h"

#include "KuroEngineCore/Render/OpenGL/ShaderProgram.h"
#include "KuroEngineCore/Render/OpenGL/VertexArray.h"
#include "KuroEngineCore/Render/OpenGL/VertexBuffer.h"
#include "KuroEngineCore/Render/OpenGL/IndexBuffer.h"
#include "KuroEngineCore/Render/OpenGL/Rendering_Opengl.h"
#include "KuroEngineCore/Render/OpenGL/Texture2D.h"
#include "KuroEngineCore/Interface/UserInterface.h"
#include "KuroEngineCore/Inputs.h"

#include <GLFW/glfw3.h>
#include <imgui/imgui.h>

#include <glm/mat3x3.hpp>
#include <glm/trigonometric.hpp>


namespace KuroEngine
{
    GLfloat ones[] = {
        //front
       -1.0f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,  1.f, 1.f,
       -1.0f,  0.5f, -0.5f,   0.0f, 1.0f, 1.0f,    0.f, 1.f,
        -1.0f, -0.5f,  0.5f,   1.0f, 0.0f, 1.0f,    1.f, 0.f,
       -1.0f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,    0.f, 0.f,
        //back
        0.0f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,  1.f, 1.f,
        0.0f,  0.5f, -0.5f,   0.0f, 1.0f, 1.0f,    0.f, 1.f,
        0.0f, -0.5f,  0.5f,   1.0f, 0.0f, 1.0f,    1.f, 0.f,
        0.0f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,    0.f, 0.f
    };
    GLuint indices[] = {
       0, 1, 2, 3, 2, 1, // front
        4, 5, 6, 7, 6, 5, // back
        0, 4, 6, 0, 2, 6, // right
        1, 5, 3, 3, 7, 5, // left
        3, 7, 2, 7, 6, 2, // top
        1, 5, 0, 5, 0, 4  // bottom
    };


    const char* path_photo = "D:/cfbd15d1ac7b9f5ca9936b82dc043080.jpg";
    const char* vertex_shader =
        "#version 460\n"
        "layout(location = 0) in vec3 vertex_position;"
        "layout(location = 1) in vec3 vertex_color;"
        "layout(location = 2) in vec2 texture_coordinates;" // Добавляем текстурные координаты
        "uniform mat4 model_matrix;"
        "uniform mat4 camera_matrix;"
        "out vec3 color;"
        "out vec2 tex_coords;" // Передаем текстурные координаты во фрагментный шейдер
        "void main() {"
        "   color = vertex_color;"
        "   tex_coords = texture_coordinates;" // Передаем текстурные координаты во фрагментный шейдер
        "   gl_Position = camera_matrix * model_matrix * vec4(vertex_position, 1.0);"
        "}";

    const char* fragment_shader =
        "#version 460\n"
        "in vec3 color;"
        "in vec2 tex_coords;" // Получаем текстурные координаты из вершинного шейдера
        "uniform sampler2D texture_sampler;" // Объявляем сэмплер для текстуры
        "out vec4 frag_color;"
        "void main() {"
        " frag_color = texture(texture_sampler, tex_coords) * vec4(vec3(1,1,1), 1.0);"
        "}";
    std::unique_ptr<ShaderProgram> shader_program;
    std::unique_ptr<VertexBuffer> one_buffer;
    std::unique_ptr<IndexBuffer> index_buffer;
    std::unique_ptr<VertexArray> main_vao;

    float scale[3] = { 1.f, 1.f, 1.f };
    float translate[3] = { 0.f, 0.f, 0.f };
    float rotate = 0.f;
    float BackgroundRGB[4] = { 0.2,0.2,0.2,0 };
    std::array<glm::vec3, 5> positions = {
            glm::vec3(-2.f, 1.f, -1.5f),
            glm::vec3(1.f,  0.f,  2.f),
            glm::vec3(2.f,  1.f, -2.f),
            glm::vec3(3.f, 2.f,  -1.f),
            glm::vec3(1.f, -2.f,  1.f)
    };


    Application::Application()
    {
        LOG_INFO("bye bye");
    }
    Application::~Application()
    {

    }

    int Application::start(unsigned int window_width, unsigned int window_height, const char* title)
    {
        AppWindow = new Window(window_width, window_height, "New Application");
        Event_dispatcher.add_event_listener<EventMouseMoved>([](EventMouseMoved& event) {
            //LOG_INFO("Mouse moved to {0},{1}", event.x, event.y);
            });

        Event_dispatcher.add_event_listener<EventWindowResize>([](EventWindowResize& event) {
            LOG_INFO("Window size changed to {0}x{1}", event.width, event.height);
            });
        
        Event_dispatcher.add_event_listener<EventWindowClose>([&](EventWindowClose& event)
            {
                bCloseWindow = true;
            });

        Event_dispatcher.add_event_listener<EventMouseButtonPressed>([&](EventMouseButtonPressed& event) {
            Inputs::PressButton(event.ButtonCode);
            On_button_updated(event.ButtonCode, event.x, event.y, true);
            });
        Event_dispatcher.add_event_listener<EventMouseButtonReleased>([&](EventMouseButtonReleased& event) {
            Inputs::ReleaseButton(event.ButtonCode);
            On_button_updated(event.ButtonCode, event.x, event.y, false);
            });

        Event_dispatcher.add_event_listener<EventKeyPressed>([&](EventKeyPressed&event)
            {
                LOG_INFO("key pressed {0}",(char)event.KeyCode);
                Inputs::PressKey(event.KeyCode);
            });

        Event_dispatcher.add_event_listener<EventKeyReleased>([&](EventKeyReleased& event)
            {
                LOG_INFO("key released {0}", (char)event.KeyCode);
                Inputs::ReleaseKey(event.KeyCode);
            });

        AppWindow->SetEventCallback([&](BaseEvent& event)
            {
                Event_dispatcher.dispatch(event);
            });

        std::unique_ptr<Texture2D> maki_texture = std::make_unique<Texture2D>(path_photo);
        
      
       // properties creation
        shader_program = std::make_unique<ShaderProgram>(vertex_shader, fragment_shader);
        if (!shader_program->isCompiled())
            return -1;
        BufferLayout newlayout
        {
            BufferElement(ShaderDataType::Float3),
            BufferElement(ShaderDataType::Float3),
             BufferElement(ShaderDataType::Float2)

        };
        one_buffer = std::make_unique<VertexBuffer>(ones, sizeof(ones), newlayout);
        index_buffer = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(GLuint));
        // two_buffer = std::make_unique<VertexBuffer>(colors, sizeof(colors),newlayout,VertexBuffer::EUsage::Static);
        main_vao = std::make_unique<VertexArray>();

        main_vao->add_vertex_buffer(*one_buffer);
        main_vao->set_index_buffer(*index_buffer);
        // end

        Rendering_OpengGl::Enable_depth();
        while (!bCloseWindow)
        {
            //drawing begin
            Rendering_OpengGl::Set_clear_color(BackgroundRGB[0], BackgroundRGB[1], BackgroundRGB[2], BackgroundRGB[3]);
            Rendering_OpengGl::Clear();

            if (shader_program->isCompiled())
                shader_program->bind();
            Rendering_OpengGl::Draw(*main_vao);

            UserInterface::On_draw_begin();
            bool ShowDock = true;
            UserInterface::ShowExampleAppDockSpace(&ShowDock);
            //ImGui::ShowDemoWindow();
            ImGui::ShowDemoWindow();
            ImGui::Begin("Background Color");
            ImGui::ColorEdit4("RGB and Alpha settings", BackgroundRGB);
            // sliders for editing
            ImGui::SliderFloat3("scale", scale, 0.f, 2.f);
            ImGui::SliderFloat3("translate", translate, -1.f, 1.f);
            ImGui::SliderFloat("rotate", &rotate, 0.f, 360.f);
            ImGui::SliderFloat3("camera position", camera_position, -10.f, 10.f);
            ImGui::SliderFloat3("camera rotation", camera_rotation, 0, 360.f);
            ImGui::Checkbox("Perspective camera", &perspective_camera);

            //mathematics
            glm::mat4 scale_matrix(scale[0], 0, 0, 0,
                0, scale[1], 0, 0,
                0, 0, scale[2], 0,
                0, 0, 0, 1);

            float rotate_in_radians = glm::radians(rotate);
            glm::mat4 rotate_matrix(cos(rotate_in_radians), sin(rotate_in_radians), 0, 0,
                -sin(rotate_in_radians), cos(rotate_in_radians), 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1);

            glm::mat4 translate_matrix(1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                translate[0], translate[1], translate[2], 1);

            // matrix init
            shader_program->SetMatrix4("model_matrix", translate_matrix * rotate_matrix * scale_matrix);

            camera.set_projection_mode(perspective_camera ? Camera::ProjectionType::Perspective : Camera::ProjectionType::Orthographic);
            shader_program->SetMatrix4("camera_matrix", camera.get_projection_matrix() * camera.get_view_matrix());

           shader_program->SetImage("InTexture", maki_texture->Get_texture_id());
           Rendering_OpengGl::Draw(*main_vao);

           for (const glm::vec3& current_position : positions)
           {
               glm::mat4 nexttranslate_matrix(1, 0, 0, 0,
                   0, 1, 0, 0,
                   0, 0, 1, 0,
                   current_position[0], current_position[1], current_position[2], 1);
               shader_program->SetMatrix4("model_matrix", nexttranslate_matrix* rotate_matrix* scale_matrix);
               Rendering_OpengGl::Draw(*main_vao);
           }


            ImGui::End();

            On_draw_interface();

            UserInterface::On_draw_end();
            //drawing end

            AppWindow->on_update();
            On_update();
        }
        AppWindow = nullptr;
        return 0;
    }
  
    glm::vec2 Application::Get_cursor_position()
    {
        return AppWindow->Get_cursor_position();
    }
}