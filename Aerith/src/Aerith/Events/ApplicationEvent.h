#pragma once
#include "Event.h"

namespace Aerith
{
	class AERITH_API WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			:m_Width(width), m_Height(height) {}

		inline int GetWidth() { return m_Width; }
		inline int GetHeight() { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);

	private:
		unsigned int m_Width, m_Height;
	};

	class AERITH_API WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowCloseEvent: ";
			return ss.str();
		}
		EVENT_CLASS_TYPE(WindowClose);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	};

	class AERITH_API AppTick : public Event
	{
	public:
		AppTick() {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "AppTick: ";
			return ss.str();
		}
		EVENT_CLASS_TYPE(AppTick);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	};

	class AERITH_API AppUpdate : public Event
	{
	public:
		AppUpdate() {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "AppUpdate: ";
			return ss.str();
		}
		EVENT_CLASS_TYPE(AppUpdate);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);

	};

	class AERITH_API AppRender : public Event
	{
	public:
		AppRender() {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "AppRender: ";
			return ss.str();
		}
		EVENT_CLASS_TYPE(AppRender);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	};
}