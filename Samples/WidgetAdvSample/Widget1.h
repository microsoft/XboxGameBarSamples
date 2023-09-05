#pragma once

#include "Widget1.g.h"
#include <winrt/Windows.UI.Xaml.Navigation.h>
#include <winrt/Windows.UI.Xaml.Media.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Microsoft.Gaming.XboxGameBar.h>
#include <winrt/Microsoft.Gaming.XboxGameBar.Authentication.h>

namespace winrt::WidgetAdvSample::implementation
{
    struct Widget1 : Widget1T<Widget1>
    {
        Widget1();

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
        Windows::Foundation::IAsyncAction CenterWindowAsync_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        Windows::Foundation::IAsyncAction AuthenticateAsync_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        Windows::Foundation::IAsyncAction LaunchUriAsyncButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        Windows::Foundation::IAsyncAction LaunchUriAsyncAdvancedButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        Windows::Foundation::IAsyncAction StartActivityButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        Windows::Foundation::IAsyncAction StopActivityButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        Windows::Foundation::IAsyncAction ShowBasicNotification_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        Windows::Foundation::IAsyncAction ShowAdvancedNotification_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);

        void HorizontalResizeSupportedCheckBox_Checked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void HorizontalResizeSupportedCheckBox_Unchecked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void VerticalResizeSupportedCheckBox_Checked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void VerticalResizeSupportedCheckBox_Unchecked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void PinningSupportedCheckBox_Checked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void PinningSupportedCheckBox_Unchecked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void SettingsSupportedCheckBox_Checked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void SettingsSupportedCheckBox_Unchecked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void MinWindowSize_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void MaxWindowSize_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void OpacityOverride_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);

        // Settings click handler for widget settings click event
        Windows::Foundation::IAsyncAction SettingsButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::Foundation::IInspectable const& e);
        winrt::fire_and_forget FavoritedChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::Foundation::IInspectable const& e);
        void GameBarDisplayModeChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::Foundation::IInspectable const& e);
        winrt::fire_and_forget PinnedChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::Foundation::IInspectable const& e);
        winrt::fire_and_forget RequestedOpacityChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::Foundation::IInspectable const& e);
        winrt::fire_and_forget RequestedThemeChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::Foundation::IInspectable const& e);
        winrt::fire_and_forget TargetChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::Foundation::IInspectable const& e);
        void VisibleChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::Foundation::IInspectable const& e);
        void WindowStateChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::Foundation::IInspectable const& e);
        
        // Methods to handle updating of Text and UI
        void SetBackgroundColor();
        void SetBackgroundOpacity();
        void SetRequestedOpacityState();
        hstring RequestedThemeToString();
        hstring FavoritedStateToString();
        hstring PinnedStateToString();
        hstring TargetToString();
        void OutputVisibleState();
        void OutputWindowState();
        void OutputGameBarDisplayMode();

    private:
        Microsoft::Gaming::XboxGameBar::XboxGameBarWidget m_widget{ nullptr };
        Microsoft::Gaming::XboxGameBar::Authentication::XboxGameBarWebAuthenticationBroker m_gameBarWebAuth{ nullptr };
        Microsoft::Gaming::XboxGameBar::XboxGameBarWidgetControl m_widgetControl{ nullptr };
        Microsoft::Gaming::XboxGameBar::XboxGameBarWidgetActivity m_widgetActivity{ nullptr };
        Microsoft::Gaming::XboxGameBar::XboxGameBarAppTargetTracker m_appTargetTracker{ nullptr };
        Microsoft::Gaming::XboxGameBar::IXboxGameBarWidgetNotificationManager m_widgetNotificationManager{ nullptr };

        Windows::UI::Core::CoreWindow m_widgetCoreWindow{ nullptr };
        Windows::UI::Xaml::Media::SolidColorBrush m_widgetDarkThemeBrush{ nullptr };
        Windows::UI::Xaml::Media::SolidColorBrush m_widgetLightThemeBrush{ nullptr };
        std::optional<double> m_opacityOverride;

        winrt::event_token m_settingsToken{};
        winrt::event_token m_favoritedChangedToken{};
        winrt::event_token m_displayModeChangedToken{};
        winrt::event_token m_pinnedChangedToken{};
        winrt::event_token m_opacityChangedToken{};
        winrt::event_token m_themeChangedToken{};
        winrt::event_token m_visibleChangedToken{};
        winrt::event_token m_windowStateChangedToken{};
        winrt::event_token m_targetChangedToken{};
        winrt::event_token m_targetSettingChangedToken{};
    };
}

namespace winrt::WidgetAdvSample::factory_implementation
{
    struct Widget1 : Widget1T<Widget1, implementation::Widget1>
    {
    };
}
