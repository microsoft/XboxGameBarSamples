#pragma once
#include "WidgetFT.SampleComponent.g.h"

namespace winrt::WidgetFT::implementation
{
    // wrlrt::module_base is required to ensure the ref count of you COM server works properly
    // xblib::singleton_winrt_base adds singleton support to this class. If you want to do initialization on creation
    // see variant inside singleton_base.h.
    struct SampleComponent : SampleComponentT<SampleComponent, wrlrt::module_base>, xblib::singleton_winrt_base<SampleComponent>
    {
        SampleComponent() = default;

        winrt::Windows::Foundation::IAsyncAction DemoAsync();
        void DemoSync();
        bool DemoBoolProperty();
        void DemoBoolProperty(bool value);
        winrt::event_token DemoBoolPropertyChanged(winrt::WidgetFT::DemoBoolPropertyChangedDelegate const& handler);
        void DemoBoolPropertyChanged(winrt::event_token const& token) noexcept;

    private:

        winrt::fire_and_forget RaiseDemoBoolPropertyChanged(bool value);

    private:

        bool m_demoBoolProperty{ false };
        winrt::event<winrt::WidgetFT::DemoBoolPropertyChangedDelegate> m_demoBoolPropertyChangedEvent;
    };
}
