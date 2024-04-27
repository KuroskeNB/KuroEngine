#pragma once
#include <functional>
#include <array> 
#include "KuroEngineCore/Inputs.h"

namespace KuroEngine
{
	enum class EventType
	{
		WindowResize = 0,
		WindowClose,
		KeyPressed,
		KeyReleased,
		MouseButtonPressed,
		MouseButtonReleased,
		MouseMoved,

		EventsCount
	};

	struct BaseEvent
	{
		virtual ~BaseEvent() = default;
		virtual EventType GetEventType() const = 0;
	};

	class EventDispatcher
	{
	public:
		template<typename EventName>
		void add_event_listener(std::function<void(EventName&)> callback)
		{
			auto baseCallback = [func = std::move(callback)](BaseEvent& e)
				{
					if (e.GetEventType() == EventName::type)
					{
						func(static_cast<EventName&>(e));
					}
				};
			EventCallbacks[static_cast<size_t>(EventName::type)] = std::move(baseCallback);
		}

		void dispatch(BaseEvent& event)
		{
			auto& callback = EventCallbacks[static_cast<size_t>(event.GetEventType())];
			if (callback)
			{
				callback(event);
			}
		}

	private:
		std::array<std::function<void(BaseEvent&)>, static_cast<size_t>(EventType::EventsCount)> EventCallbacks;
	};


	struct EventMouseMoved : public BaseEvent
	{
		EventMouseMoved(const unsigned int new_x, const unsigned int new_y)
			: x(new_x),y(new_y)
		{
		}

		unsigned int x;
		unsigned int y;
		virtual EventType GetEventType() const { return type; };
		static const EventType type = EventType::MouseMoved;
	};

	struct EventWindowResize : public BaseEvent
	{
		EventWindowResize(const unsigned int new_width, const unsigned int new_height)
			:width(new_width),height(new_height)
		{}

		unsigned int width;
		unsigned int height;
		virtual EventType GetEventType() const { return type; };
		static const EventType type = EventType::WindowResize;
	};

	struct EventWindowClose : public BaseEvent
	{
		virtual EventType GetEventType() const { return type; };
		static const EventType type = EventType::WindowClose;
	};

	struct EventKeyPressed : public BaseEvent
	{
		EventKeyPressed(const InputKeys Key,const bool repeat)
		{
			KeyCode = Key;
			repeatable = repeat;
		}
		InputKeys KeyCode;
		bool repeatable=false;
		virtual EventType GetEventType() const { return type; };
		static const EventType type = EventType::KeyPressed;
	};

	struct EventKeyReleased : public BaseEvent
	{
		EventKeyReleased(const InputKeys Key, const bool repeat)
		{
			KeyCode = Key;
			repeatable = repeat;
		}
		InputKeys KeyCode;
		bool repeatable = false;
		virtual EventType GetEventType() const { return type; };
		static const EventType type = EventType::KeyReleased;
	};
	
	struct EventMouseButtonPressed : public BaseEvent
	{
		EventMouseButtonPressed(const MouseButtons Button, const double x, const double y)
		{
			ButtonCode = Button;
			this->x = x;
			this->y = y;
		}
		MouseButtons ButtonCode;
		double x;
		double y;
		virtual EventType GetEventType() const { return type; };
		static const EventType type = EventType::MouseButtonPressed;
	};
	struct EventMouseButtonReleased : public BaseEvent
	{
		EventMouseButtonReleased(const MouseButtons Button, const double x, const double y)
		{
			ButtonCode = Button;
			this->x = x;
			this->y = y;
		}
		MouseButtons ButtonCode;
		double x;
		double y;
		virtual EventType GetEventType() const { return type; };
		static const EventType type = EventType::MouseButtonReleased;
	};
}