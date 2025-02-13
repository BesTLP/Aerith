#pragma once
#include "Core.h"
#include "Aerith/Window.h"
#include "Events/ApplicationEvent.h"

namespace Aerith
{
	class AERITH_API Application
	{
	public:
		Application();

		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		bool OnWindowClose(WindowCloseEvent& e);
	private: 
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	// To be defined in CLINET
	Application* CreateApplication();
}
