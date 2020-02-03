#pragma once

#include "Extension1.g.h"
#include <winrt/Windows.UI.Xaml.Navigation.h>
#include <winrt/Microsoft.Gaming.XboxGameBar.h>

namespace winrt::UIExtAdvSample::implementation
{
    struct Extension1 : Extension1T<Extension1>
    {
        Extension1();

        virtual void OnNavigatedTo(winrt::Windows::UI::Xaml::Navigation::NavigationEventArgs e);

        Windows::Foundation::IAsyncAction ActivateAsyncAppExtIdButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        Windows::Foundation::IAsyncAction ActivateAsyncAppIdButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        Windows::Foundation::IAsyncAction ActivateWithUriAsyncButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        Windows::Foundation::IAsyncAction MinimizeAsyncAppIdButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        Windows::Foundation::IAsyncAction MinimizeAsyncAppExtIdButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        Windows::Foundation::IAsyncAction MaximizeAsyncAppExtIdButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        Windows::Foundation::IAsyncAction MaximizeAsyncAppIdButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        Windows::Foundation::IAsyncAction CloseAsyncAppExtIdButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        Windows::Foundation::IAsyncAction CloseAsyncAppIdButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);

    private:

        Microsoft::Gaming::XboxGameBar::XboxGameBarUIExtension m_uiExtension{ nullptr };
        Microsoft::Gaming::XboxGameBar::XboxGameBarExtensionControl m_extensionControl{ nullptr };
    };
}

namespace winrt::UIExtAdvSample::factory_implementation
{
    struct Extension1 : Extension1T<Extension1, implementation::Extension1>
    {
    };
}
