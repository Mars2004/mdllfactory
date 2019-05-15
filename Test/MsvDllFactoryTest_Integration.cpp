
#include "pch.h"


#include "mdllfactory/MsvDllFactory.h"
#include "mdllfactory/MsvDllList.h"

#include "merror/MsvErrorCodes.h"

#include "mdllfactory/Test/testdll_1/MsvTest1DllObject.h"


#ifndef MSV_TEST_WITH_LOGGING
#define MSV_TEST_WITH_LOGGING 0
#endif


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

class MsvTestDllObject:
	public IMsvDllObject
{
public:
	MsvTestDllObject()
	{

	}
};

class MsvTestDllList:
	public MsvDllList
{
public:
	MsvTestDllList(std::shared_ptr<MsvLogger> spLogger = nullptr):
		MsvDllList(spLogger)
	{
		
	}

	MsvErrorCode Initialize()
	{
		//two different objects in one DLL
		MSV_RETURN_FAILED(AddDll("{9F31D4A9-CDF5-49FA-90A8-AA109735DC9F}", "testdll_1.dll"));
		MSV_RETURN_FAILED(AddDll("{337AB087-1B69-4561-A0E4-771723EFCBFE}", "testdll_1.dll", nullptr));
		//this is not in DLL (for check error handling)
		MSV_RETURN_FAILED(AddDll("{82ABA7A1-5BBC-4F14-B0E6-866BB6BB8136}", "testdll_1.dll"));

		//DLL without exported GetDllObject function
		std::shared_ptr<IMsvDllDecorator> spTestDll2(new (std::nothrow) TestDll2());
		if (!spTestDll2) { return MSV_ALLOCATION_ERROR; }
		MSV_RETURN_FAILED(AddDll("{BB8B8CC6-FF99-47C8-8668-F17ABE5E63A6}", "testdll_2.dll", spTestDll2));
		//this is not in DLL (for check error handling) - will try to load GetDllObject exported function and should failed
		MSV_RETURN_FAILED(AddDll("{3FB71C99-07EB-48BB-91CD-13EC5F53E49B}", "testdll_2.dll"));

		return MSV_SUCCESS;
	}
};

class MsvDllFactory_Integration:
	public::testing::Test
{
public:
	MsvDllFactory_Integration()
	{

	}

	virtual void SetUp()
	{
#if defined(MSV_TEST_WITH_LOGGING) && MSV_TEST_WITH_LOGGING != 0
		m_spLoggerProvider.reset(new (std::nothrow) MsvLoggerProvider());
		EXPECT_NE(m_spLoggerProvider, nullptr);
		m_spLogger = m_spLoggerProvider->GetLogger();
		EXPECT_NE(m_spLogger, nullptr);
#endif

		m_spDllList.reset(new (std::nothrow) MsvTestDllList(m_spLogger));
		EXPECT_NE(m_spDllList, nullptr);
		EXPECT_TRUE(MSV_SUCCEEDED(m_spDllList->Initialize()));

		m_spDllFactory.reset(new (std::nothrow) MsvDllFactory(m_spDllList, m_spLogger));
		EXPECT_NE(m_spDllFactory, nullptr);
	}

	virtual void TearDown()
	{
		m_spDllFactory.reset();
		m_spDllList.reset();
	}

	//logger
	std::shared_ptr<IMsvLoggerProvider> m_spLoggerProvider;
	std::shared_ptr<MsvLogger> m_spLogger;

	//test functions anc classes
	std::shared_ptr<MsvTestDllList> m_spDllList;
	std::shared_ptr<MsvDllFactory> m_spDllFactory;
};

TEST_F(MsvDllFactory_Integration, ItShouldFailedWhenDllIsNotInList)
{
	std::shared_ptr<IMsvDll> spDll;
	EXPECT_EQ(m_spDllFactory->GetDll("{7368D519-0F40-40BE-B7FE-EA382279219F}", spDll), MSV_NOT_FOUND_ERROR);

	std::shared_ptr<IMsvDllObject> spDllObject;
	EXPECT_EQ(m_spDllFactory->GetDllObject("{7368D519-0F40-40BE-B7FE-EA382279219F}", spDllObject), MSV_NOT_FOUND_ERROR);

	std::shared_ptr<MsvTestDllObject> spTestDllObject;
	MsvErrorCode errorCode = std::static_pointer_cast<IMsvDllFactory, MsvDllFactory>(m_spDllFactory)->GetDllObject<MsvTestDllObject>("{7368D519-0F40-40BE-B7FE-EA382279219F}", spTestDllObject);
	EXPECT_EQ(errorCode, MSV_NOT_FOUND_ERROR);
}

TEST_F(MsvDllFactory_Integration, ItShouldReturnSameDlls)
{
	std::shared_ptr<IMsvDll> spDll1;
	EXPECT_EQ(m_spDllFactory->GetDll("{9F31D4A9-CDF5-49FA-90A8-AA109735DC9F}", spDll1), MSV_SUCCESS);

	std::shared_ptr<IMsvDll> spDll2;
	EXPECT_EQ(m_spDllFactory->GetDll("{337AB087-1B69-4561-A0E4-771723EFCBFE}", spDll2), MSV_SUCCESS);

	EXPECT_EQ(spDll1, spDll2);
}

TEST_F(MsvDllFactory_Integration, ItShouldReturnSameDllObjects)
{
	std::shared_ptr<MsvTest1DllObject> spDllObject1;
	MsvErrorCode errorCode = std::static_pointer_cast<IMsvDllFactory, MsvDllFactory>(m_spDllFactory)->GetDllObject<MsvTest1DllObject>("{9F31D4A9-CDF5-49FA-90A8-AA109735DC9F}", spDllObject1);
	EXPECT_EQ(errorCode, MSV_SUCCESS);

	std::shared_ptr<MsvTest1DllObject> spDllObject2;
	errorCode = std::static_pointer_cast<IMsvDllFactory, MsvDllFactory>(m_spDllFactory)->GetDllObject<MsvTest1DllObject>("{337AB087-1B69-4561-A0E4-771723EFCBFE}", spDllObject2);
	EXPECT_EQ(errorCode, MSV_SUCCESS);

	EXPECT_EQ(spDllObject1, spDllObject2);
}

TEST_F(MsvDllFactory_Integration, ItShouldFailedWhenObjectIsNotInDll)
{
	//check that DLL can be loaded for this ID
	std::shared_ptr<IMsvDll> spDll;
	EXPECT_EQ(m_spDllFactory->GetDll("{82ABA7A1-5BBC-4F14-B0E6-866BB6BB8136}", spDll), MSV_SUCCESS);

	std::shared_ptr<IMsvDllObject> spDllObject;
	EXPECT_EQ(m_spDllFactory->GetDllObject("{82ABA7A1-5BBC-4F14-B0E6-866BB6BB8136}", spDllObject), MSV_NOT_FOUND_ERROR);
	EXPECT_TRUE(spDllObject == nullptr);
}

TEST_F(MsvDllFactory_Integration, ItShouldFailedWhenObjectIsNotInDllList)
{
	//check that DLL can be loaded for this ID
	std::shared_ptr<IMsvDll> spDll;
	EXPECT_EQ(m_spDllFactory->GetDll("{7D1AC2ED-F3CD-4540-9591-B5D7CE51703D}", spDll), MSV_NOT_FOUND_ERROR);
	EXPECT_TRUE(spDll == nullptr);

	std::shared_ptr<IMsvDllObject> spDllObject;
	EXPECT_EQ(m_spDllFactory->GetDllObject("{7D1AC2ED-F3CD-4540-9591-B5D7CE51703D}", spDllObject), MSV_NOT_FOUND_ERROR);
	EXPECT_TRUE(spDllObject == nullptr);
}

TEST_F(MsvDllFactory_Integration, ItShouldFailedWhenDecoratorIsNotDefined)
{
	//check that DLL can be loaded for this ID
	std::shared_ptr<IMsvDll> spDll;
	EXPECT_EQ(m_spDllFactory->GetDll("{3FB71C99-07EB-48BB-91CD-13EC5F53E49B}", spDll), MSV_SUCCESS);
	EXPECT_NE(spDll, nullptr);

	std::shared_ptr<IMsvDllObject> spDllObject;
	EXPECT_EQ(m_spDllFactory->GetDllObject("{3FB71C99-07EB-48BB-91CD-13EC5F53E49B}", spDllObject), MSV_NOT_FOUND_ERROR);
	EXPECT_TRUE(spDllObject == nullptr);
}

TEST_F(MsvDllFactory_Integration, ItShouldReturnValidObjectWhenDecoratorIsDefined)
{
	//check that DLL can be loaded for this ID
	std::shared_ptr<IMsvDll> spDll;
	EXPECT_EQ(m_spDllFactory->GetDll("{BB8B8CC6-FF99-47C8-8668-F17ABE5E63A6}", spDll), MSV_SUCCESS);
	EXPECT_NE(spDll, nullptr);

	std::shared_ptr<IMsvDllObject> spDllObject;
	EXPECT_EQ(m_spDllFactory->GetDllObject("{BB8B8CC6-FF99-47C8-8668-F17ABE5E63A6}", spDllObject), MSV_SUCCESS);
	EXPECT_NE(spDllObject, nullptr);
}

TEST_F(MsvDllFactory_Integration, ItShouldCallDllFunctions)
{
	std::shared_ptr<TestDll2> spDllObject;
	MsvErrorCode errorCode = std::static_pointer_cast<IMsvDllFactory, MsvDllFactory>(m_spDllFactory)->GetDllObject<TestDll2>("{BB8B8CC6-FF99-47C8-8668-F17ABE5E63A6}", spDllObject);
	EXPECT_EQ(errorCode, MSV_SUCCESS);
	EXPECT_NE(spDllObject, nullptr);

	EXPECT_EQ(spDllObject->Increment(), 1);
	EXPECT_EQ(spDllObject->Increment(), 2);

	EXPECT_EQ(spDllObject->GetValue(), 2);

	EXPECT_EQ(spDllObject->Decrement(), 1);
	EXPECT_EQ(spDllObject->Decrement(), 0);

	EXPECT_EQ(spDllObject->GetValue(), 0);
}

TEST_F(MsvDllFactory_Integration, ItShouldFailedWhenTryingToReleaseDllHoldedByAnyoneElse)
{
	std::shared_ptr<IMsvDll> spDll;
	EXPECT_EQ(m_spDllFactory->GetDll("{9F31D4A9-CDF5-49FA-90A8-AA109735DC9F}", spDll), MSV_SUCCESS);
	EXPECT_NE(spDll, nullptr);

	EXPECT_EQ(m_spDllFactory->ReleaseDll("{9F31D4A9-CDF5-49FA-90A8-AA109735DC9F}"), MSV_NOT_ALLOWED_ERROR);

	EXPECT_EQ(spDll.use_count(), 2);
}

/*
//currently it will succeded (we don't know if references is hold in DLL or somewhere else, it unloads DLL anyway)
TEST_F(MsvDllFactory_Integration, ItShouldFailedWhenTryingToReleaseDllObjectHoldedByAnyoneElse)
{
	std::shared_ptr<MsvTest1DllObject> spDllObject;
	MsvErrorCode errorCode = std::static_pointer_cast<IMsvDllFactory, MsvDllFactory>(m_spDllFactory)->GetDllObject<MsvTest1DllObject>("{9F31D4A9-CDF5-49FA-90A8-AA109735DC9F}", spDllObject);
	EXPECT_EQ(errorCode, MSV_SUCCESS);
	EXPECT_NE(spDllObject, nullptr);

	EXPECT_EQ(m_spDllFactory->ReleaseDll("{9F31D4A9-CDF5-49FA-90A8-AA109735DC9F}"), MSV_NOT_ALLOWED_ERROR);
}*/

TEST_F(MsvDllFactory_Integration, ItShouldSucceededToUnloadDllHoldedOnlyByFactory)
{
	std::shared_ptr<IMsvDll> spDll;
	EXPECT_EQ(m_spDllFactory->GetDll("{9F31D4A9-CDF5-49FA-90A8-AA109735DC9F}", spDll), MSV_SUCCESS);
	spDll = nullptr;
	EXPECT_EQ(spDll, nullptr);
	EXPECT_EQ(spDll.use_count(), 0);

	std::shared_ptr<MsvTest1DllObject> spDllObject;
	MsvErrorCode errorCode = std::static_pointer_cast<IMsvDllFactory, MsvDllFactory>(m_spDllFactory)->GetDllObject<MsvTest1DllObject>("{9F31D4A9-CDF5-49FA-90A8-AA109735DC9F}", spDllObject);
	EXPECT_EQ(errorCode, MSV_SUCCESS);
	spDllObject = nullptr;
	EXPECT_EQ(spDllObject, nullptr);
	EXPECT_EQ(spDllObject.use_count(), 0);

	EXPECT_EQ(m_spDllFactory->ReleaseDll("{9F31D4A9-CDF5-49FA-90A8-AA109735DC9F}"), MSV_SUCCESS);
}

TEST_F(MsvDllFactory_Integration, ItShouldFailedTryingToUnloadNotLoadedDll)
{
	EXPECT_EQ(m_spDllFactory->ReleaseDll("{9F31D4A9-CDF5-49FA-90A8-AA109735DC9F}"), MSV_NOT_FOUND_INFO);
}

TEST_F(MsvDllFactory_Integration, ItShouldFailedTryingToUnloadWhenDllIsNotInList)
{
	EXPECT_EQ(m_spDllFactory->ReleaseDll("{7368D519-0F40-40BE-B7FE-EA382279219F}"), MSV_NOT_FOUND_ERROR);
}
