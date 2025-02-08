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