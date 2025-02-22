#include "Aerith.h"

class ExampleLayer : public Aerith::Layer
{
public:
	ExampleLayer() :Layer("Example")
	{

	}

	void OnUpdate() override
	{

		AERITH_INFO("ExampleLayer::Update");
	}

	void OnEvent(Aerith::Event& event) override
	{
		AERITH_TRACE("{0}", event);
	}
};
class Sandbox :public Aerith::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Aerith::ImGuiLayer());
	}

	~Sandbox(){}

};

Aerith::Application* Aerith::CreateApplication()
{
	return new Sandbox();
}