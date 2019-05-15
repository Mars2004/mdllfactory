

#include "mdllfactory/IMsvDllObject.h"

#include "merror/MsvErrorCodes.h"

MSV_DISABLE_ALL_WARNINGS

#include <mutex>
#include <memory>
#include <string>

MSV_ENABLE_WARNINGS




class Test1Object:
	public IMsvDllObject
{
public:
	Test1Object() {};
	virtual ~Test1Object() {};

};


std::recursive_mutex g_lock;
std::shared_ptr<Test1Object> g_spTest1Object;


#ifdef _WIN32
MsvErrorCode GetDllObject(const char* id, std::shared_ptr<IMsvDllObject>& spDllObject)
#else
extern "C" MsvErrorCode GetDllObject(const char* id, std::shared_ptr<IMsvDllObject>& spDllObject)
#endif // _WIN32
{
	std::lock_guard<std::recursive_mutex> lock(g_lock);

	std::string idString(id);

	if (idString.compare("{9F31D4A9-CDF5-49FA-90A8-AA109735DC9F}") == 0)
	{
		if (!g_spTest1Object)
		{
			g_spTest1Object.reset(new (std::nothrow) Test1Object());
			if (!g_spTest1Object)
			{
				return MSV_ALLOCATION_ERROR;
			}
		}

		spDllObject = g_spTest1Object;

		return MSV_SUCCESS;
	}
	else if (idString.compare("{337AB087-1B69-4561-A0E4-771723EFCBFE}") == 0)
	{
		if (!g_spTest1Object)
		{
			g_spTest1Object.reset(new (std::nothrow) Test1Object());
			if (!g_spTest1Object)
			{
				return MSV_ALLOCATION_ERROR;
			}
		}

		spDllObject = g_spTest1Object;

		return MSV_SUCCESS;
	}

	//not found -> error
	return MSV_NOT_FOUND_ERROR;
}