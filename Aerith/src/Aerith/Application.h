#pragma once
#include "Core.h"
#include "Aerith/Window.h"
#include "Events/ApplicationEvent.h"

#include "Aerith/LayerStack.h"

namespace Aerith
{
	class AERITH_API Application
	{
	public:
		Application();

		virtual ~Application();

		void Run();

		// Event Operation
		void OnEvent(Event& e);
		
		// Layer Operation
		void PushLayer(Layer* layer);
		
		void PushOverlay(Layer* layer);

	private: 
		// WindowClose Bind Event Function
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;

		bool m_Running = true;

		LayerStack m_LayerStack;
	};

	// To be defined in CLINET
	Application* CreateApplication();
}
