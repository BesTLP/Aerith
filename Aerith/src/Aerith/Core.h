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

#ifdef AERITH_DEBUG
	#define AERITH_ENABLE_ASSERTS
#endif // AERITH_DEBUG


#ifdef AERITH_ENABLE_ASSERTS	
	#define AERITH_ASSERT(x, ...) {if(!(x)){AERITH_ERROR("Assertion Failed: {0}", __VA_ARGS__);__debugbreak();}}
	#define AERITH_CORE_ASSERT(x, ...){if(!(x)){AERITH_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__);__debugbreak();}}
#else
	#define AERITH_ASSERT(x, ...)
	#define AERITH_CORE_ASSERT(x, ...)
#endif // 

#define BIT(x) (1 << x)

#define AERITH_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
