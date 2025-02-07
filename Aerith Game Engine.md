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

✅ **`#ifdef BUILD_DLL` 宏**

- **编译 DLL 时**，定义 `BUILD_DLL`，`DLL_API` 变成 `_declspec(dllexport)`。
- **使用 DLL 时**，没有 `BUILD_DLL`，`DLL_API` 变成 `_declspec(dllimport)`。

这样，我们可以使用**同一个头文件**，在不同的编译环境下自动切换导入/导出。

```c++
#pragma once

#ifndef BUILD_DLL
	#define DLL_API _declspec(dllexport)
#else
	#define DLL_API _declspec(dllimport)
#endif // !BUILD_DLL

namespace Aerith
{
	_declspec(dllexport) void Print();
}


```

使用

```c++
namespace Aerith
{
	_declspec(dllimport) void Print();
}
void main()
{
	Aerith::Print();
}
```

