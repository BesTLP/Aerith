#pragma once

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