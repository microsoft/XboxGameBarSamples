#pragma once

#include <WidgetFTFactoryClsid.h>
#include <winrt/WidgetFT.h>

// This file has the helper(s) to connect to the Full Trust COM server.
//
// Connection must be established using classic COM, but then we immediately switch over
// to using WinRT. You could do the same in C# if you desire. You would just need to pinvoke the
// CoCreation of the object, then cast the object to the WinRT projection you got for C#.
// In this case, we're casting to C++/WinRT for the nice C++ projection support.

namespace winrt::WidgetFTSample
{
    inline winrt::WidgetFT::WidgetFTFactory GetWidgetFTFactory()
    {
        return winrt::create_instance<winrt::WidgetFT::WidgetFTFactory>(CLSID_WidgetFTFactory, CLSCTX_LOCAL_SERVER);
    }
}