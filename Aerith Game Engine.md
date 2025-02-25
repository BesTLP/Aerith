# 什么是游戏引擎

核心就是实时可视化的交互式应用程序，是一个平台可以为用户来制作应用程序，可以是视频，建筑可视化，也可以是游戏。本质上就是读取数据然后转换为我们想要的形式来呈现，即使是某些不懂代码的人，我们也可以帮助其呈现。

# 游戏引擎

- Entry Point：我们启动引擎的时候会发生什么。

- Application Layout：处理应用程序的生命周期和事件等，比如保持时间向前，或者渲染，或者键盘输入。

- Window Layout：
  - Input
  - Event
- Renderer:渲染图形
- Render API abstraction
- Debugging Support
- Scripting Language
- Memory Systems
- Entity Component System
- Physics
- File I/O，Virtual File System
- Build System

# 准备项目

![image-20250207135339014](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\image-20250207135339014.png)

对于在Aerith中编写的函数，我们都将其配置为.dll的形式，然后让另外的项目来调用

![image-20250207135520469](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\image-20250207135520469.png)

✅ **`_declspec(dllexport)`**

- 用于 DLL 内部，告诉编译器：**我要导出这个函数**，让其他程序可以调用它。

✅ **`_declspec(dllimport)`**

- 用于使用 DLL 的程序，告诉编译器：**这个函数在 DLL 里面**，我需要从 DLL 里找到它。

## Core.h

Core用于管理Windows平台下的动态链接库（DLL）导入和导出。

- 根据 `AERITH_BUILD_DLL`，定义 `AERITH_API` 为 `__declspec(dllexport)`（导出）或 `__declspec(dllimport)`（导入）。
- 如果不是 Windows 平台，编译时报错。

```c++
#pragma once
#ifdef AERITH_PLATFORM_WINDOWS
	#ifdef AERITH_BUILD_DLL
		#define AERITH_API _declspec(dllexport)
	#else 
		#define AERITH_API _declspec(dllimport)
	#endif
#else 
	#error Only Support Windows Now.
#endif // AERITH_PLATFORM_WINDOWS
```

![image-20250207153340320](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\image-20250207153340320.png)

## Application

为应用程序框架提供了基本的生命周期管理，客户端需要实现 `CreateApplication()` 来实例化 `Application`，然后调用 `Run()` 进入主循环。

```c++
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

```

```c++
#include "Application.h"
#include "iostream"
namespace Aerith
{
	Application::Application()
	{}
	Application::~Application()
	{}

	void Application::Run()
	{
		while (true)
		{}
	}

}
```

比如我们构建了一个SandBoxApplication来继承Application，就需要实现对应的应用创建功能

```c++
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
```

## EntryPoint

**`extern Aerith::Application\* Aerith::CreateApplication()`**：

声明外部 `CreateApplication()` 函数，该函数由客户端实现，用于创建 `Application` 实例。

```c++
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
```

# SPDLOG（日志系统）

使用SPDLOG来构建日志系统

```c++
#pragma once

#include <memory>

#include "Core.h"

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Aerith
{
	class AERITH_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_CoreLogger; }

	private:
		// 定义为 static,
		// 使用Log的时候不用构建对应的类实例
		// 并且共用同一个 Logger
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;

	};

}

// 加 :: 的情况下会始终解析为全局 Aerith::Log，不会受到 using namespace 影响
// Aerith::Log::GetCoreLogger()->warn("Initialized Game Engine!");
// Aerith::Log::GetClientLogger()->info("Welcome to Aerith Game Engine!");

// Core Log Macros
#define AERITH_CORE_TRACE(...)		::Aerith::Log::GetCoreLogger()->trace(__VA_ARGS__);
#define AERITH_CORE_DEBUG(...)		::Aerith::Log::GetCoreLogger()->debug(__VA_ARGS__);
#define AERITH_CORE_INFO(...)		::Aerith::Log::GetCoreLogger()->info(__VA_ARGS__);
#define AERITH_CORE_WARN(...)		::Aerith::Log::GetCoreLogger()->warn(__VA_ARGS__);
#define AERITH_CORE_ERROR(...)		::Aerith::Log::GetCoreLogger()->error(__VA_ARGS__);
#define AERITH_CORE_CRITICAL(...)	::Aerith::Log::GetCoreLogger()->critical(__VA_ARGS__);


// Client Log Macros
#define AERITH_TRACE(...)			::Aerith::Log::GetClientLogger()->trace(__VA_ARGS__);
#define AERITH_DEBUG(...)			::Aerith::Log::GetClientLogger()->debug(__VA_ARGS__);
#define AERITH_INFO(...)			::Aerith::Log::GetClientLogger()->info(__VA_ARGS__);
#define AERITH_WARN(...)			::Aerith::Log::GetClientLogger()->warn(__VA_ARGS__);
#define AERITH_ERROR(...)			::Aerith::Log::GetClientLogger()->error(__VA_ARGS__);
#define AERITH_CRITICAL(...)		::Aerith::Log::GetClientLogger()->critical(__VA_ARGS__);



#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"
namespace Aerith
{
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;


	void Log::Init()
	{
		// Set Log Pattern, ref spdlog wiki
		spdlog::set_pattern("%^[%T] %n: %v%$");

		// Set Core Logger
		s_CoreLogger = spdlog::stdout_color_mt("Aerith");
		s_CoreLogger->set_level(spdlog::level::trace);

		// Set Client Logger
		s_ClientLogger = spdlog::stderr_color_mt("Client");
		s_ClientLogger->set_level(spdlog::level::trace);
	}
}
```



# PREMAKE

使用PREMAKE来预构建程序

# EVENT SYSTEM

## 事件系统说明

本事件系统的设计理念是实现事件的解耦和灵活传播，使得不同层级的组件可以独立地处理和响应事件，而不直接依赖于其他系统的实现细节。下面是系统的工作流程和关键概念。

### 1. **事件创建与传播**

事件系统的核心是事件对象。每当用户与界面交互时，例如点击、键盘输入等，系统会生成一个 `Event` 对象，包含事件类型、事件源、时间戳等信息。事件对象由应用程序的顶层组件（例如 `Application`）接收并传播。

- **事件生成：** 当 `Window` 或其他 UI 组件接收到用户交互时，系统会生成一个事件对象（`Event`），封装了事件的相关信息。
- **事件传递：** 事件通过 `Application` 类进行传播。具体地，`Window` 类并不知道 `Application` 类的存在，但它将通过某种机制将事件传递到上层进行处理。

### 2. **解耦设计：回调机制**

为了使各个组件之间解耦，我们为每个组件（如 `Window`）提供回调机制。在创建 `Window` 时，可以为该窗口设置一个或多个回调函数。当事件发生时，系统会检查这些回调函数是否已注册。如果有回调函数，事件就会触发相应的回调处理。

- **回调函数：** 每个组件（如 `Window`）可以注册一个回调函数。该函数会在事件发生时被调用，负责处理事件逻辑。
- **事件处理：** 当 `Window` 接收到事件时，它会检查是否存在回调函数。如果存在，则触发回调函数执行事件处理逻辑。

### 3. **应用程序与窗口的关系**

通过这种设计，`Window` 类不需要直接依赖于 `Application` 类，避免了紧密耦合。`Application` 类负责事件的调度和管理，而 `Window` 只需关注如何响应事件。

- **无直接依赖：** `Window` 类只负责事件的触发和回调的执行，不需要知道 `Application` 的具体实现。反之，`Application` 也只知道如何接收和分发事件，不需要关心具体的事件处理逻辑。
- **灵活性：** 这种方式使得每个组件能够独立地处理事件，使得系统更具扩展性和灵活性。

## Event.h

**`enum class EventType`**

定义了不同的事件类型。`EventType` 是事件的“种类”，用于标识不同的事件。

- `None`: 无事件。
- `WindowClose`: 窗口关闭事件。
- `WindowResize`: 窗口尺寸改变事件。
- `WindowFocus`: 窗口获得焦点事件。
- `WindowLostFocus`: 窗口失去焦点事件。
- `WindowMoved`: 窗口移动事件。
- `AppTick`, `AppUpdate`, `AppRender`: 应用程序的各个生命周期阶段事件。
- `KeyPressed`, `KeyReleased`: 键盘按下和释放事件。
- `MouseButtonPressed`, `MouseButtonReleased`: 鼠标按钮按下和释放事件。
- `MouseMoved`, `MouseScrolled`: 鼠标移动和滚轮滚动事件。

---

**`enum EventCategory`**

用于定义事件的类别，每个事件类型可以属于一个或多个类别。这样可以根据类别过滤事件。

- `None`: 无事件类别。
- `EventCategoryApplication`: 应用程序相关的事件。
- `EventCategoryInput`: 输入事件（包括键盘、鼠标等）。
- `EventCategoryKeyboard`: 键盘事件。
- `EventCategoryMouse`: 鼠标相关事件。
- `EventCategoryMouseButton`: 鼠标按钮事件。

---

这些类别是使用 `BIT(x)` 宏来设置的，`BIT(x)` 是一个常见的位运算宏，`(1 << x)` 用来获取 `x` 位置的二进制位。例如，`EventCategoryApplication` 是 `1 << 0`，意味着它的值是 `1`。

使用位运算来表示不同的类型或类别，特别是在枚举（`enum`）类型中，确实是因为**类型之间可能会有重叠**，而位运算提供了一种高效的方式来**组合、检查和操作这些重叠的类型**。

由于每个类别占用不同的位，我们可以通过按位或（`|`）运算来组合多个类别。例如，一个事件同时属于 `EventCategoryApplication` 和 `EventCategoryKeyboard`，我们可以通过：

```c++
int combinedFlags = EventCategoryApplication | EventCategoryKeyboard;
```

这将产生 `0001 | 0100 = 0101`，即 `EventCategoryApplication` 和 `EventCategoryKeyboard` 被组合在一起。

---

 **`class AERITH_API Event`**

定义了一个名为 `Event` 的抽象类，所有具体事件类型都应该继承它。`AERITH_API` 是一个宏，通常用于控制导出和导入符号，确保在不同平台上正确链接。

- `virtual EventType GetEventType() const = 0;`: 纯虚函数，每个继承类必须实现。返回事件的类型。
- `virtual const char* GetName() const = 0;`: 纯虚函数，返回事件名称的字符串表示。
- `virtual int GetCategoryFlags() const = 0;`: 纯虚函数，返回事件所属类别的标志。
- `virtual std::string ToString() const`: 默认返回事件名称（可以被子类重写）。
- `inline bool IsInCategory(EventCategory category)`: 判断当前事件是否属于某个类别。

> `= 0` 用于声明纯虚函数。它的作用是让一个函数成为**纯虚函数**，表明该函数没有在当前类中提供实现，必须由继承该类的子类去实现。

`protected: bool m_Handled = false;`

`m_Handled` 是一个标志，表示事件是否已被处理。在事件分发过程中，事件是否被处理是非常重要的，通常用来避免重复处理同一个事件。

**宏 `EVENT_CLASS_TYPE`**

这是一个宏，用于为事件类添加 `GetEventType()` 和 `GetName()` 函数的实现。

```c++
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType(){ return EventType::##type; } \
							virtual EventType GetEventType() const override { return GetStaticType(); } \
    						virtual const char* GetName() const override { return #type; }
```

它会为事件类提供静态的 `GetStaticType()` 和动态的 `GetEventType()`，返回事件类型。

同时，`GetName()` 返回类型名称字符串（如 `"KeyPressed"`）。这个宏可以在派生类中使用，避免每次都手动写这两个方法。

>### `#type`（字符串化操作符）
>
>`#` 是**字符串化操作符**（Stringification Operator），它的作用是将宏参数转换为一个字符串字面量。具体来说，它会把宏参数（例如 `type`）转换成该参数的名称字符串。
>
>### `##type`（连接操作符）
>
>`##` 是**连接操作符**（Token Pasting Operator），它用于连接宏参数和其他文本。这意味着它可以把两个标记（token）连接成一个新的标记。

**宏 `EVENT_CLASS_CATEGORY`**

这个宏为事件类添加 `GetCategoryFlags()` 的实现，返回事件的类别标志。

```c++
#define EVENT_CLASS_CATEGORY(category) \
    virtual int GetCategoryFlags() const override { return category; }
```

它会为派生类提供类别标志（如 `EventCategoryKeyboard`）的返回值，标明事件属于哪个类别。

**`class EventDispatcher`**

`EventDispatcher` 类负责事件的分发。它是事件处理系统的关键部分。

之后我再来细讲。

**`inline std::string format_as(const Event& e)`**

这个内联函数返回事件的字符串表示，调用事件的 `ToString()` 方法，主要是为了**SPDLOG使用**

## MouseEvent.h

 `MouseEvent.h` 文件包含了一个用于鼠标事件系统的类层次结构，其中定义了不同的鼠标事件（例如鼠标移动、滚动、按钮按下和按钮释放）。

`#include "Event.h"`：包含了事件的基类 `Event`。这个类应该定义了所有事件的基础接口，所有具体的鼠标事件都继承自 `Event` 类。

------

**MouseMovedEvent**

```c++
class AERITH_API MouseMovedEvent : public Event
{
public:
    MouseMovedEvent(float x, float y)
        :m_MouseX(x), m_MouseY(y) {}
```

- `MouseMovedEvent` 继承自 `Event`，表示鼠标移动事件。
- 构造函数接受两个 `float` 类型的参数 `x` 和 `y`，表示鼠标在屏幕上的 X 和 Y 坐标，并初始化类的成员变量 `m_MouseX` 和 `m_MouseY`。

```c++
	inline float GetX() { return m_MouseX; }
    inline float GetY() { return m_MouseY; }
```

- `GetX()` 和 `GetY()` 是内联函数，返回鼠标的 X 和 Y 坐标。这些是 `MouseMovedEvent` 类的访问器函数。

```c++
std::string ToString()const override
    {
        std::stringstream ss;
        ss << "MouseMovedEvent: " << m_MouseX << " ," << m_MouseY;
        return ss.str();
    }
```

- `ToString()` 是重写的虚函数，用于将 `MouseMovedEvent` 对象转换为一个字符串格式，通常用于日志输出或调试信息。
- 使用 `std::stringstream` 将 `m_MouseX` 和 `m_MouseY` 格式化为字符串并返回。

```c++
EVENT_CLASS_TYPE(MouseMoved);
EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput);
private:
    float m_MouseX, m_MouseY;
};
```

- `EVENT_CLASS_TYPE(MouseMoved)`：宏定义，用于指定事件的类型是 `MouseMoved`，这样可以通过 `GetStaticType()` 获取事件类型。
- `EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)`：宏定义，表示该事件属于鼠标事件和输入事件类别。使用位运算将这些类别组合起来。
- `private`：`m_MouseX` 和 `m_MouseY` 是私有成员变量，存储鼠标的 X 和 Y 坐标。

------

**MouseScrolledEvent类**

```c++
class AERITH_API MouseScrolledEvent : public Event
{
public:
    MouseScrolledEvent(float xOffset, float yOffset)
        :m_XOffset(xOffset), m_YOffset(yOffset) {}
```

- `MouseScrolledEvent` 继承自 `Event`，表示鼠标滚轮事件。
- 构造函数接受两个 `float` 类型的参数，分别表示鼠标滚轮的 X 和 Y 偏移量（即滚动的距离）。

```
    inline float GetXOffset() const { return m_XOffset; }
    inline float GetYOffset() const { return m_YOffset; }
```

- `GetXOffset()` 和 `GetYOffset()` 返回滚轮的 X 和 Y 偏移量。

```c++
    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
        return ss.str();
    }
```

- `ToString()` 函数重写，用于将滚动事件的信息转换为字符串，格式化为 `"MouseScrolledEvent: xOffset, yOffset"`。

```c++
    EVENT_CLASS_TYPE(MouseScrolled);
    EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)
private:
    float m_XOffset, m_YOffset;
};
```

- `EVENT_CLASS_TYPE(MouseScrolled)`：为事件指定类型为 `MouseScrolled`。
- `EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)`：设置事件为输入事件和鼠标事件。
- `private`：`m_XOffset` 和 `m_YOffset` 是私有成员，存储鼠标滚轮的偏移量。

------

**`MouseButtonEvent` 类（基类）**

```c++
class AERITH_API MouseButtonEvent : public Event
{
public:
    inline int GetMouseButton() const { return m_Button; }
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton);
protected:
    MouseButtonEvent(int button)
        :m_Button(button) {}
    int m_Button;
};
```

- `MouseButtonEvent` 是一个基类，表示鼠标按钮相关的事件（按下和释放事件）。
- `GetMouseButton()` 是一个访问器函数，用来返回鼠标按钮的编号。
- `EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)`：这表明该事件属于鼠标、输入和鼠标按钮事件类别。
- **`protected` 构造函数**：构造函数是 `protected`，意味着该类不能被直接实例化，但可以由它的派生类实例化。`MouseButtonEvent` 作为基类，不能直接创建，但它的派生类 `MouseButtonPressedEvent` 和 `MouseButtonReleasedEvent` 可以调用该构造函数。这样设计的目的是为了防止直接创建 `MouseButtonEvent` 对象，但允许子类（如 `MouseButtonPressedEvent` 和 `MouseButtonReleasedEvent`）继承并使用构造函数。

------

**MouseButtonPressedEvent 类**

```c++
class AERITH_API MouseButtonPressedEvent : public MouseButtonEvent
{
public:
    MouseButtonPressedEvent(int button) :
        MouseButtonEvent(button) {}
```

- `MouseButtonPressedEvent` 继承自 `MouseButtonEvent`，表示鼠标按钮按下事件。
- 构造函数传递 `button` 参数给基类 `MouseButtonEvent` 的构造函数。

```c++
    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "MouseButtonPressedEvent: " << m_Button;
        return ss.str();
    }
    EVENT_CLASS_TYPE(MouseButtonPressed);
};
```

- `ToString()` 重写，将 `MouseButtonPressedEvent` 的信息格式化为字符串，显示按下的按钮。
- `EVENT_CLASS_TYPE(MouseButtonPressed)` 为事件指定类型 `MouseButtonPressed`。

------

**MouseButtonReleasedEvent类**

```c++
class AERITH_API MouseButtonReleasedEvent : public MouseButtonEvent
{
public:
    MouseButtonReleasedEvent(int button)
        :MouseButtonEvent(button) {}
```

- `MouseButtonReleasedEvent` 继承自 `MouseButtonEvent`，表示鼠标按钮释放事件。
- 构造函数与 `MouseButtonPressedEvent` 相同，传递 `button` 参数给基类的构造函数。

```c++
    std::string ToString() const override 
    {
        std::stringstream ss;
        ss << "MouseButtonReleasedEvent: " << m_Button;
        return ss.str();
    }
    EVENT_CLASS_TYPE(MouseButtonReleased);
};
```

- `ToString()` 重写，将 `MouseButtonReleasedEvent` 的信息格式化为字符串，显示释放的按钮。
- `EVENT_CLASS_TYPE(MouseButtonReleased)` 为事件指定类型 `MouseButtonReleased`。

------

**为什么构造函数是 `protected`**

- **`protected` 构造函数**：在 `MouseButtonEvent` 类中，构造函数被设置为 `protected`，这是因为我们不希望直接创建 `MouseButtonEvent` 的对象。该类只是一个基类，它的构造函数只能被继承它的派生类（如 `MouseButtonPressedEvent` 和 `MouseButtonReleasedEvent`）使用。这样设计是为了确保 `MouseButtonEvent` 本身不能被直接实例化，而只有在特定事件发生时（按下或释放按钮），通过派生类才能构建事件对象。这种设计可以避免不必要的基类对象实例化，同时让子类更具灵活性和可控性。

## ApplicationEvent.h

类似MouseEvent

## KeyEvent.h

类似MouseEvent

# 预编译头文件

# 抽象窗口类

为每个平台创建一个单独的窗口类。

### `Window.h`

```c++
namespace Aerith
{
    struct WindowProps
    {
        std::string Title;
        unsigned int Width;
        unsigned int Height;

        WindowProps(const std::string& title = "Aerith Engine",
            unsigned int width = 1280,
            unsigned int height = 720)
            : Title(title), Width(width), Height(height)
        {
        }
    };
```

- `WindowProps`：定义了一个结构体来封装窗口的属性。包含窗口的标题、宽度和高度，构造函数允许初始化默认值。

`Window` 类是一个接口类，定义了窗口系统的基本功能，包括：

- `OnUpdate`：窗口的更新操作。
- `GetWidth`、`GetHeight`：获取窗口的宽高。
- `SetEventCallback`：设置事件回调。
- `SetVSync` 和 `IsVSync`：设置和检查V-Sync（垂直同步）是否启用。
- `Create`：一个静态工厂函数，根据`WindowProps`创建窗口实例，由子类，比如WindowsWindow，MacWindow去实现

```c++
class AERITH_API Window
{
public:
	using EventCallbackFn = std::function<void(Event&)>;

	virtual ~Window() {}

	virtual void OnUpdate() = 0;

	virtual unsigned int GetWidth() const = 0;
	virtual unsigned int GetHeight() const = 0;

	virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
	virtual void SetVSync(bool enabled) = 0;
	virtual bool IsVSync() const = 0;

	static Window* Create(const WindowProps& Props = WindowProps());

};
```

### 第二个文件：`WindowsWindow.h`

```c++
#pragma once
#include "Aerith/Window.h"
#include <GLFW/glfw3.h>
```

- 引入`Window`接口和GLFW库。

```c++
namespace Aerith
{
    class WindowsWindow : public Window
    {
    public:
        WindowsWindow(const WindowProps& props);
        virtual ~WindowsWindow();

        void OnUpdate() override;
        inline unsigned int GetWidth() const override { return m_Data.Width; }
        inline unsigned int GetHeight() const override { return m_Data.Height; }
        inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; };
        void SetVSync(bool enabled);
        bool IsVSync() const;

    private:
        virtual void Init(const WindowProps& props);
        virtual void Shutdown();
    private:
        GLFWwindow* m_Window;
        struct WindowData
        {
            std::string Title;
            unsigned int Width, Height;
            bool VSync;
            EventCallbackFn EventCallback;
        };
        WindowData m_Data;
    };
}
```

- `WindowsWindow` 类继承自`Window`，具体实现了该接口，表示GLFW平台下的窗口管理。
- 成员函数：
  - `OnUpdate`：更新窗口，处理事件。
  - `GetWidth`、`GetHeight`：返回窗口的宽高。
  - `SetEventCallback`：设置事件回调。
  - `SetVSync`、`IsVSync`：设置和检查V-Sync的状态。
  - `Init`：初始化窗口。
  - `Shutdown`：销毁窗口。

### 第三个文件：`WindowsWindow.cpp`

```c++
#include "AerithPch.h"
#include "WindowsWindow.h"
```

- 包含了预编译头文件和Windows窗口实现文件。

```c++
	static bool s_GLFWInitialized = false;
```

- `s_GLFWInitialized` 是一个静态变量，用于确保GLFW库只初始化一次。

```c++
    Window* Window::Create(const WindowProps& props)
    {
        return new WindowsWindow(props);
    }
```

- `Window::Create` 工厂函数，返回一个`WindowsWindow`实例。

```c++
    WindowsWindow::WindowsWindow(const WindowProps& props)
    {
        Init(props);
    }
```

- `WindowsWindow` 构造函数，调用`Init`初始化窗口。

```c++
    WindowsWindow::~WindowsWindow()
    {
        Shutdown();
    }
```

- 析构函数，调用`Shutdown`销毁窗口。

```c++
    void WindowsWindow::Init(const WindowProps& props)
    {
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;

        AERITH_CORE_INFO("Creating window {0} ({1},{2})", props.Title, props.Width, props.Height);
```

- `Init`方法初始化窗口的数据成员，并输出窗口创建的信息。

```c++
        if (!s_GLFWInitialized)
        {
            int success = glfwInit();
            AERITH_CORE_ASSERT(success, "Could not initialize GLFW!");

            s_GLFWInitialized = true;
        }


#ifdef AERITH_ENABLE_ASSERTS	
	#define AERITH_ASSERT(x, ...) {if(!(x)){AERITH_ERROR("Assertion Failed: {0}", __VA_ARGS__);__debugbreak();}}
	#define AERITH_CORE_ASSERT(x, ...){if(!(x)){AERITH_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__);__debugbreak();}}
#else
	#define AERITH_ASSERT(x, ...)
	#define AERITH_CORE_ASSERT(x, ...)
#endif // 
```

- 如果GLFW还未初始化，调用`glfwInit`初始化GLFW，并设置`GLFWInitialized`标志。

```c++
        m_Window = glfwCreateWindow((int)m_Data.Width, (int)m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(m_Window);
        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);
    }
```

- 创建GLFW窗口，设置窗口的上下文（即OpenGL上下文），并将窗口数据结构绑定到窗口用户指针上，最后开启V-Sync。

```c++
    void WindowsWindow::Shutdown()
    {
        glfwDestroyWindow(m_Window);
    }
```

- 销毁GLFW窗口。

```c++
    void WindowsWindow::OnUpdate()
    {
        glfwPollEvents();
        glfwSwapBuffers(m_Window);
    }
```

- `OnUpdate`：调用`glfwPollEvents`处理所有待处理的事件（例如输入事件），然后交换缓冲区，更新窗口内容。

```c++
    void WindowsWindow::SetVSync(bool enabled)
    {
        if (enabled)
            glfwSwapInterval(1); // 开启V-Sync
        else
            glfwSwapInterval(0); // 关闭V-Sync

        m_Data.VSync = enabled;
    }
```

- 设置V-Sync的状态，调用`glfwSwapInterval`来控制垂直同步。

```c++
    bool WindowsWindow::IsVSync() const
    {
        return m_Data.VSync;
    }
}
```

- 返回当前V-Sync的状态。

# 设置窗口事件

## EventCallbackFn 

`using EventCallbackFn = std::function<void(Event&)>;` 

### 1. **`std::function` 的概念**：

`std::function` 是 C++ 标准库提供的一个模板类，用来封装任何可调用对象（如普通函数、成员函数、函数指针、Lambda 表达式等）。它提供了一种通用的方式来处理函数对象，使得你可以通过统一的接口来调用这些不同类型的可调用对象。

### 2. **`using` 关键字**：

`using` 是 C++11 引入的一个别名声明，它用于为类型定义一个新的名字。你可以把 `using` 看作是 `typedef` 的一个更现代的替代方式。

```c++
using EventCallbackFn = std::function<void(Event&)>;
```

这行代码的意思是，定义了一个新的类型别名 `EventCallbackFn`，它实际上是 `std::function<void(Event&)>` 的别名。

### 3. **`std::function<void(Event&)>`**：

`std::function<void(Event&)>` 表示一个函数对象，它是一个接受 `Event&` 类型的参数并且没有返回值（`void`）的函数。

具体来说：

- `std::function<void(Event&)>` 可以封装任何接受 `Event&` 参数并返回 `void` 的可调用对象。
- 这个可调用对象可以是普通函数、成员函数、Lambda 表达式或者函数指针。

### 4. **如何使用 `EventCallbackFn`**：

`EventCallbackFn` 定义后，你可以像使用任何其他类型一样使用它。它可以用于存储函数、Lambda 表达式或其他可调用对象，目的是用它来存储一个函数或回调，稍后在事件触发时调用。

#### 示例 1: 使用普通函数

假设你有一个普通的事件处理函数：

```c++
void HandleEvent(Event& e) {
    std::cout << "Handling event: " << e.GetName() << std::endl;
}
```

你可以将 `HandleEvent` 函数绑定到 `EventCallbackFn` 上：

```c++
EventCallbackFn callback = HandleEvent;
```

当你想调用这个回调时，只需要：

```c++
Event event;
callback(event);  // 调用 HandleEvent(event)
```

#### 示例 2: 使用 Lambda 表达式

你也可以将 Lambda 表达式赋值给 `EventCallbackFn`：

```c++
EventCallbackFn callback = [](Event& e) {
    std::cout << "Lambda handling event: " << e.GetName() << std::endl;
};
```

然后像上面一样调用回调：

```c++
Event event;
callback(event);  // 调用 Lambda 处理事件
```

#### 示例 3: 使用成员函数

你还可以用成员函数来处理事件。假设你有一个类 `Application`，它有一个成员函数 `OnEvent`：

```c++
class Application {
public:
    void OnEvent(Event& e) {
        std::cout << "Application handling event: " << e.GetName() << std::endl;
    }
};
```

你可以使用 `std::bind` 来绑定成员函数：

```
Application app;
EventCallbackFn callback = std::bind(&Application::OnEvent, &app, std::placeholders::_1);
```

然后你可以通过 `callback` 调用 `OnEvent`：

```
Event event;
callback(event);  // 调用 Application::OnEvent(event)
```

- `using EventCallbackFn = std::function<void(Event&)>;` 定义了一个新的类型别名，表示一个接收 `Event&` 参数并返回 `void` 的回调函数类型。
- 你可以将任何符合这个签名的可调用对象（如函数、成员函数、Lambda）赋值给 `EventCallbackFn` 类型的变量。
- 当事件发生时，你可以通过这个回调函数对象来执行相应的事件处理逻辑。

通过 `std::function` 和 `using`，你可以方便地实现高灵活性的回调机制，并使得代码更加模块化和可扩展。

## 设置回调函数

我们可以在窗口的`Init`里面重写各种回调函数

以`SetWindowSizeCallback`为例

这是GLFW官方提供的API，我们基本上是为了覆写对应的函数，可以通过lambda函数来编写

```c++
GLFWAPI GLFWwindowsizefun glfwSetWindowSizeCallback(GLFWwindow* handle,
                                                    GLFWwindowsizefun cbfun)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);

    _GLFW_REQUIRE_INIT_OR_RETURN(NULL);
    _GLFW_SWAP(GLFWwindowsizefun, window->callbacks.size, cbfun);
    return cbfun;
}
```

我们需要提供一个GLFWwindowsizefun，其形式如下：

```c++
typedef void (* GLFWwindowsizefun)(GLFWwindow* window, int width, int height);
```

示例代码演示了如何使用GLFW提供的`SetWindowSizeCallback`来处理窗口大小变化事件。在GLFW中，窗口相关的事件（如大小变化、键盘输入等）通常通过回调函数来处理，回调函数允许你在事件发生时执行自定义的逻辑。

### 1. **定义回调函数类型**

```c++
typedef void (* GLFWwindowsizefun)(GLFWwindow* window, int width, int height);
```

这行代码定义了一个回调函数的类型`GLFWwindowsizefun`，它接收三个参数：

- `GLFWwindow* window`：窗口对象的指针。
- `int width`：窗口的新宽度。
- `int height`：窗口的新高度。

### 2. **设置窗口大小回调**

通过调用`glfwSetWindowSizeCallback`，你可以为窗口注册一个回调函数，这样在窗口大小发生变化时，GLFW会自动调用这个回调函数。

```c++
glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
{
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
    data.Width = width;
    data.Height = height;

    WindowResizeEvent event(width, height);
    data.EventCallback(event);
});
```

这里用lambda表达式定义了回调函数。当窗口大小变化时，会触发这个lambda函数。

- `window`：表示当前窗口。
- `width` 和 `height`：表示新的窗口大小。

### 3. **存储并更新窗口尺寸**

```c++
WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
data.Width = width;
data.Height = height;
```

`glfwGetWindowUserPointer`用于获取与窗口相关联的用户数据。`WindowData`是用户自定义的结构体，它保存了窗口的宽度和高度等信息。通过`data.Width`和`data.Height`更新窗口的新尺寸。

### 4. **触发事件回调**

```
WindowResizeEvent event(width, height);
data.EventCallback(event);
```

在窗口尺寸改变时，创建一个`WindowResizeEvent`事件，并通过`EventCallback`触发事件，通知系统或应用处理窗口大小变化的逻辑。

`WindowsWindow::OnUpdate()`方法通过调用 `glfwPollEvents()` 来处理所有的事件，包括用户的输入、窗口事件等。这是一个常见的做法，用于确保事件回调能够持续被触发并被处理。

```c++
void WindowsWindow::OnUpdate()
{
    glfwPollEvents();    // 轮询所有事件
    glfwSwapBuffers(m_Window);  // 交换缓冲区以显示渲染结果
}
```

`glfwPollEvents()`：

- 这个函数会触发所有待处理的事件回调。它会遍历事件队列，处理诸如键盘输入、鼠标输入、窗口大小调整、窗口关闭等事件。
- 在调用了`glfwPollEvents()`之后，所有注册的事件回调（比如`SetWindowSizeCallback`、`SetKeyCallback`等）会被触发。这时，像你在`Application::OnEvent`中定义的事件回调（比如窗口关闭事件）也会被调用。

`glfwSwapBuffers(m_Window)`：

- 这个函数负责交换前后缓冲区，将渲染结果显示到窗口上。一般来说，`glfwPollEvents()`和`glfwSwapBuffers()`是渲染循环中的两个重要步骤，前者处理事件，后者更新显示。

## 整个回调机制的流程

实际上，调用 `data.EventCallback(event)` 就是执行了 `EventCallbackFn` 类型的函数指针（`using EventCallbackFn = std::function<void(Event&)>`），而这个指针指向的是通过 `m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent))` 设置的回调函数。

### 1. **`EventCallbackFn` 是一个函数类型别名：**

你定义了 `EventCallbackFn` 类型别名，它是一个可以接收 `Event&` 作为参数并返回 `void` 的函数类型：

```c++
using EventCallbackFn = std::function<void(Event&)>;
```

### 2. **`SetEventCallback` 存储回调函数：**

`WindowsWindow` 中的 `SetEventCallback` 函数接收一个 `EventCallbackFn` 类型的回调函数并将其保存在 `m_Data.EventCallback` 中：

```c++
void WindowsWindow::SetEventCallback(const EventCallbackFn& callback) {
    m_Data.EventCallback = callback;
}
```

### 3. **`BIND_EVENT_FN(OnEvent)` 生成回调函数：**

在 `Application` 构造函数中，调用了 `SetEventCallback(BIND_EVENT_FN(OnEvent))`。

这实际上是通过 `BIND_EVENT_FN` 宏将 `Application::OnEvent` 方法绑定到回调函数上，并返回一个 `std::function<void(Event&)>` 类型的函数对象：

```c++
m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
```

- `BIND_EVENT_FN(OnEvent)` 实际上会创建一个 `std::bind` 返回的函数对象，这个对象会调用 `Application::OnEvent`，并自动传入事件对象 `Event&`。
- 这就意味着，当事件发生时，`m_Data.EventCallback` 会调用 `OnEvent`。

### 4. **`data.EventCallback(event)` 调用回调：**

当 GLFW 捕获到事件并触发回调时（比如窗口大小改变、键盘输入等），在 `WindowsWindow` 类的回调函数里，事件对象 `event` 会被传递给 `m_Data.EventCallback`：

```c++
data.EventCallback(event);
```

- 这里的 `data.EventCallback` 就是之前通过 `SetEventCallback` 设置的回调函数，它指向了 `OnEvent`。
- 因此，调用 `data.EventCallback(event)` 实际上等于调用 `Application::OnEvent(event)`。

### 5. **回调的执行：**

在 `Application::OnEvent` 中，事件被捕获后，通过 `EventDispatcher` 被进一步分发到具体的事件处理函数（例如 `OnWindowClose`）：

```c++
void Application::OnEvent(Event& e) {
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
    AERITH_CORE_TRACE("{0}", e);
}
```

### 总结：

1. **`EventCallbackFn` 是函数类型：** 它是一个函数指针类型，指向一个接受 `Event&` 参数并返回 `void` 的函数。
2. **回调存储：** `SetEventCallback` 将这个回调函数存储到 `m_Data.EventCallback` 中。
3. **回调函数绑定：** `BIND_EVENT_FN(OnEvent)` 通过 `std::bind` 将 `Application::OnEvent` 方法与 `this` 绑定，并返回一个函数对象，存储在 `m_Data.EventCallback`。
4. **回调执行：** 当事件发生时，`data.EventCallback(event)` 被调用，实际调用的是 `OnEvent(event)`。

## 另外的回调函数(编写方式同理)

```c++
glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
{
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
    WindowCloseEvent event;
    data.EventCallback(event);
});
glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) 
{
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
    switch (action)
    {
        case GLFW_PRESS:
        {
            KeyPressedEvent event(key, 0);
            data.EventCallback(event);
            break;
        }
        case GLFW_RELEASE:
        {
            KeyReleasedEvent event(key);
            data.EventCallback(event);
            break;
        }
        case GLFW_REPEAT:
        {
            KeyPressedEvent event(key, 1);
            data.EventCallback(event);
            break;
        }
    }
});
glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
{
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
    switch (action)
    {
        case GLFW_PRESS:
        {
            MouseButtonPressedEvent event(button);
            data.EventCallback(event);
            break; 
        }
        case GLFW_RELEASE:
        {
            MouseButtonReleasedEvent event(button);
            data.EventCallback(event);
            break;
        }
    }
});
glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset)
{
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
    MouseScrolledEvent event((float)xoffset, (float)yoffset);
    data.EventCallback(event);
});
glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos) 
{
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
    MouseMovedEvent event((float)xpos, (float)ypos);
    data.EventCallback(event);
});
```

# Layer

基本上可以看作是一个有序的图层列表或者图层堆栈，当你想要画东西的时候，Layer决定了事物的绘制顺序。在游戏引擎中，`Layer` 是一种用于组织不同功能模块的结构。它通常用于将游戏的不同部分分离成不同的层次（例如输入层、渲染层、UI层等），每一层都可以独立处理相关的任务，比如：

- **渲染层**：负责绘制游戏画面。
- **事件层**：处理输入事件或其他游戏事件。
- **UI层**：处理用户界面及其交互。

**事件处理顺序**：通常是从栈的后面（最上层）向前（最底层）进行的。原因是，事件通常应该先由最上面的 `Layer` 来处理，因为这层可能是UI层或弹出的窗口，它们通常是最需要响应事件的。事件处理从上层开始，可以避免底层的层捕获不该处理的事件。例如，如果用户点击了一个按钮，UI层应先接收到事件并处理，而不是底层的渲染层或游戏逻辑层。

**渲染顺序**：渲染顺序通常是从栈的前面（底层）向后（最上层）。这也符合常规的图形渲染的逻辑。最底层的内容会先被渲染，随后上面的层覆盖下层内容。通常，最上层是UI层或一些动态元素，它们需要在最后渲染，以确保它们能显示在其他所有元素的上面。

`Layer` 本身通常是一个基类，提供了一些基础功能，如事件响应和更新处理等。在具体实现中，可以继承 `Layer` 类并重写其方法来实现不同的功能。

## `Layer` 类

`Layer` 类提供了以下几个方法：

- `OnAttach()`：当 `Layer` 被添加到 `LayerStack` 时调用。
- `OnDetach()`：当 `Layer` 被移除时调用。
- `OnUpdate()`：每一帧的更新逻辑。
- `OnEvent(Event& event)`：处理事件（如键盘输入、鼠标点击等）。

此外，`Layer` 类还保存了一个名字 `m_DebugName`，用于调试目的。

```c++
#pragma once

#include "Aerith/Core.h"
#include "Aerith/Events/Event.h"

namespace Aerith
{
	class AERITH_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(Event& event) {}
	
		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};
}
```

## `LayerStack` 类

`LayerStack` 是一个层次结构容器，存储了多个 `Layer` 对象，提供了管理这些 `Layer` 的功能：

- `PushLayer(Layer* layer)`：将一个普通的 `Layer` 推入栈中。
- `PushOverlay(Layer* overlay)`：将一个叠加层（Overlay）推入栈中，通常用于UI层等总是需要放在最上层的层。
- `PopLayer(Layer* layer)`：移除栈中的普通 `Layer`。
- `PopOverlay(Layer* overlay)`：移除栈中的叠加层。

`LayerStack` 使用一个 `std::vector<Layer*>` 来存储所有的层，并且用一个迭代器 `m_LayerInsert` 来标记插入点。

```c++
#pragma once
#include "Aerith/Core.h"
#include "Layer.h"
#include <vector>

namespace Aerith
{
	class AERITH_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }

	private:
		std::vector<Layer*> m_Layers;
		std::vector<Layer*>::iterator m_LayerInsert;
	};
}
#include "AerithPch.h"
#include "LayerStack.h"

namespace Aerith
{
	LayerStack::LayerStack()
	{
		m_LayerInsert = m_Layers.begin();
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Layers)
		{
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_LayerInsert = m_Layers.emplace(m_LayerInsert, layer);
	}

	// Overlay Always On the top.
	void LayerStack::PushOverlay(Layer* overlay)
	{
		m_Layers.emplace_back(overlay);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it != m_Layers.end())
		{
			m_Layers.erase(it);
			m_LayerInsert--;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
		if (it != m_Layers.end())
			m_Layers.erase(it);
	}
}

```

## Overlay和Layer

**普通的 `Layer` 插入的位置永远是在叠加层之前**。这就是为什么我们需要区分 **普通层** 和 **叠加层** 的原因之一，也是为什么我们插入普通Layer的时候需要插入到`m_LayerInsert`而不是直接在末尾，所以即使你插入了一个普通的Layer，它也不会在Overlay的上层。

叠加层总是保持在最上面，不会被其他层覆盖，这对于UI界面、弹窗、通知等非常重要。

# Glad

安装`Glad`，`version 4.6`

```c++
project "Glad"
	kind "StaticLib"
	language "C"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"include//glad/glad.h",
		"include//KHR/khrplatform.h",
		"src/glad.c"
	}

	includedirs
	{
		"include"
	}
	filter "system:windows"
		systemversion "latest"
		staticruntime "On" --staticly linking the runtime libraries


	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		buildoptions "/MT"
		optimize "on"
```

```c++
workspace "Aerith"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	IncludeDir["Glad"] = "Aerith/vendor/Glad/include"
	include "Aerith/vendor/Glad"

		includedirs
		{
			"%{IncludeDir.Glad}"
		}

		links
		{
			"Glad",
		}
```

# Imgui

### ImGui简介

<font color='A2CD5A'>**ImGui**</font>（Immediate Mode GUI）是一个用于C++应用程序的即时模式图形用户界面（GUI）库，通常用于游戏引擎、调试工具和可视化软件。它由**Omar Cornut**开发，<font color='87CEFA'>**主要用于快速构建开发者工具，而不是用于最终用户界面。**</font>

------

## ImGui的特点

1. **即时模式（Immediate Mode）**
   - 与传统的**保留模式（Retained Mode）**GUI不同，ImGui不会存储UI组件的状态，而是<font color='87CEFA'>**每帧重新绘制界面**</font>。
   - 这种模式使得**UI更新更直观**，无需管理复杂的UI状态。
2. **轻量级和高效**
   - ImGui的核心库非常小，不依赖任何外部库。
   - 由于每帧绘制，省去了维护UI状态的开销，适用于**嵌入式调试工具**。
3. **跨平台**
   - 只需绑定到不同的渲染后端（如OpenGL、Vulkan、DirectX），ImGui即可在**Windows、Linux、MacOS**等平台运行。
4. **易于集成**
   - 可以很容易地嵌入现有的应用程序或游戏引擎中。
   - 官方提供了与**SDL、GLFW、OpenGL、Vulkan、DirectX**等的绑定示例。
5. **适用于调试和开发工具**
   - 由于其即时模式的特性，它特别适合**游戏开发**中的调试界面、性能监视器、关卡编辑器等。

------

## **ImGui的核心概念**

- **窗口（Window）**：一个可拖动、可调整大小的UI容器。
- **控件（Widgets）**：按钮、滑动条、输入框等组件。
- **状态（State）**：每帧重新绘制，不存储控件状态，需要程序手动管理。

# Imgui Event

```c++
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
```

# Input Polling

