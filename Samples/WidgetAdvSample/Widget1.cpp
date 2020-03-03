#include "pch.h"
#include "Widget1.h"
#include "Widget1.g.cpp"

using namespace winrt;
using namespace winrt::Windows::UI::Core;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::UI::Xaml;
using namespace winrt::Windows::UI::Xaml::Navigation;
using namespace Microsoft::Gaming::XboxGameBar;

namespace winrt::WidgetAdvSample::implementation
{
    Widget1::Widget1()
    {
        InitializeComponent();
    }

    void Widget1::OnNavigatedTo(NavigationEventArgs e)
    {
        m_widget = e.Parameter().as<XboxGameBarWidget>();
        m_widgetControl = XboxGameBarWidgetControl(m_widget);

        // Hook up event that's fired when our settings button is clicked
        m_settingsToken = m_widget.SettingsClicked({ this, &Widget1::SettingsButton_Click });
        m_favoritedChangedToken = m_widget.FavoritedChanged({ this, &Widget1::FavoritedChanged });
        m_displayModeChangedToken = m_widget.GameBarDisplayModeChanged({ this, &Widget1::GameBarDisplayModeChanged });
        m_pinnedChangedToken = m_widget.PinnedChanged({ this, &Widget1::PinnedChanged });
        m_themeChangedToken = m_widget.RequestedThemeChanged({ this, &Widget1::RequestedThemeChanged });
        m_visibleChangedToken = m_widget.VisibleChanged({ this, &Widget1::VisibleChanged });
        m_windowStateChangedToken = m_widget.WindowStateChanged({ this, &Widget1::WindowStateChanged });

        hstring isPinned = m_widget.Pinned() ? L"true" : L"false";
        hstring pinnedOutput = L"Pinned: \t\t" + isPinned;
        PinnedStateTextBlock().Text(pinnedOutput);

        hstring favoriteState{};
        hstring isFavorited = m_widget.Favorited() ? L"true" : L"false";
        favoriteState = L"Favorited: \t" + isFavorited;
        FavoritedTextBlock().Text(favoriteState);

        hstring theme{};
        switch (m_widget.RequestedTheme())
        {
        case ElementTheme::Light:
            theme = L"Light";
            break;
        case ElementTheme::Dark:
            theme = L"Dark";
            break;
        default:
            theme = L"Default";
        }

        hstring requestedTheme = L"Theme: \t\t" + theme;
        RequestedThemeTextBlock().Text(requestedTheme);

        hstring isVisible = m_widget.Visible() ? L"true" : L"false";
        hstring visibleState = L"Visible: \t\t" + isVisible;

        hstring window = (m_widget.WindowState() == XboxGameBarWidgetWindowState::Minimized) ? L"Minimized\n" : L"Restored\n";
        hstring windowOutput = L"Window State: \t" + window;
        OutputDebugString(windowOutput.c_str());
    }

    IAsyncAction Widget1::ActivateAsyncAppExtIdButton_Click(IInspectable const& /*sender*/, RoutedEventArgs const& /*e*/)
    {
        hstring text = this->ActivateAsyncAppExtId().Text();
        return m_widgetControl.ActivateAsync(text);
    }

    IAsyncAction Widget1::ActivateAsyncAppIdButton_Click(IInspectable const& sender, RoutedEventArgs const& e)
    {
        co_return co_await m_widgetControl.ActivateAsync(
            this->ActivateAsyncAppId().Text(),
            this->ActivateAsyncAppExtId().Text());
    }

    IAsyncAction Widget1::ActivateWithUriAsyncButton_Click(IInspectable const& sender, RoutedEventArgs const& e)
    {
        Uri uri{ this->ActivateAsyncUri().Text() };
        co_return co_await m_widgetControl.ActivateWithUriAsync(uri);
    }

    IAsyncAction Widget1::MinimizeAsyncAppExtIdButton_Click(IInspectable const& sender, RoutedEventArgs const& e)
    {
        co_return co_await m_widgetControl.MinimizeAsync(this->ActivateAsyncAppExtId().Text());
    }

    IAsyncAction Widget1::MinimizeAsyncAppIdButton_Click(IInspectable const& sender, RoutedEventArgs const& e)
    {
        co_return co_await m_widgetControl.MinimizeAsync(
            this->ActivateAsyncAppId().Text(),
            this->ActivateAsyncAppExtId().Text());
    }

    IAsyncAction Widget1::RestoreAsyncAppExtIdButton_Click(IInspectable const& sender, RoutedEventArgs const& e)
    {
        co_return co_await m_widgetControl.RestoreAsync(this->ActivateAsyncAppExtId().Text());
    }

    IAsyncAction Widget1::RestoreAsyncAppIdButton_Click(IInspectable const& sender, RoutedEventArgs const& e)
    {
        co_return co_await m_widgetControl.RestoreAsync(
            this->ActivateAsyncAppId().Text(),
            this->ActivateAsyncAppExtId().Text());
    }

    IAsyncAction Widget1::CloseAsyncAppExtIdButton_Click(IInspectable const& sender, RoutedEventArgs const& e)
    {
        co_return co_await m_widgetControl.CloseAsync(this->ActivateAsyncAppExtId().Text());
    }

    IAsyncAction Widget1::CloseAsyncAppIdButton_Click(IInspectable const& sender, RoutedEventArgs const& e)
    {
        co_return co_await m_widgetControl.CloseAsync(
            this->ActivateAsyncAppId().Text(),
            this->ActivateAsyncAppExtId().Text());
    }

    IAsyncAction Widget1::TryResizeWindowAsync_Click(IInspectable const& sender, RoutedEventArgs const& e)
    {
        Windows::Foundation::Size size;
        size.Height = (float)_wtof(this->WindowHeightBox().Text().c_str());
        size.Width = (float)_wtof(this->WindowWidthBox().Text().c_str());
        auto result = co_await m_widget.TryResizeWindowAsync(size);
        co_return;
    }

    Windows::Foundation::IAsyncAction Widget1::SettingsButton_Click(
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Windows::Foundation::IInspectable const& e)
    {
        auto strongThis{ get_strong() };
        co_await m_widget.ActivateSettingsAsync();
    }

    winrt::fire_and_forget Widget1::FavoritedChanged(
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Windows::Foundation::IInspectable const& e)
    {
        auto strongThis{ get_strong() };

        hstring favoriteState{};
        hstring isFavorited = m_widget.Favorited() ? L"true" : L"false";
        favoriteState = L"Favorited: \t" + isFavorited;

        co_await winrt::resume_foreground(FavoritedTextBlock().Dispatcher());

        FavoritedTextBlock().Text(favoriteState);
    }

    void Widget1::GameBarDisplayModeChanged(
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Windows::Foundation::IInspectable const& e)
    {
        hstring mode = (m_widget.GameBarDisplayMode() == XboxGameBarDisplayMode::Foreground) ? L"Foreground\n" : L"PinnedOnly\n";
        hstring modeOutput = L"Game Bar View Mode: " + mode;

        OutputDebugString(modeOutput.c_str());
    }

    winrt::fire_and_forget Widget1::PinnedChanged(
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Windows::Foundation::IInspectable const& e)
    {
        auto strongThis{ get_strong() };

        hstring isPinned = m_widget.Pinned() ? L"true" : L"false";
        hstring pinnedOutput = L"Pinned: \t\t" + isPinned;

        co_await winrt::resume_foreground(PinnedStateTextBlock().Dispatcher());

        PinnedStateTextBlock().Text(pinnedOutput);
    }

    winrt::fire_and_forget  Widget1::RequestedThemeChanged(
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Windows::Foundation::IInspectable const& e)
    {
        auto strongThis{ get_strong() };
        auto value = m_widget.RequestedTheme();

        hstring theme{};
        switch (m_widget.RequestedTheme())
        {
        case ElementTheme::Light:
            theme = L"Light";
            break;
        case ElementTheme::Dark:
            theme = L"Dark";
            break;
        case ElementTheme::Default:
            theme = L"Default";
        }

        hstring requestedTheme = L"Theme: \t\t" + theme;

        co_await winrt::resume_foreground(RequestedThemeTextBlock().Dispatcher());

        RequestedThemeTextBlock().Text(requestedTheme);
    }

    void Widget1::VisibleChanged(
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Windows::Foundation::IInspectable const& e)
    {
        hstring isVisible = m_widget.Visible() ? L"true\n" : L"false\n";
        hstring visibleState = L"Visible: " + isVisible;

        OutputDebugString(visibleState.c_str());
    }

    void Widget1::WindowStateChanged(
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Windows::Foundation::IInspectable const& e)
    {
        hstring window = (m_widget.WindowState() == XboxGameBarWidgetWindowState::Minimized) ? L"Minimized\n" : L"Restored\n";
        hstring windowOutput = L"Window State: " + window;

        OutputDebugString(windowOutput.c_str());
    }
}


