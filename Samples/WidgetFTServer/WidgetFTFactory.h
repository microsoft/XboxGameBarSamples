#pragma once
#include "WidgetFT.WidgetFtFactory.g.h"
#include "WidgetFT.SampleComponent.g.h"

namespace winrt::WidgetFT::implementation
{
    struct WidgetFTFactory : WidgetFTFactoryT<WidgetFTFactory, wrlrt::module_base>
    {
        WidgetFTFactory() = default;

        uint64_t ProcessHandle();

        WidgetFT::SampleComponent CreateSampleComponent();

    private:
        DWORD GetRpcClientProcessId();
    };
}