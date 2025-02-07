#pragma once
#include "Core.h"

namespace Aerith
{
	class AERITH_API Application
	{
	public:
		Application();

		virtual ~Application();

		void Run();
	};

	// To be defined in CLINET
	Application* CreateApplication();
}
