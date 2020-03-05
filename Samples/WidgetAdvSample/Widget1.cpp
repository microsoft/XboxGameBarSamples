#include "pch.h"
#include "Widget1.h"
#include "Widget1.g.cpp"

using namespace winrt;
using namespace winrt::Windows::UI::Core;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::UI::Xaml;
using namespace winrt::Windows::UI::Xaml::Media;
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

        m_widgetDarkThemeBrush = SolidColorBrush(Windows::UI::ColorHelper::FromArgb(255, 38, 38, 38));
        m_widgetLightThemeBrush = SolidColorBrush(Windows::UI::ColorHelper::FromArgb(255, 219, 219, 219));

        // Hook up event that's fired when our settings button is clicked
        m_settingsToken = m_widget.SettingsClicked({ this, &Widget1::SettingsButton_Click });
        m_favoritedChangedToken = m_widget.FavoritedChanged({ this, &Widget1::FavoritedChanged });
        m_displayModeChangedToken = m_widget.GameBarDisplayModeChanged({ this, &Widget1::GameBarDisplayModeChanged });
        m_pinnedChangedToken = m_widget.PinnedChanged({ this, &Widget1::PinnedChanged });
        m_themeChangedToken = m_widget.RequestedThemeChanged({ this, &Widget1::RequestedThemeChanged });
        m_visibleChangedToken = m_widget.VisibleChanged({ this, &Widget1::VisibleChanged });
        m_windowStateChangedToken = m_widget.WindowStateChanged({ this, &Widget1::WindowStateChanged });

        PinnedStateTextBlock().Text(PinnedStateToString());
        FavoritedTextBlock().Text(FavoritedStateToString());
        RequestedThemeTextBlock().Text(RequestedThemeToString());

        SetBackgroundColor();
        OutputGameBarDisplayMode();
        OutputVisibleState();
        OutputWindowState();
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
        UNREFERENCED_PARAMETER(result);
        co_return;
    }

    Windows::Foundation::IAsyncAction Widget1::SettingsButton_Click(
        winrt::Windows::Foundation::IInspectable const& /*sender*/,
        winrt::Windows::Foundation::IInspectable const& /*e*/)
    {
        auto strongThis{ get_strong() };
        co_await m_widget.ActivateSettingsAsync();
    }

    winrt::fire_and_forget Widget1::FavoritedChanged(
        winrt::Windows::Foundation::IInspectable const& /*sender*/,
        winrt::Windows::Foundation::IInspectable const& /*e*/)
    {
        auto strongThis{ get_strong() };
        co_await winrt::resume_foreground(FavoritedTextBlock().Dispatcher());
        FavoritedTextBlock().Text(FavoritedStateToString());
    }

    void Widget1::GameBarDisplayModeChanged(
        winrt::Windows::Foundation::IInspectable const& /*sender*/,
        winrt::Windows::Foundation::IInspectable const& /*e*/)
    {
        OutputGameBarDisplayMode();
    }

    winrt::fire_and_forget Widget1::PinnedChanged(
        winrt::Windows::Foundation::IInspectable const& /*sender*/,
        winrt::Windows::Foundation::IInspectable const& /*e*/)
    {
        auto strongThis{ get_strong() };
        co_await winrt::resume_foreground(PinnedStateTextBlock().Dispatcher());
        PinnedStateTextBlock().Text(PinnedStateToString());
    }

    winrt::fire_and_forget  Widget1::RequestedThemeChanged(
        winrt::Windows::Foundation::IInspectable const& /*sender*/,
        winrt::Windows::Foundation::IInspectable const& /*e*/)
    {
        auto strongThis{ get_strong() };
        co_await winrt::resume_foreground(RequestedThemeTextBlock().Dispatcher());
        RequestedThemeTextBlock().Text(RequestedThemeToString());
        SetBackgroundColor();
    }

    void Widget1::VisibleChanged(
        winrt::Windows::Foundation::IInspectable const& /*sender*/,
        winrt::Windows::Foundation::IInspectable const& /*e*/)
    {
        OutputVisibleState();
    }

    void Widget1::WindowStateChanged(
        winrt::Windows::Foundation::IInspectable const& /*sender*/,
        winrt::Windows::Foundation::IInspectable const& /*e*/)
    {
        OutputWindowState();
    }

    void Widget1::SetBackgroundColor()
    {
        auto requestedTheme = m_widget.RequestedTheme();

        if (requestedTheme == ElementTheme::Dark)
        {
            this->RequestedTheme(requestedTheme);
            this->Background(m_widgetDarkThemeBrush);
        }
        else
        {
            this->RequestedTheme(requestedTheme);
            this->Background(m_widgetLightThemeBrush);
        }
    }

    hstring Widget1::RequestedThemeToString()
    {
        hstring requestedTheme{};
        switch (m_widget.RequestedTheme())
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

    hstring Widget1::FavoritedStateToString()
    {
        hstring isFavorited = m_widget.Favorited() ? L"true" : L"false";
        return isFavorited;
    }

    hstring Widget1::PinnedStateToString()
    {
        hstring isPinned = m_widget.Pinned() ? L"true" : L"false";
        return isPinned;
    }

    void Widget1::OutputVisibleState()
    {
        hstring isVisible = m_widget.Visible() ? L"true\n" : L"false\n";
        hstring visibleState = L"Visible: \t\t" + isVisible;
        OutputDebugString(visibleState.c_str());
    }

    void Widget1::OutputWindowState()
    {
        hstring window = (m_widget.WindowState() == XboxGameBarWidgetWindowState::Minimized) ? L"Minimized\n" : L"Restored\n";
        hstring windowOutput = L"Window State: \t" + window;
        OutputDebugString(windowOutput.c_str());
    }

    void Widget1::OutputGameBarDisplayMode()
    {
        hstring mode = (m_widget.GameBarDisplayMode() == XboxGameBarDisplayMode::Foreground) ? L"Foreground\n" : L"PinnedOnly\n";
        hstring modeOutput = L"Game Bar View Mode: " + mode;
        OutputDebugString(modeOutput.c_str());
    }
}