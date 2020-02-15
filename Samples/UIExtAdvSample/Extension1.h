#pragma once

#include "Extension1.g.h"
#include <winrt/Windows.UI.Xaml.Navigation.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Microsoft.Gaming.XboxGameBar.h>

namespace winrt::UIExtAdvSample::implementation
{
    struct Extension1 : Extension1T<Extension1>
    {
        Extension1();

        virtual void OnNavigatedTo(winrt::Windows::UI::Xaml::Navigation::NavigationEventArgs e);

        // Click handlers from our XAML events
        Windows::Foundation::IAsyncAction ActivateAsyncAppExtIdButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        Windows::Foundation::IAsyncAction ActivateAsyncAppIdButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        Windows::Foundation::IAsyncAction ActivateWithUriAsyncButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        Windows::Foundation::IAsyncAction MinimizeAsyncAppIdButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        Windows::Foundation::IAsyncAction MinimizeAsyncAppExtIdButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        Windows::Foundation::IAsyncAction RestoreAsyncAppExtIdButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        Windows::Foundation::IAsyncAction RestoreAsyncAppIdButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        Windows::Foundation::IAsyncAction CloseAsyncAppExtIdButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        Windows::Foundation::IAsyncAction CloseAsyncAppIdButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        Windows::Foundation::IAsyncAction TryResizeWindowAsync_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);

        // Settings click handler for ui extension settings click event
        Windows::Foundation::IAsyncAction SettingsButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::Foundation::IInspectable const& e);
        winrt::fire_and_forget FavoritedChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::Foundation::IInspectable const& e);
        void GameBarDisplayModeChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::Foundation::IInspectable const& e);
        winrt::fire_and_forget PinnedChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::Foundation::IInspectable const& e);
        winrt::fire_and_forget  RequestedThemeChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::Foundation::IInspectable const& e);
        winrt::fire_and_forget VisibleChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::Foundation::IInspectable const& e);
        winrt::fire_and_forget WindowStateChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::Foundation::IInspectable const& e);

    private:
        winrt::event_token m_settingsToken{};

        Microsoft::Gaming::XboxGameBar::XboxGameBarUIExtension m_uiExtension{ nullptr };
        Microsoft::Gaming::XboxGameBar::XboxGameBarExtensionControl m_extensionControl{ nullptr };
        Windows::UI::Core::CoreWindow m_uiExtensionCoreWindow{ nullptr };

        winrt::event_token m_settingsClickedToken{};
        winrt::event_token m_favoritedChangedToken{};
        winrt::event_token m_displayModeChangedToken{};
        winrt::event_token m_pinnedChangedToken{};
        winrt::event_token m_themeChangedToken{};
        winrt::event_token m_visibleChangedToken{};
        winrt::event_token m_windowStateChangedToken{};
    };
}

namespace winrt::UIExtAdvSample::factory_implementation
{
    struct Extension1 : Extension1T<Extension1, implementation::Extension1>
    {
    };
}
