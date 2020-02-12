#pragma once

#include "Extension1Settings.g.h"

namespace winrt::UIExtAdvSample::implementation
{
    struct Extension1Settings : Extension1SettingsT<Extension1Settings>
    {
        Extension1Settings();

        void ClickHandler(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args);
    };
}

namespace winrt::UIExtAdvSample::factory_implementation
{
    struct Extension1Settings : Extension1SettingsT<Extension1Settings, implementation::Extension1Settings>
    {
    };
}
