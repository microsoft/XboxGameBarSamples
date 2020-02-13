#pragma once

#include "ExtensionSettings.g.h"

namespace winrt::UIExtSettingsSampleCPP::implementation
{
    struct ExtensionSettings : ExtensionSettingsT<ExtensionSettings>
    {
        ExtensionSettings();
        void ClickHandler(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args);
    };
}

namespace winrt::UIExtSettingsSampleCPP::factory_implementation
{
    struct ExtensionSettings : ExtensionSettingsT<ExtensionSettings, implementation::ExtensionSettings>
    {
    };
}
