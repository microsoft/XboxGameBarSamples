#include "pch.h"
#include "Extension1.h"
#include "Extension1.g.cpp"

using namespace winrt;
using namespace winrt::Windows::UI::Core;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::UI::Xaml;
using namespace winrt::Windows::UI::Xaml::Media;
using namespace winrt::Windows::UI::Xaml::Navigation;
using namespace Microsoft::Gaming::XboxGameBar;

namespace winrt::UIExtAdvSample::implementation
{
    Extension1::Extension1()
    {
        InitializeComponent();
    }

    void Extension1::OnNavigatedTo(NavigationEventArgs e)
    {
        m_uiExtension = e.Parameter().as<XboxGameBarUIExtension>();
        m_extensionControl = XboxGameBarExtensionControl(m_uiExtension);

        m_uiExtensionDarkThemeBrush = SolidColorBrush(Windows::UI::ColorHelper::FromArgb(255, 38, 38, 38));
        m_uiExtensionLightThemeBrush = SolidColorBrush(Windows::UI::ColorHelper::FromArgb(255, 219, 219, 219));

        // Hook up event that's fired when our settings button is clicked
        m_settingsToken = m_uiExtension.SettingsClicked({ this, &Extension1::SettingsButton_Click });
        m_favoritedChangedToken = m_uiExtension.FavoritedChanged({ this, &Extension1::FavoritedChanged });
        m_displayModeChangedToken = m_uiExtension.GameBarDisplayModeChanged({ this, &Extension1::GameBarDisplayModeChanged });
        m_pinnedChangedToken = m_uiExtension.PinnedChanged({ this, &Extension1::PinnedChanged });
        m_themeChangedToken = m_uiExtension.RequestedThemeChanged({ this, &Extension1::RequestedThemeChanged });
        m_visibleChangedToken = m_uiExtension.VisibleChanged({ this, &Extension1::VisibleChanged });
        m_windowStateChangedToken = m_uiExtension.WindowStateChanged({ this, &Extension1::WindowStateChanged });

        PinnedStateTextBlock().Text(PinnedStateToString());
        FavoritedTextBlock().Text(FavoritedStateToString());
        RequestedThemeTextBlock().Text(RequestedThemeToString());

        SetBackgroundColor();
        OutputGameBarDisplayMode();
        OutputVisibleState();
        OutputWindowState();
    }

    IAsyncAction Extension1::ActivateAsyncAppExtIdButton_Click(IInspectable const& /*sender*/, RoutedEventArgs const& /*e*/)
    {
        hstring text = this->ActivateAsyncAppExtId().Text();
        return m_extensionControl.ActivateAsync(text);
    }

    IAsyncAction Extension1::ActivateAsyncAppIdButton_Click(IInspectable const& sender, RoutedEventArgs const& e)
    {
        co_return co_await m_extensionControl.ActivateAsync(
            this->ActivateAsyncAppId().Text(),
            this->ActivateAsyncAppExtId().Text());
    }

    IAsyncAction Extension1::ActivateWithUriAsyncButton_Click(IInspectable const& sender, RoutedEventArgs const& e)
    {
        Uri uri{ this->ActivateAsyncUri().Text() };
        co_return co_await m_extensionControl.ActivateWithUriAsync(uri);
    }

    IAsyncAction Extension1::MinimizeAsyncAppExtIdButton_Click(IInspectable const& sender, RoutedEventArgs const& e)
    {
        co_return co_await m_extensionControl.MinimizeAsync(this->ActivateAsyncAppExtId().Text());
    }

    IAsyncAction Extension1::MinimizeAsyncAppIdButton_Click(IInspectable const& sender, RoutedEventArgs const& e)
    {
        co_return co_await m_extensionControl.MinimizeAsync(
            this->ActivateAsyncAppId().Text(),
            this->ActivateAsyncAppExtId().Text());
    }

    IAsyncAction Extension1::RestoreAsyncAppExtIdButton_Click(IInspectable const& sender, RoutedEventArgs const& e)
    {
        co_return co_await m_extensionControl.RestoreAsync(this->ActivateAsyncAppExtId().Text());
    }

    IAsyncAction Extension1::RestoreAsyncAppIdButton_Click(IInspectable const& sender, RoutedEventArgs const& e)
    {
        co_return co_await m_extensionControl.RestoreAsync(
            this->ActivateAsyncAppId().Text(),
            this->ActivateAsyncAppExtId().Text());
    }

    IAsyncAction Extension1::CloseAsyncAppExtIdButton_Click(IInspectable const& sender, RoutedEventArgs const& e)
    {
        co_return co_await m_extensionControl.CloseAsync(this->ActivateAsyncAppExtId().Text());
    }

    IAsyncAction Extension1::CloseAsyncAppIdButton_Click(IInspectable const& sender, RoutedEventArgs const& e)
    {
        co_return co_await m_extensionControl.CloseAsync(
            this->ActivateAsyncAppId().Text(),
            this->ActivateAsyncAppExtId().Text());
    }

    IAsyncAction Extension1::TryResizeWindowAsync_Click(IInspectable const& sender, RoutedEventArgs const& e)
    {
        Windows::Foundation::Size size;
        size.Height = (float)_wtof(this->WindowHeightBox().Text().c_str());
        size.Width = (float)_wtof(this->WindowWidthBox().Text().c_str());
        auto result = co_await m_uiExtension.TryResizeWindowAsync(size);
        co_return;
    }

    Windows::Foundation::IAsyncAction Extension1::SettingsButton_Click(
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Windows::Foundation::IInspectable const& e)
    {
        auto strongThis{ get_strong() };
        co_await m_uiExtension.ActivateSettingsAsync();
    }

    winrt::fire_and_forget Extension1::FavoritedChanged(
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Windows::Foundation::IInspectable const& e)
    {
        auto strongThis{ get_strong() };
        co_await winrt::resume_foreground(FavoritedTextBlock().Dispatcher());
        FavoritedTextBlock().Text(FavoritedStateToString());
    }

    void Extension1::GameBarDisplayModeChanged(
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Windows::Foundation::IInspectable const& e)
    {
        OutputGameBarDisplayMode();
    }

    winrt::fire_and_forget Extension1::PinnedChanged(
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Windows::Foundation::IInspectable const& e)
    {
        auto strongThis{ get_strong() };
        co_await winrt::resume_foreground(PinnedStateTextBlock().Dispatcher());
        PinnedStateTextBlock().Text(PinnedStateToString());
    }

    winrt::fire_and_forget  Extension1::RequestedThemeChanged(
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Windows::Foundation::IInspectable const& e)
    {
        auto strongThis{ get_strong() };
        co_await winrt::resume_foreground(RequestedThemeTextBlock().Dispatcher());
        RequestedThemeTextBlock().Text(RequestedThemeToString());
        SetBackgroundColor();
    }

    void Extension1::VisibleChanged(
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Windows::Foundation::IInspectable const& e)
    {
        OutputVisibleState();
    }

    void Extension1::WindowStateChanged(
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Windows::Foundation::IInspectable const& e)
    {
        OutputWindowState();
    }

    void Extension1::SetBackgroundColor()
    {
        auto requestedTheme = m_uiExtension.RequestedTheme();

        if (requestedTheme == ElementTheme::Dark)
        {
            this->RequestedTheme(requestedTheme);
            this->Background(m_uiExtensionDarkThemeBrush);
        }
        else
        {
            this->RequestedTheme(requestedTheme);
            this->Background(m_uiExtensionLightThemeBrush);
        }
    }

    hstring Extension1::RequestedThemeToString()
    {
        hstring requestedTheme{};
        switch (m_uiExtension.RequestedTheme())
        {
        case ElementTheme::Light:
            requestedTheme = L"Light";
            break;
        case ElementTheme::Dark:
            requestedTheme = L"Dark";
            break;
        default:
            requestedTheme = L"Default";
        }

        return requestedTheme;
    }

    hstring Extension1::FavoritedStateToString()
    {
        hstring isFavorited = m_uiExtension.Favorited() ? L"true" : L"false";
        return isFavorited;
    }

    hstring Extension1::PinnedStateToString()
    {
        hstring isPinned = m_uiExtension.Pinned() ? L"true" : L"false";
        return isPinned;
    }

    void Extension1::OutputVisibleState()
    {
        hstring isVisible = m_uiExtension.Visible() ? L"true\n" : L"false\n";
        hstring visibleState = L"Visible: \t\t" + isVisible;
        OutputDebugString(visibleState.c_str());
    }

    void Extension1::OutputWindowState()
    {
        hstring window = (m_uiExtension.WindowState() == XboxGameBarUIExtensionWindowState::Minimized) ? L"Minimized\n" : L"Restored\n";
        hstring windowOutput = L"Window State: \t" + window;
        OutputDebugString(windowOutput.c_str());
    }

    void Extension1::OutputGameBarDisplayMode()
    {
        hstring mode = (m_uiExtension.GameBarDisplayMode() == XboxGameBarDisplayMode::Foreground) ? L"Foreground\n" : L"PinnedOnly\n";
        hstring modeOutput = L"Game Bar View Mode: " + mode;
        OutputDebugString(modeOutput.c_str());
    }
}


