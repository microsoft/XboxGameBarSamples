#pragma once

// This file is shared between the WidgetFTServer and the WidgetFTSample projects. It provides
// the CLSID for the WidgetFTFactory class. This is the glue that allows the CoCreate from the
// widget process to find and connect to the object on the server side. Once connected, we'll
// just create objects using this factory. Factory implementation is in WidgetFTFactory.h/cpp.
// WidgetFTFactory.cpp contains the registration of the object with WRL
//
// CoCreatableSingletonCppWinRtClass(WidgetFTFactory);
//
// In WinMain.cpp, when module.Register is called, the factory is registered with COM.

namespace winrt::WidgetFT::implementation
{
    // Be sure to update this GUID via VS -> Tools -> Create GUID or similar tool
    struct __declspec(uuid("C8B22328-C713-431C-A29C-D81C46D3C34C")) WidgetFTFactory;
}

const CLSID CLSID_WidgetFTFactory = __uuidof(winrt::WidgetFT::implementation::WidgetFTFactory);