#pragma once

#ifdef AERITH_PLATFORM_WINDOWS

extern Aerith::Application* Aerith::CreateApplication();

int main(int argc, char** argv)
{
	Aerith::Log::Init();
	AERITH_CORE_WARN("Initialized Aerith Game Engine Log!");
	AERITH_INFO("Welcome to Aerith Game Engine!");
	auto app = Aerith::CreateApplication();
	app->Run();
	delete app;
}
#endif



