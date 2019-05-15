/**************************************************************************************************//**
* @addtogroup	MDLLFACTORY
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech Dll Implementation
* @details		Contains implementation @ref MsvDll.
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


#include "MsvDll.h"
#include "MsvDll_Factory.h"

#include "merror/MsvErrorCodes.h"


/********************************************************************************************************************************
*															Constructors and destructors
********************************************************************************************************************************/


MsvDll::MsvDll(std::shared_ptr<MsvLogger> spLogger, std::shared_ptr<MsvDll_Factory> spFactory):
	m_initialized(false),
	m_pGetDllObjectFunction(nullptr),
	m_spDllAdapter(nullptr),
	m_spLogger(spLogger),
	m_spFactory(spFactory ? spFactory : MsvDll_Factory::Get())
{

}

MsvDll::~MsvDll()
{
	Uninitialize();
}


/********************************************************************************************************************************
*															IMsvDll public methods
********************************************************************************************************************************/


MsvErrorCode MsvDll::Initialize(const char* dllPath)
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	MSV_LOG_INFO(m_spLogger, "Initializing DLL library \"{}\".", dllPath);

	if (Initialized())
	{
		MSV_LOG_INFO(m_spLogger, "DLL library \"{}\" has been already initialized.", dllPath);
		return MSV_ALREADY_INITIALIZED_INFO;
	}

	void* pDllAddress = nullptr;
	m_spDllAdapter = m_spFactory->GetIMsvDllAdapter(m_spLogger);
	if (!m_spDllAdapter)
	{
		MSV_LOG_ERROR(m_spLogger, "Create MsvDllAdapter object for DLL library \"{}\" failed.", dllPath);
		return MSV_ALLOCATION_ERROR;
	}
	
	MSV_RETURN_FAILED(m_spDllAdapter->LoadDllLibrary(dllPath));

	m_initialized = true;

	return MSV_SUCCESS;
}

MsvErrorCode MsvDll::Uninitialize()
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	MSV_LOG_INFO(m_spLogger, "Uninitializing DLL library.");

	if (!Initialized())
	{
		MSV_LOG_INFO(m_spLogger, "DLL library has iot been initialized.");
		return MSV_NOT_INITIALIZED_INFO;
	}

	if (GetDllReferenceCount() > 0)
	{
		//DLL has referenced objects (it might me shared_ptr in DLL but it might be shared_ptr anywhere else, we don't know) -> just log WARNING
		MSV_LOG_WARN(m_spLogger, "Unloading DLL library with references - reference count: {}", GetDllReferenceCount());

		//return MSV_INVALID_DATA_ERROR;
	}

	//delete all weak references and unload library
	m_dllObjects.clear();
	MSV_RETURN_FAILED(m_spDllAdapter->UnloadDllLibrary());

	m_pGetDllObjectFunction = nullptr;	
	m_spDllAdapter.reset();

	m_initialized = false;

	return MSV_SUCCESS;
}

bool MsvDll::Initialized() const
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	return m_initialized;
}

MsvErrorCode MsvDll::GetDllObject(const char* id, std::shared_ptr<IMsvDllObject>& spDllObject, std::shared_ptr<IMsvDllDecorator> spDecorator)
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	MSV_LOG_INFO(m_spLogger, "Getting DLL object \"{}\".", id);

	if (!Initialized())
	{
		MSV_LOG_ERROR(m_spLogger, "Trying to get DLL object \"{}\" from uninitialized DLL.", id);
		return MSV_NOT_INITIALIZED_ERROR;
	}

	//check if is in map
	std::map<std::string, std::weak_ptr<IMsvDllObject>>::iterator it = m_dllObjects.find(id);
	if (it != m_dllObjects.end())
	{
		if ((spDllObject = it->second.lock()))
		{
			return MSV_SUCCESS;
		}
	}

	//not in map or weak_ptr is expired

	//object is not in the map -> load it
	std::shared_ptr<IMsvDllObject> spInnerDllObject;
	if (spDecorator)
	{
		//it is DLL without exported function GetDllObject (probably C DLL, third party DLL, etc.)
		MSV_RETURN_FAILED(spDecorator->DecorateDllObject(id, m_spDllAdapter));
		spInnerDllObject = spDecorator;
	}
	else
	{
		//it is DLL with exported function GetDllObject
		MsvErrorCode errorCode = MSV_SUCCESS;

		//check if GetDllObject is already loaded and load it if not
		if (!m_pGetDllObjectFunction)
		{
			void* pDllAddress = nullptr;
			if (MSV_FAILED(errorCode = m_spDllAdapter->GetDllAddress("GetDllObject", pDllAddress)))
			{
				MSV_LOG_ERROR(m_spLogger, "Get DLL address \"GetDllObject\" (for object \"{}\") failed with error: {0:x}.", id, errorCode);
				return errorCode;
			}

			//m_pGetDllObjectFunction = (MsvErrorCode (*)(const char* id, std::shared_ptr<IMsvDllObject>& spDllObject))pDllAddress;
			m_pGetDllObjectFunction = reinterpret_cast<MsvErrorCode(*)(const char* id, std::shared_ptr<IMsvDllObject>& spDllObject)>(pDllAddress);
		}

		//GetDllObject function is loaded -> load requested DLL object
		if (MSV_FAILED(errorCode = m_pGetDllObjectFunction(id, spInnerDllObject)))
		{
			MSV_LOG_ERROR(m_spLogger, "Get DLL object \"{}\" from DLL failed with error: {0:x}.", id, errorCode);
			return errorCode;
		}
	}

	//insert object to map
	m_dllObjects[id] = spInnerDllObject;
	spDllObject = spInnerDllObject;

	return MSV_SUCCESS;
}

std::int64_t MsvDll::GetDllReferenceCount() const
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	std::int64_t referenceCount = 0;

	//get all objects from DLL and add reference count
	std::map<std::string, std::weak_ptr<IMsvDllObject>>::const_iterator endIt = m_dllObjects.end();
	for (std::map<std::string, std::weak_ptr<IMsvDllObject>>::const_iterator it = m_dllObjects.begin(); it != endIt; ++it)
	{
		referenceCount += it->second.use_count();

		//consider to add reference counting to IMsvDllObject
		//referenceCount += it->second->GetReferenceCount();
	}

	return referenceCount;
}


/** @} */	//End of group MDLLFACTORY.
