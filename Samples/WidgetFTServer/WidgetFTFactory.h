#pragma once
#include "inc/WidgetFTFactoryClsid.h"
#include "WidgetFT.WidgetFtFactory.g.h"
#include "WidgetFT.SampleComponent.g.h"

namespace winrt::WidgetFT::implementation
{
    struct WidgetFTFactory : WidgetFTFactoryT<WidgetFTFactory, wrlrt::module_base>, xblib::singleton_winrt_base<WidgetFTFactory>
    {
        WidgetFTFactory() = default;

        uint64_t ProcessHandle();

        WidgetFT::SampleComponent CreateSampleComponent();

    private:
        DWORD GetRpcClientProcessId();
    };
}