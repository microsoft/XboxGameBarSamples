#pragma once

#include "Widget.g.h"
#include <winrt/Windows.UI.Xaml.Navigation.h>
#include <winrt/Microsoft.Gaming.XboxGameBar.h>

namespace winrt::WidgetSettingsSample::implementation
{
    struct Widget : WidgetT<Widget>
    {
        Widget();

        virtual void OnNavigatedTo(winrt::Windows::UI::Xaml::Navigation::NavigationEventArgs e);

        void MyButton_Click(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args);

        // Settings click handler for widget settings click event
        Windows::Foundation::IAsyncAction SettingsButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::Foundation::IInspectable const& e);
      
    private:
        winrt::event_token m_settingsToken{};
        Microsoft::Gaming::XboxGameBar::XboxGameBarWidget m_widget{ nullptr };
        Microsoft::Gaming::XboxGameBar::XboxGameBarWidgetControl m_widgetControl{ nullptr };
    };
}

namespace winrt::WidgetSettingsSample::factory_implementation
{
    struct Widget : WidgetT<Widget, implementation::Widget>
    {
    };
}
