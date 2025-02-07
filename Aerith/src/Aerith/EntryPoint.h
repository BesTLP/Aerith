#pragma once
#include"iostream"
#ifdef AERITH_PLATFORM_WINDOWS

extern Aerith::Application* Aerith::CreateApplication();

int main(int argc, char** argv)
{
	std::cout << "Welcome To Aerith Game Engine!!!" << std::endl;
	auto app = Aerith::CreateApplication();
	app->Run();
	delete app;
}
#endif



