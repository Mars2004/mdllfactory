/**************************************************************************************************//**
* @addtogroup	MDLLFACTORY
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech Dll Factory Implementation
* @details		Contains implementation @ref MsvDllFactory.
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


#include "MsvDllFactory.h"
#include "MsvDllFactory_Factory.h"

#include "merror/MsvErrorCodes.h"


/********************************************************************************************************************************
*															Constructors and destructors
********************************************************************************************************************************/


MsvDllFactory::MsvDllFactory(const std::shared_ptr<IMsvDllList>& spDllList, std::shared_ptr<MsvLogger> spLogger, std::shared_ptr<MsvDllFactory_Factory> spFactory):
	m_spDllList(spDllList),
	m_spLogger(spLogger),
	m_spFactory(spFactory ? spFactory : MsvDllFactory_Factory::Get())
{

}


/********************************************************************************************************************************
*															IMsvDllFactory public methods
********************************************************************************************************************************/


MsvErrorCode MsvDllFactory::GetDll(const char* id, std::shared_ptr<IMsvDll>& spDll)
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	std::shared_ptr<IMsvDllDecorator> spDecorator;

	return GetDll(id, spDll, spDecorator);
}

MsvErrorCode MsvDllFactory::GetDllObject(const char* id, std::shared_ptr<IMsvDllObject>& spDllObject)
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	MSV_LOG_INFO(m_spLogger, "Getting DLL object \"{}\".", id);

	std::shared_ptr<IMsvDll> spDll;
	std::shared_ptr<IMsvDllDecorator> spDecorator;
	MSV_RETURN_FAILED(GetDll(id, spDll, spDecorator));
	MSV_RETURN_FAILED(spDll->GetDllObject(id, spDllObject, spDecorator));

	MSV_LOG_INFO(m_spLogger, "Returning DLL object \"{}\".", id);

	return MSV_SUCCESS;
}

MsvErrorCode MsvDllFactory::ReleaseDll(const char* id)
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	MSV_LOG_INFO(m_spLogger, "Releasing DLL library \"{}\".", id);

	std::string dllPath;
	std::shared_ptr<IMsvDllDecorator> spDecorator;
	MSV_RETURN_FAILED(m_spDllList->GetDll(id, dllPath, spDecorator));

	//we have DLL data -> check if is already loaded (in list)
	std::map<std::string, std::shared_ptr<IMsvDll>>::const_iterator it = m_loadedDlls.find(dllPath);
	if (it != m_loadedDlls.end())
	{
		//check if can unload DLL
		if (it->second.use_count() > 1)
		{
			//DLL is holded by anyone else (can't release)
			MSV_LOG_ERROR(m_spLogger, "DLL library \"{}\" (\"{}\") is hold by someone else (not only by DLL factory).", id, dllPath);
			return MSV_NOT_ALLOWED_ERROR;
		}

		MSV_LOG_INFO(m_spLogger, "Uninitializing DLL library \"{}\" (\"{}\").", id, dllPath);
		
		//uninitialize, unload and release DLL
		MSV_RETURN_FAILED(it->second->Uninitialize());
		m_loadedDlls.erase(it);

		MSV_LOG_INFO(m_spLogger, "DLL library \"{}\" (\"{}\") has been successfully unitialized, unloaded and released.", id, dllPath);

		return MSV_SUCCESS;
	}

	MSV_LOG_INFO(m_spLogger, "DLL library \"{}\" (\"{}\") has not been loaded.", id, dllPath);

	//not in list (not loaded or already released) - just info
	return MSV_NOT_FOUND_INFO;
}

MsvErrorCode MsvDllFactory::GetDll(const char* id, std::shared_ptr<IMsvDll>& spDll, std::shared_ptr<IMsvDllDecorator>& spDecorator)
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	MSV_LOG_INFO(m_spLogger, "Getting DLL library \"{}\".", id);

	std::string dllPath;

	MSV_RETURN_FAILED(m_spDllList->GetDll(id, dllPath, spDecorator));

	//we have DLL data -> check if is already loaded (in list)
	std::map<std::string, std::shared_ptr<IMsvDll>>::const_iterator it = m_loadedDlls.find(dllPath);
	if (it != m_loadedDlls.end())
	{
		MSV_LOG_INFO(m_spLogger, "DLL library \"{}\" (\"{}\") has been already loaded - returning it.", id, dllPath);
		spDll = it->second;
		return MSV_SUCCESS;
	}

	//not in list
	MSV_LOG_INFO(m_spLogger, "DLL library \"{}\" (\"{}\") is not loaded - loading it.", id, dllPath);

	MsvErrorCode errorCode = MSV_SUCCESS;
	std::shared_ptr<IMsvDll> spInnerDll = m_spFactory->GetIMsvDll(m_spLogger);
	if (!spInnerDll)
	{
		MSV_LOG_ERROR(m_spLogger, "Create DLL library object for \"{}\" (\"{}\") failed.", id, dllPath);
		return MSV_ALLOCATION_ERROR;
	}

	if (MSV_FAILED(errorCode = spInnerDll->Initialize(dllPath.c_str())))
	{
		MSV_LOG_ERROR(m_spLogger, "Initialize DLL library object for \"{}\" (\"{}\") failed with error: {}", id, dllPath, errorCode);
		return errorCode;
	}

	m_loadedDlls[dllPath] = spInnerDll;
	spDll = spInnerDll;

	MSV_LOG_INFO(m_spLogger, "DLL library \"{}\" (\"{}\") has been successfully loaded.", id, dllPath);

	return errorCode;
}


/** @} */	//End of group MDLLFACTORY.
