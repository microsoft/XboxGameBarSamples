#pragma once

#include "Extension.g.h"
#include <winrt/Windows.UI.Xaml.Navigation.h>
#include <winrt/Microsoft.Gaming.XboxGameBar.h>

namespace winrt::UIExtSettingsSample::implementation
{
    struct Extension : ExtensionT<Extension>
    {
        Extension();

        virtual void OnNavigatedTo(winrt::Windows::UI::Xaml::Navigation::NavigationEventArgs e);

        void MyButton_Click(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args);

        // Settings click handler for ui extension settings click event
        Windows::Foundation::IAsyncAction SettingsButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::Foundation::IInspectable const& e);
      
    private:
        winrt::event_token m_settingsToken{};
        Microsoft::Gaming::XboxGameBar::XboxGameBarUIExtension m_uiExtension{ nullptr };
        Microsoft::Gaming::XboxGameBar::XboxGameBarExtensionControl m_extensionControl{ nullptr };
    };
}

namespace winrt::UIExtSettingsSample::factory_implementation
{
    struct Extension : ExtensionT<Extension, implementation::Extension>
    {
    };
}
