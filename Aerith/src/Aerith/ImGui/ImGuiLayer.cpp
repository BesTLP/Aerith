#include "AerithPch.h"
#include "ImGuiLayer.h"

#include "Aerith/Platform/OpenGL/ImGuiOpenGLRenderer.h"
#include "imgui.h"
#include "Aerith/Application.h"

#include "GLFW/glfw3.h"
#include <glad/glad.h>

namespace Aerith
{

	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnAttach()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();

		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
        io.KeyMap[GLFW_KEY_TAB] = ImGuiKey_Tab;
        io.KeyMap[GLFW_KEY_LEFT] = ImGuiKey_LeftArrow;
        io.KeyMap[GLFW_KEY_RIGHT] = ImGuiKey_RightArrow;
        io.KeyMap[GLFW_KEY_UP] = ImGuiKey_UpArrow;
        io.KeyMap[GLFW_KEY_DOWN] = ImGuiKey_DownArrow;
        io.KeyMap[GLFW_KEY_PAGE_UP] = ImGuiKey_PageUp;
        io.KeyMap[GLFW_KEY_PAGE_DOWN] = ImGuiKey_PageDown;
        io.KeyMap[GLFW_KEY_HOME] = ImGuiKey_Home;
        io.KeyMap[GLFW_KEY_END] = ImGuiKey_End;
        io.KeyMap[GLFW_KEY_INSERT] = ImGuiKey_Insert;
        io.KeyMap[GLFW_KEY_DELETE] = ImGuiKey_Delete;
        io.KeyMap[GLFW_KEY_BACKSPACE] = ImGuiKey_Backspace;
        io.KeyMap[GLFW_KEY_SPACE] = ImGuiKey_Space;
        io.KeyMap[GLFW_KEY_ENTER] = ImGuiKey_Enter;
        io.KeyMap[GLFW_KEY_ESCAPE] = ImGuiKey_Escape;
        io.KeyMap[GLFW_KEY_APOSTROPHE] = ImGuiKey_Apostrophe;
        io.KeyMap[GLFW_KEY_COMMA] = ImGuiKey_Comma;
        io.KeyMap[GLFW_KEY_MINUS] = ImGuiKey_Minus;
        io.KeyMap[GLFW_KEY_PERIOD] = ImGuiKey_Period;
        io.KeyMap[GLFW_KEY_SLASH] = ImGuiKey_Slash;
        io.KeyMap[GLFW_KEY_SEMICOLON] = ImGuiKey_Semicolon;
        io.KeyMap[GLFW_KEY_EQUAL] = ImGuiKey_Equal;
        io.KeyMap[GLFW_KEY_LEFT_BRACKET] = ImGuiKey_LeftBracket;
        io.KeyMap[GLFW_KEY_BACKSLASH] = ImGuiKey_Backslash;
        io.KeyMap[GLFW_KEY_RIGHT_BRACKET] = ImGuiKey_RightBracket;
        io.KeyMap[GLFW_KEY_GRAVE_ACCENT] = ImGuiKey_GraveAccent;
        io.KeyMap[GLFW_KEY_CAPS_LOCK] = ImGuiKey_CapsLock;
        io.KeyMap[GLFW_KEY_SCROLL_LOCK] = ImGuiKey_ScrollLock;
        io.KeyMap[GLFW_KEY_NUM_LOCK] = ImGuiKey_NumLock;
        io.KeyMap[GLFW_KEY_PRINT_SCREEN] = ImGuiKey_PrintScreen;
        io.KeyMap[GLFW_KEY_PAUSE] = ImGuiKey_Pause;
        io.KeyMap[GLFW_KEY_KP_0] = ImGuiKey_Keypad0;
        io.KeyMap[GLFW_KEY_KP_1] = ImGuiKey_Keypad1;
        io.KeyMap[GLFW_KEY_KP_2] = ImGuiKey_Keypad2;
        io.KeyMap[GLFW_KEY_KP_3] = ImGuiKey_Keypad3;
        io.KeyMap[GLFW_KEY_KP_4] = ImGuiKey_Keypad4;
        io.KeyMap[GLFW_KEY_KP_5] = ImGuiKey_Keypad5;
        io.KeyMap[GLFW_KEY_KP_6] = ImGuiKey_Keypad6;
        io.KeyMap[GLFW_KEY_KP_7] = ImGuiKey_Keypad7;
        io.KeyMap[GLFW_KEY_KP_8] = ImGuiKey_Keypad8;
        io.KeyMap[GLFW_KEY_KP_9] = ImGuiKey_Keypad9;
        io.KeyMap[GLFW_KEY_KP_DECIMAL] = ImGuiKey_KeypadDecimal;
        io.KeyMap[GLFW_KEY_KP_DIVIDE] = ImGuiKey_KeypadDivide;
        io.KeyMap[GLFW_KEY_KP_MULTIPLY] = ImGuiKey_KeypadMultiply;
        io.KeyMap[GLFW_KEY_KP_SUBTRACT] = ImGuiKey_KeypadSubtract;
        io.KeyMap[GLFW_KEY_KP_ADD] = ImGuiKey_KeypadAdd;
        io.KeyMap[GLFW_KEY_KP_ENTER] = ImGuiKey_KeypadEnter;
        io.KeyMap[GLFW_KEY_KP_EQUAL] = ImGuiKey_KeypadEqual;
        io.KeyMap[GLFW_KEY_LEFT_SHIFT] = ImGuiKey_LeftShift;
        io.KeyMap[GLFW_KEY_LEFT_CONTROL] = ImGuiKey_LeftCtrl;
        io.KeyMap[GLFW_KEY_LEFT_ALT] = ImGuiKey_LeftAlt;
        io.KeyMap[GLFW_KEY_LEFT_SUPER] = ImGuiKey_LeftSuper;
        io.KeyMap[GLFW_KEY_RIGHT_SHIFT] = ImGuiKey_RightShift;
        io.KeyMap[GLFW_KEY_RIGHT_CONTROL] = ImGuiKey_RightCtrl;
        io.KeyMap[GLFW_KEY_RIGHT_ALT] = ImGuiKey_RightAlt;
        io.KeyMap[GLFW_KEY_RIGHT_SUPER] = ImGuiKey_RightSuper;
        io.KeyMap[GLFW_KEY_MENU] = ImGuiKey_Menu;
        io.KeyMap[GLFW_KEY_0] = ImGuiKey_0;
        io.KeyMap[GLFW_KEY_1] = ImGuiKey_1;
        io.KeyMap[GLFW_KEY_2] = ImGuiKey_2;
        io.KeyMap[GLFW_KEY_3] = ImGuiKey_3;
        io.KeyMap[GLFW_KEY_4] = ImGuiKey_4;
        io.KeyMap[GLFW_KEY_5] = ImGuiKey_5;
        io.KeyMap[GLFW_KEY_6] = ImGuiKey_6;
        io.KeyMap[GLFW_KEY_7] = ImGuiKey_7;
        io.KeyMap[GLFW_KEY_8] = ImGuiKey_8;
        io.KeyMap[GLFW_KEY_9] = ImGuiKey_9;
        io.KeyMap[GLFW_KEY_A] = ImGuiKey_A;
        io.KeyMap[GLFW_KEY_B] = ImGuiKey_B;
        io.KeyMap[GLFW_KEY_C] = ImGuiKey_C;
        io.KeyMap[GLFW_KEY_D] = ImGuiKey_D;
        io.KeyMap[GLFW_KEY_E] = ImGuiKey_E;
        io.KeyMap[GLFW_KEY_F] = ImGuiKey_F;
        io.KeyMap[GLFW_KEY_G] = ImGuiKey_G;
        io.KeyMap[GLFW_KEY_H] = ImGuiKey_H;
        io.KeyMap[GLFW_KEY_I] = ImGuiKey_I;
        io.KeyMap[GLFW_KEY_J] = ImGuiKey_J;
        io.KeyMap[GLFW_KEY_K] = ImGuiKey_K;
        io.KeyMap[GLFW_KEY_L] = ImGuiKey_L;
        io.KeyMap[GLFW_KEY_M] = ImGuiKey_M;
        io.KeyMap[GLFW_KEY_N] = ImGuiKey_N;
        io.KeyMap[GLFW_KEY_O] = ImGuiKey_O;
        io.KeyMap[GLFW_KEY_P] = ImGuiKey_P;
        io.KeyMap[GLFW_KEY_Q] = ImGuiKey_Q;
        io.KeyMap[GLFW_KEY_R] = ImGuiKey_R;
        io.KeyMap[GLFW_KEY_S] = ImGuiKey_S;
        io.KeyMap[GLFW_KEY_T] = ImGuiKey_T;
        io.KeyMap[GLFW_KEY_U] = ImGuiKey_U;
        io.KeyMap[GLFW_KEY_V] = ImGuiKey_V;
        io.KeyMap[GLFW_KEY_W] = ImGuiKey_W;
        io.KeyMap[GLFW_KEY_X] = ImGuiKey_X;
        io.KeyMap[GLFW_KEY_Y] = ImGuiKey_Y;
        io.KeyMap[GLFW_KEY_Z] = ImGuiKey_Z;
        io.KeyMap[GLFW_KEY_F1] = ImGuiKey_F1;
        io.KeyMap[GLFW_KEY_F2] = ImGuiKey_F2;
        io.KeyMap[GLFW_KEY_F3] = ImGuiKey_F3;
        io.KeyMap[GLFW_KEY_F4] = ImGuiKey_F4;
        io.KeyMap[GLFW_KEY_F5] = ImGuiKey_F5;
        io.KeyMap[GLFW_KEY_F6] = ImGuiKey_F6;
        io.KeyMap[GLFW_KEY_F7] = ImGuiKey_F7;
        io.KeyMap[GLFW_KEY_F8] = ImGuiKey_F8;
        io.KeyMap[GLFW_KEY_F9] = ImGuiKey_F9;
        io.KeyMap[GLFW_KEY_F10] = ImGuiKey_F10;
        io.KeyMap[GLFW_KEY_F11] = ImGuiKey_F11;
        io.KeyMap[GLFW_KEY_F12] = ImGuiKey_F12;

        ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach()
	{
	}

	void ImGuiLayer::OnUpdate()
	{
        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::Get();
        io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

        float time = (float)glfwGetTime();
        io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f);
        m_Time = time;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        static bool show = true;
        ImGui::ShowDemoWindow(&show);
        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

	void ImGuiLayer::OnEvent(Event& event)
	{
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<MouseButtonPressedEvent>(AERITH_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
        dispatcher.Dispatch<MouseButtonReleasedEvent>(AERITH_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
        dispatcher.Dispatch<MouseMovedEvent>(AERITH_BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
        dispatcher.Dispatch<MouseScrolledEvent>(AERITH_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));
        dispatcher.Dispatch<KeyPressedEvent>(AERITH_BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
        dispatcher.Dispatch<KeyReleasedEvent>(AERITH_BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
        dispatcher.Dispatch<WindowResizeEvent>(AERITH_BIND_EVENT_FN(ImGuiLayer::OnWindowResizeEvent));
        dispatcher.Dispatch<WindowResizeEvent>(AERITH_BIND_EVENT_FN(ImGuiLayer::OnWindowResizeEvent));
        dispatcher.Dispatch<KeyTypedEvent>(AERITH_BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));
	}

    bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[e.GetMouseButton()] = true;

        return false;
    }

    bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[e.GetMouseButton()] = false;
        return false;
    }

    bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MousePos = ImVec2(e.GetX(), e.GetY());
        return false;
    }

    bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseWheel += e.GetXOffset();
        io.MouseWheelH + e.GetYOffset();

        return false;
    }

    bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[e.GetKeyCode()] = true;

        io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
        io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
        io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
        io.KeySuper= io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
        return false;
    }

    bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[e.GetKeyCode()] = false;
        return false;
    }

    bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
        io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
        glViewport(0, 0, e.GetWidth(), e.GetHeight());

        return false;
    }

    bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent e)
    {
        ImGuiIO& io = ImGui::GetIO();
        int keycode = e.GetKeyCode();
        if (keycode > 0 && keycode < 0x10000)
            io.AddInputCharacter((unsigned short)keycode);

        return false;
    }

}

