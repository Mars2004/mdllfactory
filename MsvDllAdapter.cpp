/**************************************************************************************************//**
* @addtogroup	MDLLFACTORY
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech Dll Adapter Implementation
* @details		Contains implementation @ref MsvDllAdapter.
* @author		Martin Svoboda
* @date			06.04.2019
* @copyright	GNU General Public License (GPLv3).
******************************************************************************************************/


/*
This file is part of MarsTech Dll Factory.

MarsTech Dependency Injection is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

MarsTech Promise Like Syntax is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar. If not, see <https://www.gnu.org/licenses/>.
*/


#include "MsvDllAdapter.h"

#include "merror/MsvErrorCodes.h"


/********************************************************************************************************************************
*															Constructors and destructors
********************************************************************************************************************************/


MsvDllAdapter::MsvDllAdapter(std::shared_ptr<MsvLogger> spLogger):
	m_pHandle(nullptr),
	m_spLogger(spLogger)
{
	
}

MsvDllAdapter::~MsvDllAdapter()
{
	UnloadDllLibrary();
}


/********************************************************************************************************************************
*															IMsvDllAdapter public methods
********************************************************************************************************************************/


MsvErrorCode MsvDllAdapter::GetDllAddress(const char* dllAddressName, void*& pdllAddress)
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	MSV_LOG_INFO(m_spLogger, "Loading DLL address \"{}\".", dllAddressName);

	pdllAddress = nullptr;

	if (!Loaded())
	{
		MSV_LOG_ERROR(m_spLogger, "The library has not been loaded (it must be loaded before calling GetProcAddress)!");
		return MSV_NOT_INITIALIZED_ERROR;
	}

#ifdef _WIN32
	FARPROC farProc = GetProcAddress(m_pHandle, dllAddressName);
	if (!farProc)
	{
		MSV_LOG_ERROR(m_spLogger, "GetProcAddress \"{}\" failed with error: {}", dllAddressName, GetLastError());
		return MSV_NOT_FOUND_ERROR;
	}
#else
	void* farProc = dlsym(m_pHandle, dllAddressName);
	if (!farProc)
	{
		MSV_LOG_ERROR(m_spLogger, "Load symbol \"{}\" failed with error: {}", dllAddressName, dlerror());
		return MSV_NOT_FOUND_ERROR;
	}
#endif //_WIN32

	pdllAddress = farProc;

	MSV_LOG_INFO(m_spLogger, "DLL address \"{}\" has been successfully loaded.", dllAddressName);

	return MSV_SUCCESS;
}

bool MsvDllAdapter::Loaded() const
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	return m_pHandle ? true : false;
}

MsvErrorCode MsvDllAdapter::LoadDllLibrary(const char* dllPath)
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	MSV_LOG_INFO(m_spLogger, "Loading DLL library \"{}\".", dllPath);

	if (Loaded())
	{
		MSV_LOG_INFO(m_spLogger, "DLL library \"{}\" has been already loaded.", dllPath);
		return MSV_ALREADY_INITIALIZED_INFO;
	}

#ifdef _WIN32
	m_pHandle = LoadLibrary(dllPath);
	if (!m_pHandle)
	{
		MSV_LOG_ERROR(m_spLogger, "Load DLL library \"{}\" failed with error: {}", dllPath, GetLastError());
		return MSV_OPEN_ERROR;
	}
#else
	m_pHandle = dlopen(dllPath, RTLD_LAZY);
	if (!m_pHandle)
	{
		MSV_LOG_ERROR(m_spLogger, "Load DLL library \"{}\" failed with error: {}", dllPath, dlerror());
		return MSV_OPEN_ERROR;
	}
#endif //_WIN32

	MSV_LOG_INFO(m_spLogger, "DLL library \"{}\" has been successfully loaded.", dllPath);

	return MSV_SUCCESS;
}

MsvErrorCode MsvDllAdapter::UnloadDllLibrary()
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	MSV_LOG_INFO(m_spLogger, "Unloading DLL library.");

	if (!Loaded())
	{
		MSV_LOG_INFO(m_spLogger, "DLL library hasn't been loaded.");
		return MSV_NOT_INITIALIZED_INFO;
	}

#ifdef _WIN32
	BOOL result = FreeLibrary(m_pHandle);
	if (!result)
	{
		MSV_LOG_ERROR(m_spLogger, "Free DLL library failed with error: {}", GetLastError());
		return MSV_CLOSE_ERROR;
	}
#else
	int result = dlclose(m_pHandle);
	if (result != 0)
	{
		MSV_LOG_ERROR(m_spLogger, "Close DLL library failed with error ({}): {}", result, dlerror());
		return MSV_CLOSE_ERROR;
	}
#endif //_WIN32

	m_pHandle = nullptr;

	MSV_LOG_INFO(m_spLogger, "DLL library has been successfully unloaded.");

	return MSV_SUCCESS;
}


/** @} */	//End of group MDLLFACTORY.
