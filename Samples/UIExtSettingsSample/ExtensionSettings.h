#pragma once

#include "ExtensionSettings.g.h"

namespace winrt::UIExtSettingsSample::implementation
{
    struct ExtensionSettings : ExtensionSettingsT<ExtensionSettings>
    {
        ExtensionSettings();
        void ClickHandler(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args);
    };
}

namespace winrt::UIExtSettingsSample::factory_implementation
{
    struct ExtensionSettings : ExtensionSettingsT<ExtensionSettings, implementation::ExtensionSettings>
    {
    };
}
