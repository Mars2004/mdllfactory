# MarsTech DLL Factory
 - [Installation](#installation)
	 - [Dependencies](#dependencies)
	 - [Configuration](#configuration)
 - [DLL Factory](#dll-factory)
 - [DLLs Compatible With DLL Factory](#dlls-compatible-with-dll-factory)
	 - [DLLs With Exported GetDllObject Function](#dlls-with-exported-getdllobject-function)
	 - [Decorator For DLLs Without GetDllObject Function](#decorator-for-dlls-without-getdllobject-function)
 - [Usage Example](#usage-example)
 - [Source Code Documentation](#source-code-documentation)
 - [License](#license)

MarsTech DLL factory manages dynamic/shared libraries and provides interface to get DLL objects from loaded dynamic/shared libraries.
It loads dynamic/shared libraries on demand and each library is loaded only once per executable binary. It unloads dynamic/shared libraries on demand or when the DLL factory is destroyed.

## Installation
Download all [MarsTech](https://github.com/Mars2004) dependencies and put them to a same directory. Create a new subdirectory "3rdParty" and put there "3rdParty" dependencies ([spdlog](https://github.com/gabime/spdlog)).

### Dependencies
 - [MarsTech Headers](https://github.com/Mars2004/mheaders)
 - [MarsTech Dependency Injection](https://github.com/Mars2004/mdi)
 - [MarsTech Error Handling](https://github.com/Mars2004/merror)
 - [MarsTech Logging](https://github.com/Mars2004/mlogging)
 - [spdlog](https://github.com/gabime/spdlog)

### Configuration
No build configuration is needed - just build whole solution.

## DLL Factory
DLL Factory is static library which should be linked to executable binary only (do not link it to dynamic/shared libraries). Only one instance should be created per one executable binary and should be injected to all objects which wants to load dynamic/shared libraries and theirs objects.

**Example:**
~~~cpp
#include "mdllfactory/MsvDllFactory.h"
#include "mdllfactory/MsvDllList.h"
#include "msys/msys/MsvSysDll_Interface.h"

std::shared_ptr<MsvLogger> spLogger;

//create and initialize DLL list
std::shared_ptr<IMsvDllList> spDllList(new MsvDllList());
MSV_RETURN_FAILED(spDllList->AddDll(MSV_SYS_OBJECT_ID_LAST, "msys.dll"));

//create DLL Factory instance
std::shared_ptr<IMsvDllFactory> spDllFactory(new MsvDllFactory(spDllList, spLogger));

//get MarsTech C++ SYS Library interface
std::shared_ptr<IMsvSys_Last> spSys;
if (MSV_FAILED(errorCode = spDllFactory->GetDllObject(MSV_SYS_OBJECT_ID_LAST, spSys)))
{
	MSV_LOG_ERROR(spLogger, "Get SYS DLL object failed with error: {0:x}", errorCode);
	return errorCode;
}
~~~

## DLLs Compatible With DLL Factory
There are two options how to create/get DLLs and theirs objects:

 - DLLs with exported GetDllObject function
 - Decorator for DLLs without GetDllObject function

### DLLs With Exported GetDllObject Function
They are DLLs written for MarsTech DLL Factory and can be loaded by DLL Factory without any wrappers/adapters.

**Example:**
~~~cpp
//this example is copied from MarsTech C++ SYS DLL implementation (https://github.com/Mars2004/msys/blob/master/msys/main.cpp)

#include "MsvSysDll_Interface.h"
#include "msys/msys_lib/MsvSys.h"
#include "mdllfactory/MsvDllMainHelper.h"

#include "merror/MsvErrorCodes.h"

std::recursive_mutex g_lock;
//IMsvSys interface inherits from IMsvDllObject
std::weak_ptr<IMsvDllObject> g_spSys;

//GetDllObject implementation (Windows/Linux support)
#ifdef _WIN32
MsvErrorCode GetDllObject(const char* id, std::shared_ptr<IMsvDllObject>& spDllObject)
#else
extern "C" MsvErrorCode GetDllObject(const char* id, std::shared_ptr<IMsvDllObject>& spDllObject)
#endif // _WIN32
{
	std::lock_guard<std::recursive_mutex> lock(g_lock);

	//convert to string for comparison
	std::string idString(id);

	//create and return DLL object when known id was used
	MSV_GETWEAKSHARED_DLLOBJECT_WITH_ID(idString, MSV_SYS_OBJECT_ID, MsvSys(), g_spSys, spDllObject);

	//uknown id (not found) -> error
	return MSV_NOT_FOUND_ERROR;
}
~~~

### Decorator For DLLs Without GetDllObject Function
Decorator is usefull when you need to load DLL without exported GetDllObject function (3rd party DLLs, DLLs with C interface, etc.).

**Example:**
~~~cpp
//this example is copied from MarsTech C++ SYS DLL implementation (https://github.com/Mars2004/msys/blob/master/msys/main.cpp) and decorates testdll_2 (https://github.com/Mars2004/mdllfactory/blob/master/Test/testdll_2/main.cpp)

#include  "mdllfactory/IMsvDllDecorator.h"

class TestDll2:
	public IMsvDllDecorator
{
public:

	TestDll2():
		m_pIncrementFunction(nullptr),
		m_pDecrementFunction(nullptr),
		m_pGetValueFunction(nullptr)
	{
	}

	int32_t Increment()
	{
		return m_pIncrementFunction();
	}

	int32_t Decrement()
	{
		return m_pDecrementFunction();
	}

	int32_t GetValue()
	{
		return m_pGetValueFunction();
	}

protected:
	virtual MsvErrorCode DecorateDllObject(const char* id, std::shared_ptr<IMsvDllAdapter> spMsvDllAdapter) override
	{
		void* pDllAddress = nullptr;

		MSV_RETURN_FAILED(spMsvDllAdapter->GetDllAddress("Increment", pDllAddress));
		m_pIncrementFunction = reinterpret_cast<int32_t(*)()>(pDllAddress);

		MSV_RETURN_FAILED(spMsvDllAdapter->GetDllAddress("Decrement", pDllAddress));
		m_pDecrementFunction = reinterpret_cast<int32_t(*)()>(pDllAddress);

		MSV_RETURN_FAILED(spMsvDllAdapter->GetDllAddress("GetValue", pDllAddress));
		m_pGetValueFunction = reinterpret_cast<int32_t(*)()>(pDllAddress);

		return MSV_SUCCESS;
	}

protected:
	int32_t(*m_pIncrementFunction)();
	int32_t(*m_pDecrementFunction)();
	int32_t(*m_pGetValueFunction)();
};
~~~

## Usage Example
There is also an [usage example](https://github.com/Mars2004/msys/tree/master/Example) which uses the most of [MarsTech](https://github.com/Mars2004) projects and libraries.
Its source codes and readme can be found at:
 - [https://github.com/Mars2004/msys/tree/master/Example](https://github.com/Mars2004/msys/tree/master/Example)

## Source Code Documentation
You can find generated source code documentation at [https://www.marstech.cz/projects/mdllfactory/1.0.1/doc](https://www.marstech.cz/projects/mdllfactory/1.0.1/doc).

## License
This project is released under GNU General Public License version 3. If you can not or do not want to accept GNU GPLv3 license and you would like to use this project under another license, please contact me on [info@marstech.cz](mailto:info@marstech.cz) or visit [www.marstech.cz](https://www.marstech.cz/en).
