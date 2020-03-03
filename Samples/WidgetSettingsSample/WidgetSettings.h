#pragma once

#include "WidgetSettings.g.h"

namespace winrt::WidgetSettingsSample::implementation
{
    struct WidgetSettings : WidgetSettingsT<WidgetSettings>
    {
        WidgetSettings();
        void MyButton_Click(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args);
    };
}

namespace winrt::WidgetSettingsSample::factory_implementation
{
    struct WidgetSettings : WidgetSettingsT<WidgetSettings, implementation::WidgetSettings>
    {
    };
}
