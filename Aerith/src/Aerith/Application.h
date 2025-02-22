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

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }
	private: 
		// WindowClose Bind Event Function
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;

		bool m_Running = true;

		LayerStack m_LayerStack;
		static Application* s_Instance;
	};

	// To be defined in CLINET
	Application* CreateApplication();
}
