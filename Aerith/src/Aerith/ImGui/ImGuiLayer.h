#pragma once
#include "Aerith/Layer.h"
#include "Aerith/Events/Event.h"
#include "Aerith/Events/KeyEvent.h"
#include "Aerith/Events/MouseEvent.h"
#include "Aerith/Events/ApplicationEvent.h"

namespace Aerith
{
	class AERITH_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event& event);

	private:
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool OnMouseMovedEvent(MouseMovedEvent& e);
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);
		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnKeyReleasedEvent(KeyReleasedEvent& e);
		bool OnWindowResizeEvent(WindowResizeEvent& e);
		bool OnKeyTypedEvent(KeyTypedEvent e);

	private:
		float m_Time = 0.0f;
	};
}