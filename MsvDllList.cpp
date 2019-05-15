/**************************************************************************************************//**
* @addtogroup	MDLLFACTORY
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech Dll List Implementation
* @details		Contains implementation @ref IMsvDllList.
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


#include "MsvDllList.h"

#include "merror/MsvErrorCodes.h"


/********************************************************************************************************************************
*															IMsvDllList::MsvDllData implementation
********************************************************************************************************************************/


MsvDllList::MsvDllData::MsvDllData(const char* dllPath, std::shared_ptr<IMsvDllDecorator> spDllDecorator):
	m_dllPath(dllPath),
	m_spDllDecorator(spDllDecorator)
{

}

void MsvDllList::MsvDllData::GetDllData(std::string& dllPath, std::shared_ptr<IMsvDllDecorator>& spDllDecorator) const
{
	dllPath.assign(m_dllPath);
	spDllDecorator = m_spDllDecorator;
}


/********************************************************************************************************************************
*															Constructors and destructors
********************************************************************************************************************************/


MsvDllList::MsvDllList(std::shared_ptr<MsvLogger> spLogger):
	m_spLogger(spLogger)
{
	/*
	//It might be usefull to create whole DLL table in child constructor or in some initialize method (better for check error codes):

	MSV_RETURN_FAILED(AddDll("{9F31D4A9-CDF5-49FA-90A8-AA109735DC9F}", "<path_to_dll>"));
	MSV_RETURN_FAILED(AddDll("{337AB087-1B69-4561-A0E4-771723EFCBFE}", "<path_to_dll>", nullptr));
	std::shared_ptr<IMsvDllDecorator> spDllDecorator(new (std::nothrow) MsvDllDecorator());
	if (!spDllDecorator) { return MSV_ALLOCATION_ERROR; }
	MSV_RETURN_FAILED(AddDll("{BB8B8CC6-FF99-47C8-8668-F17ABE5E63A6}", "<path_to_dll>", spDllDecorator));

	*/
}

MsvDllList::~MsvDllList()
{

}


/********************************************************************************************************************************
*															IMsvDllList public methods
********************************************************************************************************************************/


MsvErrorCode MsvDllList::GetDll(const char* id, std::string& dllPath, std::shared_ptr<IMsvDllDecorator>& spDllDecorator) const
{
	MSV_LOG_INFO(m_spLogger, "Getting DLL library \"{}\" data from list.", id);

	std::map<std::string, std::shared_ptr<MsvDllData>>::const_iterator it = m_dlls.find(id);
	if (it != m_dlls.end())
	{
		//it is in map -> return DLL data
		it->second->GetDllData(dllPath, spDllDecorator);
		return MSV_SUCCESS;
	}

	MSV_LOG_ERROR(m_spLogger, "DLL library \"{}\" is not in the list.", id);

	//not in map -> not found error
	return MSV_NOT_FOUND_ERROR;
}


/********************************************************************************************************************************
*															MsvDllList public methods
********************************************************************************************************************************/


MsvErrorCode MsvDllList::AddDll(const char* id, const char* dllPath, std::shared_ptr<IMsvDllDecorator> spDllDecorator)
{
	MSV_LOG_INFO(m_spLogger, "Adding DLL library \"{}\" to DLL list.", id);

	std::map<std::string, std::shared_ptr<MsvDllData>>::iterator it = m_dlls.find(id);
	if (it != m_dlls.end())
	{
		//dll already exists -> return error (probably called twice for one DLL, but it might be copy paste error, when id is used more times for more DLLs)
		MSV_LOG_ERROR(m_spLogger, "DLL library \"{}\" already in the list.", id);
		return MSV_ALREADY_EXISTS_ERROR;
	}

	std::shared_ptr<MsvDllData> spDllData(new (std::nothrow) MsvDllData(dllPath, spDllDecorator));
	if (!spDllData)
	{
		MSV_LOG_ERROR(m_spLogger, "Create MsvDllData for DLL library \"{}\" failed.", id);
		return MSV_ALLOCATION_ERROR;
	}

	m_dlls[id] = spDllData;

	return MSV_SUCCESS;
}


/** @} */	//End of group MDLLFACTORY.
