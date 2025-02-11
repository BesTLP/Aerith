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

