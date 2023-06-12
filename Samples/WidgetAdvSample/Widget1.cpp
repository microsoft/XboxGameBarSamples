#include "pch.h"
#include "Widget1.h"
#include "Widget1.g.cpp"

#include <sstream>
#include <strsafe.h>

using namespace winrt;
using namespace winrt::Windows::UI::Core;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::UI::Xaml;
using namespace winrt::Windows::UI::Xaml::Media;
using namespace winrt::Windows::UI::Xaml::Navigation;
using namespace winrt::Microsoft::Gaming::XboxGameBar;
using namespace winrt::Microsoft::Gaming::XboxGameBar::Authentication;

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
        m_gameBarWebAuth = XboxGameBarWebAuthenticationBroker(m_widget);

        m_widgetDarkThemeBrush = SolidColorBrush(Windows::UI::ColorHelper::FromArgb(255, 38, 38, 38));
        m_widgetLightThemeBrush = SolidColorBrush(Windows::UI::ColorHelper::FromArgb(255, 219, 219, 219));

        // Hook up event that's fired when our settings button is clicked
        m_settingsToken = m_widget.SettingsClicked({ this, &Widget1::SettingsButton_Click });
        m_favoritedChangedToken = m_widget.FavoritedChanged({ this, &Widget1::FavoritedChanged });
        m_displayModeChangedToken = m_widget.GameBarDisplayModeChanged({ this, &Widget1::GameBarDisplayModeChanged });
        m_opacityChangedToken = m_widget.RequestedOpacityChanged({ this, &Widget1::RequestedOpacityChanged });
        m_pinnedChangedToken = m_widget.PinnedChanged({ this, &Widget1::PinnedChanged });
        m_themeChangedToken = m_widget.RequestedThemeChanged({ this, &Widget1::RequestedThemeChanged });
        m_visibleChangedToken = m_widget.VisibleChanged({ this, &Widget1::VisibleChanged });
        m_windowStateChangedToken = m_widget.WindowStateChanged({ this, &Widget1::WindowStateChanged });

        PinnedStateTextBlock().Text(PinnedStateToString());
        FavoritedTextBlock().Text(FavoritedStateToString());
        SetRequestedOpacityState();
        RequestedThemeTextBlock().Text(RequestedThemeToString());
        OutputVisibleState();
        OutputWindowState();
        OutputGameBarDisplayMode();
        SetBackgroundColor();
        SetBackgroundOpacity();

        HorizontalResizeSupportedCheckBox().IsChecked(m_widget.HorizontalResizeSupported());
        VerticalResizeSupportedCheckBox().IsChecked(m_widget.VerticalResizeSupported());
        PinningSupportedCheckBox().IsChecked(m_widget.PinningSupported());
        SettingsSupportedCheckBox().IsChecked(m_widget.SettingsSupported());

        wchar_t buffer[256];
        StringCchPrintfW(buffer, ARRAYSIZE(buffer), L"%g", m_widget.MinWindowSize().Height);
        MinWindowHeightBox().Text(buffer);
        StringCchPrintfW(buffer, ARRAYSIZE(buffer), L"%g", m_widget.MinWindowSize().Width);
        MinWindowWidthBox().Text(buffer);
        StringCchPrintfW(buffer, ARRAYSIZE(buffer), L"%g", m_widget.MaxWindowSize().Height);
        MaxWindowHeightBox().Text(buffer);
        StringCchPrintfW(buffer, ARRAYSIZE(buffer), L"%g", m_widget.MaxWindowSize().Width);
        MaxWindowWidthBox().Text(buffer);
    }

    IAsyncAction Widget1::ActivateAsyncAppExtIdButton_Click(IInspectable const& /*sender*/, RoutedEventArgs const& /*e*/)
    {
        hstring text = this->ActivateAsyncAppExtId().Text();
        return m_widgetControl.ActivateAsync(text);
    }

    IAsyncAction Widget1::ActivateAsyncAppIdButton_Click(IInspectable const& /*sender*/, RoutedEventArgs const& /*e*/)
    {
        co_return co_await m_widgetControl.ActivateAsync(
            this->ActivateAsyncAppId().Text(),
            this->ActivateAsyncAppExtId().Text());
    }

    IAsyncAction Widget1::ActivateWithUriAsyncButton_Click(IInspectable const& /*sender*/, RoutedEventArgs const& /*e*/)
    {
        Uri uri{ this->ActivateAsyncUri().Text() };
        co_return co_await m_widgetControl.ActivateWithUriAsync(uri);
    }

    IAsyncAction Widget1::MinimizeAsyncAppExtIdButton_Click(IInspectable const& /*sender*/, RoutedEventArgs const& /*e*/)
    {
        co_return co_await m_widgetControl.MinimizeAsync(this->ActivateAsyncAppExtId().Text());
    }

    IAsyncAction Widget1::MinimizeAsyncAppIdButton_Click(IInspectable const& /*sender*/, RoutedEventArgs const& /*e*/)
    {
        co_return co_await m_widgetControl.MinimizeAsync(
            this->ActivateAsyncAppId().Text(),
            this->ActivateAsyncAppExtId().Text());
    }

    IAsyncAction Widget1::RestoreAsyncAppExtIdButton_Click(IInspectable const& /*sender*/, RoutedEventArgs const& /*e*/)
    {
        co_return co_await m_widgetControl.RestoreAsync(this->ActivateAsyncAppExtId().Text());
    }

    IAsyncAction Widget1::RestoreAsyncAppIdButton_Click(IInspectable const& /*sender*/, RoutedEventArgs const& /*e*/)
    {
        co_return co_await m_widgetControl.RestoreAsync(
            this->ActivateAsyncAppId().Text(),
            this->ActivateAsyncAppExtId().Text());
    }

    IAsyncAction Widget1::CloseAsyncAppExtIdButton_Click(IInspectable const& /*sender*/, RoutedEventArgs const& /*e*/)
    {
        co_return co_await m_widgetControl.CloseAsync(this->ActivateAsyncAppExtId().Text());
    }

    IAsyncAction Widget1::CloseAsyncAppIdButton_Click(IInspectable const& /*sender*/, RoutedEventArgs const& /*e*/)
    {
        co_return co_await m_widgetControl.CloseAsync(
            this->ActivateAsyncAppId().Text(),
            this->ActivateAsyncAppExtId().Text());
    }

    IAsyncAction Widget1::TryResizeWindowAsync_Click(IInspectable const& /*sender*/, RoutedEventArgs const& /*e*/)
    {
        Windows::Foundation::Size size;
        size.Height = (float)_wtof(this->WindowHeightBox().Text().c_str());
        size.Width = (float)_wtof(this->WindowWidthBox().Text().c_str());
        auto result = co_await m_widget.TryResizeWindowAsync(size);
        UNREFERENCED_PARAMETER(result);
        co_return;
    }

    IAsyncAction Widget1::CenterWindowAsync_Click(IInspectable const& /*sender*/, RoutedEventArgs const& /*e*/)
    {        
        co_return co_await m_widget.CenterWindowAsync();
    }

    IAsyncAction Widget1::AuthenticateAsync_Click(IInspectable const& /*sender*/, RoutedEventArgs const& /*e*/)
    {
        if (RequestUriBox().Text().empty() || CallbackUriBox().Text().empty())
        {
            co_return;
        }

        auto strongThis{ get_strong() };

        Uri requestUri{ RequestUriBox().Text() };
        Uri callbackUri{ CallbackUriBox().Text() };
        XboxGameBarWebAuthenticationResult result = co_await m_gameBarWebAuth.AuthenticateAsync(
            XboxGameBarWebAuthenticationOptions::None,
            requestUri,
            callbackUri);

        std::wstring debugOut = L"ResponseData: ";
        debugOut += result.ResponseData();
        debugOut += L"\r\n";
        OutputDebugString(debugOut.c_str());

        wchar_t buffer[256] = {};
        StringCchPrintfW(buffer, ARRAYSIZE(buffer), L"ResponseStatus: %u\r\n", (uint32_t)result.ResponseStatus());
        OutputDebugString(buffer);

        StringCchPrintfW(buffer, ARRAYSIZE(buffer), L"ResponseErrorDetail: %u\r\n", result.ResponseErrorDetail());
        OutputDebugString(buffer);
    }

    IAsyncAction Widget1::LaunchUriAsyncButton_Click(IInspectable const& /*sender*/, RoutedEventArgs const& /*e*/)
    {
        Uri uri{ this->LaunchUriAsyncText().Text() };
        bool result = co_await m_widget.LaunchUriAsync(uri);
        if (!result)
        {
            OutputDebugStringW(L"LaunchUriAsync returned false");
        }
        co_return;
    }

    IAsyncAction Widget1::StartActivityButton_Click(IInspectable const& /*sender*/, RoutedEventArgs const& /*e*/)
    {
        if (!m_widgetActivity)
        {
            try
            {
                // Throws if given activityId already exists
                m_widgetActivity = XboxGameBarWidgetActivity::Create(m_widget, L"uniqueActivityId");
            }
            catch (hresult_error error)
            {
                OutputDebugStringW(L"Activity::Create failed");
            }
        }
        co_return;
    }

    IAsyncAction Widget1::StopActivityButton_Click(IInspectable const& /*sender*/, RoutedEventArgs const& /*e*/)
    {
        if (m_widgetActivity)
        {
            m_widgetActivity.Complete();
            m_widgetActivity = nullptr;
        }
        co_return;
    }

    void Widget1::HorizontalResizeSupportedCheckBox_Checked(IInspectable const& /*sender*/, RoutedEventArgs const& /*e*/)
    {
        m_widget.HorizontalResizeSupported(true);
    }

    void Widget1::HorizontalResizeSupportedCheckBox_Unchecked(IInspectable const& /*sender*/, RoutedEventArgs const& /*e*/)
    {
        m_widget.HorizontalResizeSupported(false);
    }

    void Widget1::VerticalResizeSupportedCheckBox_Checked(IInspectable const& /*sender*/, RoutedEventArgs const& /*e*/)
    {
        m_widget.VerticalResizeSupported(true);
    }

    void Widget1::VerticalResizeSupportedCheckBox_Unchecked(IInspectable const& /*sender*/, RoutedEventArgs const& /*e*/)
    {
        m_widget.VerticalResizeSupported(false);
    }

    void Widget1::PinningSupportedCheckBox_Checked(IInspectable const& /*sender*/, RoutedEventArgs const& /*e*/)
    {
        m_widget.PinningSupported(true);
    }

    void Widget1::PinningSupportedCheckBox_Unchecked(IInspectable const& /*sender*/, RoutedEventArgs const& /*e*/)
    {
        m_widget.PinningSupported(false);
    }

    void Widget1::SettingsSupportedCheckBox_Checked(IInspectable const& /*sender*/, RoutedEventArgs const& /*e*/)
    {
        m_widget.SettingsSupported(true);
    }

    void Widget1::SettingsSupportedCheckBox_Unchecked(IInspectable const& /*sender*/, RoutedEventArgs const& /*e*/)
    {
        m_widget.SettingsSupported(false);
    }

    void Widget1::MinWindowSize_Click(IInspectable const& /*sender*/, RoutedEventArgs const& /*e*/)
    {
        Windows::Foundation::Size size;
        size.Height = (float)_wtof(this->MinWindowHeightBox().Text().c_str());
        size.Width = (float)_wtof(this->MinWindowWidthBox().Text().c_str());
        m_widget.MinWindowSize(size);
    }

    void Widget1::MaxWindowSize_Click(IInspectable const& /*sender*/, RoutedEventArgs const& /*e*/)
    {
        Windows::Foundation::Size size;
        size.Height = (float)_wtof(this->MaxWindowHeightBox().Text().c_str());
        size.Width = (float)_wtof(this->MaxWindowWidthBox().Text().c_str());
        m_widget.MaxWindowSize(size);
    }

    void Widget1::OpacityOverride_Click(IInspectable const& /*sender*/, RoutedEventArgs const& /*e*/)
    {
        std::wstring_view str{ this->OpacityOverrideBox().Text() };
        double opacityOverride{ 1 };

        if (!str.empty())
        {
            opacityOverride = _wtof(str.data());
        }

        // - Value cannot be empty
        // - Value cannot be non-numeric (wtof returns 0)
        // - Value must be between 0 and 1.
        if (   str.empty() 
            || (opacityOverride == 0 && (0 != str.compare(L"0")) && (0 != str.compare(L"0.0")))
            || (opacityOverride < 0)
            || (opacityOverride > 1))
        {
            // Invalid
            this->OpacityOverrideBox().Text(L"");
            m_opacityOverride.reset();
        }
        else
        {
            // Valid
            m_opacityOverride = opacityOverride;
        }

        SetRequestedOpacityState();
        SetBackgroundOpacity();
    }

    IAsyncAction Widget1::SettingsButton_Click(IInspectable const& /*sender*/, IInspectable const& /*e*/)
    {
        auto strongThis{ get_strong() };
        co_await m_widget.ActivateSettingsAsync();
    }

    fire_and_forget Widget1::FavoritedChanged(IInspectable const& /*sender*/, IInspectable const& /*e*/)
    {
        auto strongThis{ get_strong() };
        co_await resume_foreground(FavoritedTextBlock().Dispatcher());
        FavoritedTextBlock().Text(FavoritedStateToString());
    }

    void Widget1::GameBarDisplayModeChanged(IInspectable const& /*sender*/, IInspectable const& /*e*/)
    {
        OutputGameBarDisplayMode();
    }

    fire_and_forget Widget1::PinnedChanged(IInspectable const& /*sender*/, IInspectable const& /*e*/)
    {
        auto strongThis{ get_strong() };
        co_await resume_foreground(PinnedStateTextBlock().Dispatcher());
        PinnedStateTextBlock().Text(PinnedStateToString());
    }

    fire_and_forget Widget1::RequestedOpacityChanged(IInspectable const& /*sender*/, IInspectable const& /*e*/)
    {
        auto strongThis{ get_strong() };
        if (!m_opacityOverride.has_value())
        {
            co_await resume_foreground(BackgroundGrid().Dispatcher());
            SetRequestedOpacityState();
            SetBackgroundOpacity();
        }
    }

    fire_and_forget  Widget1::RequestedThemeChanged(IInspectable const& /*sender*/, IInspectable const& /*e*/)
    {
        auto strongThis{ get_strong() };
        co_await resume_foreground(RequestedThemeTextBlock().Dispatcher());
        RequestedThemeTextBlock().Text(RequestedThemeToString());
        SetBackgroundColor();
    }

    void Widget1::VisibleChanged(IInspectable const& /*sender*/, IInspectable const& /*e*/)
    {
        OutputVisibleState();
    }

    void Widget1::WindowStateChanged(IInspectable const& /*sender*/, IInspectable const& /*e*/)
    {
        OutputWindowState();
    }

    void Widget1::SetBackgroundColor()
    {
        auto requestedTheme = m_widget.RequestedTheme();

        if (requestedTheme == ElementTheme::Dark)
        {
            this->RequestedTheme(requestedTheme);
            BackgroundGrid().Background(m_widgetDarkThemeBrush);
        }
        else
        {
            this->RequestedTheme(requestedTheme);
            this->BackgroundGrid().Background(m_widgetLightThemeBrush);
        }

        BackgroundGrid().Opacity(m_widget.RequestedOpacity());
    }

    void Widget1::SetBackgroundOpacity()
    {
        if (m_opacityOverride.has_value())
        {
            BackgroundGrid().Opacity(m_opacityOverride.value());
        }
        else
        {
            BackgroundGrid().Opacity(m_widget.RequestedOpacity());
        }
    }

    void Widget1::SetRequestedOpacityState()
    {
        auto requestedOpacity = m_widget.RequestedOpacity();
        if (m_opacityOverride.has_value())
        {
            requestedOpacity = m_opacityOverride.value();
        }

        std::wstringstream opacityStringStream;
        opacityStringStream << requestedOpacity;

        RequestedOpacityTextBlock().Text(opacityStringStream.str().c_str());
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