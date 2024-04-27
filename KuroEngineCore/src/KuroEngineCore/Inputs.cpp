#include "KuroEngineCore/Inputs.h"

namespace KuroEngine
{
	bool Inputs::IsKeyPressed(const InputKeys key_code)
	{
		return keys_pressed[static_cast<size_t>(key_code)];
	}
	void Inputs::PressKey(const InputKeys key_code)
	{
		keys_pressed[static_cast<size_t>(key_code)]=true;
	}
	void Inputs::ReleaseKey(const InputKeys key_code)
	{
		keys_pressed[static_cast<size_t>(key_code)] = false;
	}

	bool Inputs::IsButtonPressed(const MouseButtons button_code)
	{
		return buttons_pressed[static_cast<size_t>(button_code)];
	}

	void Inputs::PressButton(const MouseButtons button_code)
	{
		buttons_pressed [static_cast<size_t>(button_code)] = true;
	}

	void Inputs::ReleaseButton(const MouseButtons button_code)
	{
		buttons_pressed[static_cast<size_t>(button_code)] = false;
	}

	bool Inputs::keys_pressed[static_cast<size_t>(InputKeys::KEY_LAST)+1] = {};
	bool Inputs::buttons_pressed[static_cast<size_t>(MouseButtons::MOUSE_BUTTON_8) + 1] = {};
}