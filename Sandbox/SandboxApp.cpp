#include "Aerith.h"

class Sandbox :public Aerith::Application
{
public:
	Sandbox(){}

	~Sandbox(){}

};

Aerith::Application* Aerith::CreateApplication()
{
	return new Sandbox();
}