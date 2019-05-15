

#include "mheaders/MsvCompiler.h"

MSV_DISABLE_ALL_WARNINGS

#include <mutex>

MSV_ENABLE_WARNINGS


std::recursive_mutex g_lock;
int32_t g_value = 0;


#ifdef _WIN32
int32_t Increment()
#else
extern "C" int32_t Increment()
#endif // _WIN32
{
	std::lock_guard<std::recursive_mutex> lock(g_lock);

	++g_value;

	return g_value;
}

#ifdef _WIN32
int32_t Decrement()
#else
extern "C" int32_t Decrement()
#endif // _WIN32
{
	std::lock_guard<std::recursive_mutex> lock(g_lock);

	--g_value;

	return g_value;
}

#ifdef _WIN32
int32_t GetValue()
#else
extern "C" int32_t Increment()
#endif // _WIN32
{
	std::lock_guard<std::recursive_mutex> lock(g_lock);

	return g_value;
}
