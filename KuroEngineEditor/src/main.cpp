#include <iostream>
#include "KuroEngineCore/Application.h"
#include <imgui/imgui.h>
#include <memory>
#include "KuroEngineCore/Inputs.h"

class KuroSpace :public KuroEngine::Application
{
public:

    double Start_cursor_pos_x = 0;
    double Start_cursor_pos_y = 0;
    virtual void On_button_updated(const KuroEngine::MouseButtons button_code, const double x_pos, const double y_pos, const bool pressed) override
    {
        Start_cursor_pos_x = x_pos;
        Start_cursor_pos_y = y_pos;
    };

	virtual void On_update() override
	{
        glm::vec3 movement_delta{ 0, 0, 0 };
        glm::vec3 rotation_delta{ 0, 0, 0 };

        if (KuroEngine::Inputs::IsKeyPressed(KuroEngine::InputKeys::KEY_W))
        {
            movement_delta.x += 0.05f;
        }
        if (KuroEngine::Inputs::IsKeyPressed(KuroEngine::InputKeys::KEY_S))
        {
            movement_delta.x -= 0.05f;
        }
        if (KuroEngine::Inputs::IsKeyPressed(KuroEngine::InputKeys::KEY_A))
        {
            movement_delta.y -= 0.05f;
        }
        if (KuroEngine::Inputs::IsKeyPressed(KuroEngine::InputKeys::KEY_D))
        {
            movement_delta.y += 0.05f;
        }
        if (KuroEngine::Inputs::IsKeyPressed(KuroEngine::InputKeys::KEY_E))
        {
            movement_delta.z += 0.05f;
        }
        if (KuroEngine::Inputs::IsKeyPressed(KuroEngine::InputKeys::KEY_Q))
        {
            movement_delta.z -= 0.05f;
        }
        if (KuroEngine::Inputs::IsKeyPressed(KuroEngine::InputKeys::KEY_UP))
        {
            rotation_delta.y -= 0.5f;
        } 
        if (KuroEngine::Inputs::IsKeyPressed(KuroEngine::InputKeys::KEY_DOWN))
        {
            rotation_delta.y += 0.5f;
        }
        if (KuroEngine::Inputs::IsKeyPressed(KuroEngine::InputKeys::KEY_RIGHT))
        {
            rotation_delta.z -= 0.5f;
        }
        if (KuroEngine::Inputs::IsKeyPressed(KuroEngine::InputKeys::KEY_LEFT))
        {
            rotation_delta.z += 0.5f;
        }
        if (KuroEngine::Inputs::IsKeyPressed(KuroEngine::InputKeys::KEY_P))
        {
            rotation_delta.x += 0.5f;
        }
        if (KuroEngine::Inputs::IsKeyPressed(KuroEngine::InputKeys::KEY_O))
        {
            rotation_delta.x -= 0.5f;
        }

        if (KuroEngine::Inputs::IsButtonPressed(KuroEngine::MouseButtons::MOUSE_BUTTON_RIGHT))
        {
            glm::vec2 Cursor_pos = Get_cursor_position();
            if (KuroEngine::Inputs::IsButtonPressed(KuroEngine::MouseButtons::MOUSE_BUTTON_LEFT))
            {
                camera.move_right((Start_cursor_pos_x - Cursor_pos.x) / 7);
                camera.move_up((Start_cursor_pos_y - Cursor_pos.y) / 7);
            }
            else
            {
                rotation_delta.z += (Start_cursor_pos_x - Cursor_pos.x)/5;
                rotation_delta.y -= (Start_cursor_pos_y - Cursor_pos.y)/5;
                Start_cursor_pos_x = Cursor_pos.x;
                Start_cursor_pos_y = Cursor_pos.y;
            }
            Start_cursor_pos_x = Cursor_pos.x;
            Start_cursor_pos_y = Cursor_pos.y;
        }
          
        camera.add_movement_and_rotation(movement_delta, rotation_delta);
        
	}
   
	virtual void On_draw_interface() override
	{
        camera_position[0] = camera.get_camera_position().x;
        camera_position[1] = camera.get_camera_position().y;
        camera_position[2] = camera.get_camera_position().z;
        camera_rotation[0] = camera.get_camera_rotation().x;
        camera_rotation[1] = camera.get_camera_rotation().y;
        camera_rotation[2] = camera.get_camera_rotation().z;

		ImGui::Begin("main");
        if (ImGui::SliderFloat3("camera position", camera_position, -10.f, 10.f))
        {
            camera.set_position(glm::vec3(camera_position[0], camera_position[1], camera_position[2]));
        }
        if (ImGui::SliderFloat3("camera rotation", camera_rotation, 0, 360.f))
        {
            camera.set_rotation(glm::vec3(camera_rotation[0], camera_rotation[1], camera_rotation[2]));
        }
		ImGui::Checkbox("Perspective camera", &perspective_camera);
		ImGui::End();
	}
};

int main()
{
	auto MyApp= std::make_unique<KuroSpace>();
	std::cout << "ya ne gey " << std::endl;
	MyApp->start(1980, 1280, "mdma");
	return 0;
}