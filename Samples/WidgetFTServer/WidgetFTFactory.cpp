#include "pch.h"
#include "WidgetFTFactory.h"
#include "SampleComponent\SampleComponent.h"

namespace winrt::WidgetFT::implementation
{
    // This allows the widget to get the process handle and monitor for crashes.
    // Client widget can decide how to recover.
    uint64_t WidgetFTFactory::ProcessHandle()
    {
        auto callingPid = GetRpcClientProcessId();
        wil::unique_handle callingProcess{ OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION | SYNCHRONIZE | PROCESS_DUP_HANDLE, FALSE, callingPid) };
        if (LOG_LAST_ERROR_IF(!callingProcess))
        {
            return 0;
        }

        wil::unique_handle thisProcess{ OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION | SYNCHRONIZE, FALSE, GetCurrentProcessId()) };
        if (LOG_LAST_ERROR_IF(!thisProcess))
        {
            return 0;
        }

        HANDLE dupedHandle{};
        if (LOG_LAST_ERROR_IF(!DuplicateHandle(
            GetCurrentProcess(),
            thisProcess.get(),
            callingProcess.get(),
            &dupedHandle,
            0,
            FALSE,
            DUPLICATE_SAME_ACCESS)))
        {
            return 0;
        }

        uint64_t handleUInt64{};
        handleUInt64 = reinterpret_cast<uint64_t>(dupedHandle);

        return handleUInt64;
    }

    WidgetFT::SampleComponent WidgetFTFactory::CreateSampleComponent()
    {
        // We have this component as a singleton, but you could do winrt::make<your_component>() 
        // if you wanted a new instance each time.
        return *SampleComponent::Instance();
    }

    DWORD WidgetFTFactory::GetRpcClientProcessId()
    {
        // try to find the RPC client's PID
        RPC_STATUS result = RPC_S_OK;
        RPC_CALL_ATTRIBUTES callAttributes = {};
        callAttributes.Flags = RPC_QUERY_CLIENT_PID;
        callAttributes.Version = RPC_CALL_ATTRIBUTES_VERSION;
        result = RpcServerInqCallAttributes(0, &callAttributes);
        if (RPC_S_OK != result)
        {
            return 0;
        }
        else
        {
            auto uint64val = reinterpret_cast<uint64_t>(callAttributes.ClientPID);
            return static_cast<DWORD>(uint64val);
        }
    }


    // Register this factory with COM. It uses the CLSID defined in WidgetFTFactoryClsid.h
    // This line of code then ensures when module.Register is called in WinMain.cpp that WidgetFTFactory
    // is registered with COM.
    CoCreatableSingletonCppWinRtClass(WidgetFTFactory);
}
