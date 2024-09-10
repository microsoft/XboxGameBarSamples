#include "pch.h"
#include "Widget1.h"
#include "Widget1.g.cpp"

// Create namespace aliases to clean up code without doing "using namespace <namespace>"
// Using a namespace makes it hard to know where types come from unless you're using intellisense.
// When looking at code below, you can map the alias back to the namespace where the type comes from
// without intellisense.
//
// When you copy the sample code, copy these aliases too. If you don't like the aliases, do a find/replace
// on the ::<alias>:: with ::<namespace>::
namespace winrt
{
    namespace WF = Windows::Foundation;
    namespace WFC = Windows::Foundation::Collections;
    namespace WS = Windows::System;
    namespace WUI = Windows::UI;
    namespace WUIC = Windows::UI::Core;
    namespace WUIX = Windows::UI::Xaml;
    namespace WUIXI = Windows::UI::Xaml::Input;
    namespace WUIXM = Windows::UI::Xaml::Media;
    namespace WUIXN = Windows::UI::Xaml::Navigation;
    namespace MGX = Microsoft::Gaming::XboxGameBar;
    namespace MGXI = Microsoft::Gaming::XboxGameBar::Input;
    namespace MGXA = Microsoft::Gaming::XboxGameBar::Authentication;
}

namespace winrt::WidgetAdvSample::implementation
{
    Widget1::Widget1()
    {
        InitializeComponent();
    }

    void Widget1::OnNavigatedTo(winrt::WUIXN::NavigationEventArgs const& e)
    {
        m_widget = e.Parameter().as<winrt::MGX::XboxGameBarWidget>();
        m_widgetControl = winrt::MGX::XboxGameBarWidgetControl(m_widget);
        m_gameBarWebAuth = winrt::MGXA::XboxGameBarWebAuthenticationBroker(m_widget);
        m_appTargetTracker = winrt::MGX::XboxGameBarAppTargetTracker(m_widget);
        m_widgetNotificationManager = winrt::MGX::XboxGameBarWidgetNotificationManager(m_widget);

        m_widgetDarkThemeBrush = winrt::WUIXM::SolidColorBrush(winrt::WUI::ColorHelper::FromArgb(255, 38, 38, 38));
        m_widgetLightThemeBrush = winrt::WUIXM::SolidColorBrush(winrt::WUI::ColorHelper::FromArgb(255, 219, 219, 219));

        // Hook up event that's fired when our settings button is clicked
        m_favoritedChangedToken = m_widget.FavoritedChanged({ this, &Widget1::FavoritedChanged });
        m_displayModeChangedToken = m_widget.GameBarDisplayModeChanged({ this, &Widget1::GameBarDisplayModeChanged });
        m_pinnedChangedToken = m_widget.PinnedChanged({ this, &Widget1::PinnedChanged });
        m_opacityChangedToken = m_widget.RequestedOpacityChanged({ this, &Widget1::RequestedOpacityChanged });
        m_themeChangedToken = m_widget.RequestedThemeChanged({ this, &Widget1::RequestedThemeChanged });
        m_settingsToken = m_widget.SettingsClicked({ this, &Widget1::SettingsButton_Click });
        m_visibleChangedToken = m_widget.VisibleChanged({ this, &Widget1::VisibleChanged });
        m_windowBoundsChangedToken = m_widget.WindowBoundsChanged({ this, &Widget1::WindowBoundsChanged });
        m_windowStateChangedToken = m_widget.WindowStateChanged({ this, &Widget1::WindowStateChanged });
        m_compactModeEnabledChangedToken = m_widget.CompactModeEnabledChanged({ this, &Widget1::GameBarCompactModeEnabledChanged });
        m_targetSettingChangedToken = m_appTargetTracker.SettingChanged({ this, &Widget1::TargetChanged });
        m_notificationSettingChangedToken = m_widgetNotificationManager.SettingChanged({ this, &Widget1::NotificiationSettingChanged });

        // Check that target tracker is enabled for the widget before subscribing for change events
        if (m_appTargetTracker.Setting() == Microsoft::Gaming::XboxGameBar::XboxGameBarAppTargetSetting::Enabled)
        {
            m_targetChangedToken = m_appTargetTracker.TargetChanged({ this, &Widget1::TargetChanged });
        }

        FavoritedTextBlock().Text(FavoritedStateToString());
        SetGameBarDisplayMode();
        PinnedStateTextBlock().Text(PinnedStateToString());
        SetRequestedOpacityState();
        SetBackgroundOpacity();
        SetBackgroundColor();
        RequestedThemeTextBlock().Text(RequestedThemeToString());
        OutputVisibleState();
        OutputWindowState();
        SetTargetInfo();
        SetCompactModeEnabled();

        HorizontalResizeSupportedCheckBox().IsChecked(m_widget.HorizontalResizeSupported());
        VerticalResizeSupportedCheckBox().IsChecked(m_widget.VerticalResizeSupported());
        PinningSupportedCheckBox().IsChecked(m_widget.PinningSupported());
        SettingsSupportedCheckBox().IsChecked(m_widget.SettingsSupported());

        MinWindowHeightBox().Text(winrt::to_hstring(m_widget.MinWindowSize().Height));
        MinWindowWidthBox().Text(winrt::to_hstring(m_widget.MinWindowSize().Width));
        MaxWindowHeightBox().Text(winrt::to_hstring(m_widget.MaxWindowSize().Height));
        MaxWindowWidthBox().Text(winrt::to_hstring(m_widget.MaxWindowSize().Width));

        NotificiationSettingTextBlock().Text(
            m_widgetNotificationManager.Setting() == winrt::MGX::XboxGameBarWidgetNotificationSetting::Enabled ?
            L"Enabled" : L"DisabledByUser");

        // Setup hotkey watcher for CTRL + ALT + T
        auto keys{ winrt::single_threaded_vector<winrt::Windows::System::VirtualKey>(
        {
            winrt::WS::VirtualKey::Control,
            winrt::WS::VirtualKey::Menu, // ALT
            winrt::WS::VirtualKey::T
        }) };
        m_hotkeyWatcher = winrt::MGXI::XboxGameBarHotkeyWatcher::CreateWatcher(m_widget, keys.GetView());
        m_hotkeySetChangedToken = m_hotkeyWatcher.HotkeySetStateChanged({ this, &Widget1::HotkeySetStateChanged });
    }

    winrt::WF::IAsyncAction Widget1::ActivateAsyncAppExtIdButton_Click(winrt::WF::IInspectable /*sender*/, winrt::WUIX::RoutedEventArgs /*e*/)
    {
        hstring text = ActivateAsyncAppExtId().Text();
        return m_widgetControl.ActivateAsync(text);
    }

    winrt::WF::IAsyncAction Widget1::ActivateAsyncAppIdButton_Click(winrt::WF::IInspectable /*sender*/, winrt::WUIX::RoutedEventArgs /*e*/)
    {
        co_return co_await m_widgetControl.ActivateAsync(
            ActivateAsyncAppId().Text(),
            ActivateAsyncAppExtId().Text());
    }

    winrt::WF::IAsyncAction Widget1::ActivateWithUriAsyncButton_Click(winrt::WF::IInspectable /*sender*/, winrt::WUIX::RoutedEventArgs /*e*/)
    {
        winrt::WF::Uri uri{ ActivateAsyncUri().Text() };
        co_return co_await m_widgetControl.ActivateWithUriAsync(uri);
    }

    winrt::WF::IAsyncAction Widget1::MinimizeAsyncAppExtIdButton_Click(winrt::WF::IInspectable /*sender*/, winrt::WUIX::RoutedEventArgs /*e*/)
    {
        co_return co_await m_widgetControl.MinimizeAsync(ActivateAsyncAppExtId().Text());
    }

    winrt::WF::IAsyncAction Widget1::MinimizeAsyncAppIdButton_Click(winrt::WF::IInspectable /*sender*/, winrt::WUIX::RoutedEventArgs /*e*/)
    {
        co_return co_await m_widgetControl.MinimizeAsync(
            ActivateAsyncAppId().Text(),
            ActivateAsyncAppExtId().Text());
    }

    winrt::WF::IAsyncAction Widget1::RestoreAsyncAppExtIdButton_Click(winrt::WF::IInspectable /*sender*/, winrt::WUIX::RoutedEventArgs /*e*/)
    {
        co_return co_await m_widgetControl.RestoreAsync(ActivateAsyncAppExtId().Text());
    }

    winrt::WF::IAsyncAction Widget1::RestoreAsyncAppIdButton_Click(winrt::WF::IInspectable /*sender*/, winrt::WUIX::RoutedEventArgs /*e*/)
    {
        co_return co_await m_widgetControl.RestoreAsync(
            ActivateAsyncAppId().Text(),
            ActivateAsyncAppExtId().Text());
    }

    winrt::WF::IAsyncAction Widget1::CloseAsyncAppExtIdButton_Click(winrt::WF::IInspectable /*sender*/, winrt::WUIX::RoutedEventArgs /*e*/)
    {
        co_return co_await m_widgetControl.CloseAsync(ActivateAsyncAppExtId().Text());
    }

    winrt::WF::IAsyncAction Widget1::CloseAsyncAppIdButton_Click(winrt::WF::IInspectable /*sender*/, winrt::WUIX::RoutedEventArgs /*e*/)
    {
        co_return co_await m_widgetControl.CloseAsync(
            ActivateAsyncAppId().Text(),
            ActivateAsyncAppExtId().Text());
    }

    winrt::WF::IAsyncAction Widget1::TryResizeWindowAsync_Click(winrt::WF::IInspectable /*sender*/, winrt::WUIX::RoutedEventArgs /*e*/)
    {
        Windows::Foundation::Size size;
        size.Height = (float)_wtof(WindowHeightBox().Text().c_str());
        size.Width = (float)_wtof(WindowWidthBox().Text().c_str());
        auto result = co_await m_widget.TryResizeWindowAsync(size);
        UNREFERENCED_PARAMETER(result);
        co_return;
    }

    winrt::WF::IAsyncAction Widget1::CenterWindowAsync_Click(winrt::WF::IInspectable /*sender*/, winrt::WUIX::RoutedEventArgs /*e*/)
    {
        co_return co_await m_widget.CenterWindowAsync();
    }

    winrt::WF::IAsyncAction Widget1::AuthenticateAsync_Click(winrt::WF::IInspectable /*sender*/, winrt::WUIX::RoutedEventArgs /*e*/)
    {
        if (RequestUriBox().Text().empty() || CallbackUriBox().Text().empty())
        {
            co_return;
        }

        auto strongThis{ get_strong() };

        winrt::WF::Uri requestUri{ RequestUriBox().Text() };
        winrt::WF::Uri callbackUri{ CallbackUriBox().Text() };
        winrt::MGXA::XboxGameBarWebAuthenticationResult result = co_await m_gameBarWebAuth.AuthenticateAsync(
            winrt::MGXA::XboxGameBarWebAuthenticationOptions::None,
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

    winrt::WF::IAsyncAction Widget1::LaunchUriAsyncButton_Click(winrt::WF::IInspectable /*sender*/, winrt::WUIX::RoutedEventArgs /*e*/)
    {
        winrt::WF::Uri uri{ LaunchUriAsyncText().Text() };
        bool result = co_await m_widget.LaunchUriAsync(uri);
        if (!result)
        {
            OutputDebugStringW(L"LaunchUriAsync returned false");
        }
        co_return;
    }

    winrt::WF::IAsyncAction Widget1::LaunchUriAsyncAdvancedButton_Click(winrt::WF::IInspectable /*sender*/, winrt::WUIX::RoutedEventArgs /*e*/)
    {
        winrt::WF::Uri uri{ LaunchUriAsyncText().Text() };

        winrt::WS::LauncherOptions options;
        options.TargetApplicationPackageFamilyName(L"Microsoft.WindowsCalculator_8wekyb3d8bbwe");

        winrt::WFC::ValueSet inputData;
        inputData.Insert(L"testKey1", winrt::box_value<bool>(false));
        inputData.Insert(L"testKey2", winrt::box_value<bool>(true));

        bool result = co_await m_widget.LaunchUriAsync(uri, options, inputData);
        if (!result)
        {
            OutputDebugStringW(L"LaunchUriAsync advanced returned false");
        }
        co_return;
    }

    winrt::WF::IAsyncAction Widget1::StartActivityButton_Click(winrt::WF::IInspectable /*sender*/, winrt::WUIX::RoutedEventArgs /*e*/)
    {
        if (!m_widgetActivity)
        {
            try
            {
                // Throws if given activityId already exists
                m_widgetActivity = winrt::MGX::XboxGameBarWidgetActivity(m_widget, L"uniqueActivityId");
            }
            catch (hresult_error error)
            {
                OutputDebugStringW(L"XboxGameBarWidgetActivity init failed");
            }
        }
        co_return;
    }

    winrt::WF::IAsyncAction Widget1::StopActivityButton_Click(winrt::WF::IInspectable /*sender*/, winrt::WUIX::RoutedEventArgs /*e*/)
    {
        if (m_widgetActivity)
        {
            m_widgetActivity.Complete();
            m_widgetActivity = nullptr;
        }
        co_return;
    }

    winrt::WF::IAsyncAction Widget1::ShowBasicNotification_Click(winrt::WF::IInspectable /*sender*/, winrt::WUIX::RoutedEventArgs /*e*/)
    {
        auto widgetNotification{ winrt::MGX::XboxGameBarWidgetNotificationBuilder(L"This is a toast title")
            .PrimaryImageFromPublicFolder(L"SamplePrimaryImage.png", winrt::MGX::XboxGameBarWidgetNotificationImageCrop::Circle)
            .BuildNotification() };

        // Call async show to unwind calling thread while result is obtained
        // Success means toast was succesfully shown or queued to be shown
        auto result = co_await m_widgetNotificationManager.TryShowAsync(widgetNotification);
        UNREFERENCED_PARAMETER(result);

        co_return;
    }

    winrt::WF::IAsyncAction Widget1::ShowAdvancedNotification_Click(winrt::WF::IInspectable /*sender*/, winrt::WUIX::RoutedEventArgs /*e*/)
    {
        // Check if user has enabled notifications for this widget in Game Bar settings
        // prior to attempting to show notification
        if (m_widgetNotificationManager.Setting() == Microsoft::Gaming::XboxGameBar::XboxGameBarWidgetNotificationSetting::Enabled)
        {
            auto widgetNotification{
                // (Required) Short toast title
                winrt::MGX::XboxGameBarWidgetNotificationBuilder(L"This is a toast title that can wrap to two lines if necessary")
                // (Optional) Short toast description
                .Content(L"This is a toast description that can wrap to two lines if necessary")
                // (Optional) Widget activation payload on toast activation
                .ActivationPayload(L"id=testId&context=additionalContext")
                // (Optional) Primary image
                // URI scheme: Call .PrimaryImageFromUri(...)
                // File name or relative path scheme: .PrimaryImageFromPublicFolder(...)
                // Note: using both schemes both will cause an exception
                .PrimaryImageFromPublicFolder(L"SamplePrimaryImage.png", winrt::MGX::XboxGameBarWidgetNotificationImageCrop::Default)
                // (Optional) Secondary image - same as above
                // Note: secondary image always shown with circle image crop type
                .SecondaryImageFromPublicFolder(L"SampleSecondaryImage.png")
                // (Optional) Sound - specify file name or relative path
                .SoundFromPublicFolder(L"Sounds\\SampleSound.mp3")
                // (Optional) Ignore Windows quiet hours setting for important, time sensitive notifications
                .IgnoreQuietHours(true)
                // (Optional) Avoid bringing Game Bar to the foreground for external toast activations
                .IsBackgroundActivation(false)
                // Call this to build the notification object to pass to the notification manager
                .BuildNotification() };

            // Call sync show and wait for result
            // Success means toast was succesfully shown or queued to be shown
            co_await resume_background();
            auto result = m_widgetNotificationManager.TryShow(widgetNotification);
            UNREFERENCED_PARAMETER(result);
        }
        else
        {
            OutputDebugString(L"Cannot show notificaiton due to user setting.");
        }

        co_return;
    }

    void Widget1::HorizontalResizeSupportedCheckBox_Checked(winrt::WF::IInspectable const& /*sender*/, winrt::WUIX::RoutedEventArgs const& /*e*/)
    {
        m_widget.HorizontalResizeSupported(true);
    }

    void Widget1::HorizontalResizeSupportedCheckBox_Unchecked(winrt::WF::IInspectable const& /*sender*/, winrt::WUIX::RoutedEventArgs const& /*e*/)
    {
        m_widget.HorizontalResizeSupported(false);
    }

    void Widget1::VerticalResizeSupportedCheckBox_Checked(winrt::WF::IInspectable const& /*sender*/, winrt::WUIX::RoutedEventArgs const& /*e*/)
    {
        m_widget.VerticalResizeSupported(true);
    }

    void Widget1::VerticalResizeSupportedCheckBox_Unchecked(winrt::WF::IInspectable const& /*sender*/, winrt::WUIX::RoutedEventArgs const& /*e*/)
    {
        m_widget.VerticalResizeSupported(false);
    }

    void Widget1::PinningSupportedCheckBox_Checked(winrt::WF::IInspectable const& /*sender*/, winrt::WUIX::RoutedEventArgs const& /*e*/)
    {
        m_widget.PinningSupported(true);
    }

    void Widget1::PinningSupportedCheckBox_Unchecked(winrt::WF::IInspectable const& /*sender*/, winrt::WUIX::RoutedEventArgs const& /*e*/)
    {
        m_widget.PinningSupported(false);
    }

    void Widget1::SettingsSupportedCheckBox_Checked(winrt::WF::IInspectable const& /*sender*/, winrt::WUIX::RoutedEventArgs const& /*e*/)
    {
        m_widget.SettingsSupported(true);
    }

    void Widget1::SettingsSupportedCheckBox_Unchecked(winrt::WF::IInspectable const& /*sender*/, winrt::WUIX::RoutedEventArgs const& /*e*/)
    {
        m_widget.SettingsSupported(false);
    }

    void Widget1::MinWindowSize_Click(winrt::WF::IInspectable const& /*sender*/, winrt::WUIX::RoutedEventArgs const& /*e*/)
    {
        Windows::Foundation::Size size;
        size.Height = (float)_wtof(MinWindowHeightBox().Text().c_str());
        size.Width = (float)_wtof(MinWindowWidthBox().Text().c_str());
        m_widget.MinWindowSize(size);
    }

    void Widget1::MaxWindowSize_Click(winrt::WF::IInspectable const& /*sender*/, winrt::WUIX::RoutedEventArgs const& /*e*/)
    {
        Windows::Foundation::Size size;
        size.Height = (float)_wtof(MaxWindowHeightBox().Text().c_str());
        size.Width = (float)_wtof(MaxWindowWidthBox().Text().c_str());
        m_widget.MaxWindowSize(size);
    }

    void Widget1::OpacityOverride_Click(winrt::WF::IInspectable const& /*sender*/, winrt::WUIX::RoutedEventArgs const& /*e*/)
    {
        std::wstring_view str{ OpacityOverrideBox().Text() };
        double opacityOverride{ 1 };

        if (!str.empty())
        {
            opacityOverride = _wtof(str.data());
        }

        // - Value cannot be empty
        // - Value cannot be non-numeric (wtof returns 0)
        // - Value must be between 0 and 1.
        if (str.empty()
            || (opacityOverride == 0 && (0 != str.compare(L"0")) && (0 != str.compare(L"0.0")))
            || (opacityOverride < 0)
            || (opacityOverride > 1))
        {
            // Invalid
            OpacityOverrideBox().Text(L"");
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

    winrt::WF::IAsyncAction Widget1::SettingsButton_Click(winrt::WF::IInspectable /*sender*/, winrt::WF::IInspectable /*e*/)
    {
        auto strongThis{ get_strong() };
        co_await m_widget.ActivateSettingsAsync();
    }

    winrt::fire_and_forget Widget1::FavoritedChanged(winrt::WF::IInspectable /*sender*/, winrt::WF::IInspectable /*e*/)
    {
        auto strongThis{ get_strong() };
        co_await resume_foreground(FavoritedTextBlock().Dispatcher());
        FavoritedTextBlock().Text(FavoritedStateToString());
    }

    winrt::fire_and_forget Widget1::GameBarDisplayModeChanged(winrt::WF::IInspectable /*sender*/, winrt::WF::IInspectable /*e*/)
    {
        auto strongThis{ get_strong() };
        co_await resume_foreground(GameBarDisplayModeTextBlock().Dispatcher());
        SetGameBarDisplayMode();
    }

    winrt::fire_and_forget Widget1::PinnedChanged(winrt::WF::IInspectable /*sender*/, winrt::WF::IInspectable /*e*/)
    {
        auto strongThis{ get_strong() };
        co_await resume_foreground(PinnedStateTextBlock().Dispatcher());
        PinnedStateTextBlock().Text(PinnedStateToString());
    }

    winrt::fire_and_forget Widget1::TargetChanged(winrt::WF::IInspectable /*sender*/, winrt::WF::IInspectable /*e*/)
    {
        auto strongThis{ get_strong() };
        co_await resume_foreground(TargetTrackerSettingTextBlock().Dispatcher());
        SetTargetInfo();
    }

    winrt::fire_and_forget Widget1::RequestedOpacityChanged(winrt::WF::IInspectable /*sender*/, winrt::WF::IInspectable /*e*/)
    {
        auto strongThis{ get_strong() };
        if (!m_opacityOverride.has_value())
        {
            co_await resume_foreground(BackgroundGrid().Dispatcher());
            SetRequestedOpacityState();
            SetBackgroundOpacity();
        }
    }

    winrt::fire_and_forget  Widget1::RequestedThemeChanged(winrt::WF::IInspectable /*sender*/, winrt::WF::IInspectable /*e*/)
    {
        auto strongThis{ get_strong() };
        co_await resume_foreground(RequestedThemeTextBlock().Dispatcher());
        RequestedThemeTextBlock().Text(RequestedThemeToString());
        SetBackgroundColor();
    }
    
    winrt::fire_and_forget Widget1::HotkeySetStateChanged(winrt::WF::IInspectable /*sender*/, winrt::MGXI::HotkeySetStateChangedArgs e)
    {
        auto strongThis{ get_strong() };
        co_await resume_foreground(HotkeyPressedTextBlock().Dispatcher());
        HotkeyPressedTextBlock().Text(e.HotkeySetDown ? L"Pressed" : L"Not Pressed");
    }

    winrt::WF::IAsyncAction Widget1::StartHotkeyWatcher_Click(winrt::WF::IInspectable /*sender*/, winrt::WUIX::RoutedEventArgs /*e*/)
    {
        auto strongThis{ get_strong() };

        HotkeyPressedTextBlock().Text(L"Not Pressed");

        co_await winrt::resume_background();
        m_hotkeyWatcher.Start();
    }

    winrt::WF::IAsyncAction Widget1::StopHotkeyWatcher_Click(winrt::WF::IInspectable /*sender*/, winrt::WUIX::RoutedEventArgs /*e*/)
    {
        auto strongThis{ get_strong() };

        HotkeyPressedTextBlock().Text(L"Watcher not running");

        co_await winrt::resume_background();
        m_hotkeyWatcher.Stop();
    }

    winrt::fire_and_forget Widget1::NotificiationSettingChanged(winrt::WF::IInspectable /*sender*/, winrt::WF::IInspectable /*e*/)
    {
        auto strongThis{ get_strong() };
        co_await resume_foreground(NotificiationSettingTextBlock().Dispatcher());

        NotificiationSettingTextBlock().Text(
            m_widgetNotificationManager.Setting() == winrt::MGX::XboxGameBarWidgetNotificationSetting::Enabled ?
            L"Enabled" : L"DisabledByUser");
    }

    void Widget1::VisibleChanged(winrt::WF::IInspectable const& /*sender*/, winrt::WF::IInspectable const& /*e*/)
    {
        OutputVisibleState();
    }

    void Widget1::WindowStateChanged(winrt::WF::IInspectable const& /*sender*/, winrt::WF::IInspectable const& /*e*/)
    {
        OutputWindowState();
    }

    winrt::fire_and_forget Widget1::WindowBoundsChanged(winrt::WF::IInspectable /*sender*/, winrt::WF::IInspectable /*e*/)
    {
        auto strongThis{ get_strong() };
        co_await resume_foreground(WindowBoundsTextBlock().Dispatcher());

        auto bounds{ m_widget.WindowBounds() };
        std::wstringstream strStream;
        strStream << L"H(" << bounds.Height << L") W(" << bounds.Width << L") X(" << bounds.X << L") Y(" << bounds.Y << L")";
        WindowBoundsTextBlock().Text({ strStream.str() });
    }

    winrt::fire_and_forget Widget1::GameBarCompactModeEnabledChanged(winrt::WF::IInspectable /*sender*/, winrt::WF::IInspectable /*e*/)
    {
        auto strongThis{ get_strong() };
        co_await resume_foreground(GameBarCompactModeEnabledTextBlock().Dispatcher());

        SetCompactModeEnabled();
    }

    void Widget1::SetBackgroundColor()
    {
        auto requestedTheme = m_widget.RequestedTheme();

        if (requestedTheme == winrt::WUIX::ElementTheme::Dark)
        {
            RequestedTheme(requestedTheme);
            BackgroundGrid().Background(m_widgetDarkThemeBrush);
        }
        else
        {
            RequestedTheme(requestedTheme);
            BackgroundGrid().Background(m_widgetLightThemeBrush);
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
        case winrt::WUIX::ElementTheme::Light:
            requestedTheme = L"Light";
            break;
        case winrt::WUIX::ElementTheme::Dark:
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

    void Widget1::SetTargetInfo()
    {
        auto settingEnabled{ m_appTargetTracker.Setting() == Microsoft::Gaming::XboxGameBar::XboxGameBarAppTargetSetting::Enabled };

        winrt::MGX::XboxGameBarAppTarget target{ nullptr };
        if (settingEnabled)
        {
            target = m_appTargetTracker.GetTarget();
        }

        TargetTrackerSettingTextBlock().Text(settingEnabled ? L"Enabled" : L"DeniedByUser");
        DisplayNameTextBlock().Text(target ? target.DisplayName() : L"");
        AumIdTextBlock().Text(target ? target.AumId() : L"");
        TitleIdTextBlock().Text(target ? target.TitleId() : L"");
        IsFullscreenTextBlock().Text(target ? winrt::to_hstring(target.IsFullscreen()) : L"");
        IsGameTextBlock().Text(target ? winrt::to_hstring(target.IsGame()) : L"");
    }

    void Widget1::OutputVisibleState()
    {
        hstring isVisible = m_widget.Visible() ? L"true\n" : L"false\n";
        hstring visibleState = L"Visible: \t\t" + isVisible;
        OutputDebugString(visibleState.c_str());
    }

    void Widget1::OutputWindowState()
    {
        hstring window = (m_widget.WindowState() == winrt::MGX::XboxGameBarWidgetWindowState::Minimized) ? L"Minimized\n" : L"Restored\n";
        hstring windowOutput = L"Window State: \t" + window;
        OutputDebugString(windowOutput.c_str());
    }

    void Widget1::SetGameBarDisplayMode()
    {
        hstring mode = (m_widget.GameBarDisplayMode() == winrt::MGX::XboxGameBarDisplayMode::Foreground) ? L"Foreground" : L"PinnedOnly";
        hstring modeOutput = L"Game Bar Display Mode: " + mode + L"\r\n";
        OutputDebugString(modeOutput.c_str());

        GameBarDisplayModeTextBlock().Text(mode);
    }

    void Widget1::SetCompactModeEnabled()
    {
        hstring isEnabled = m_widget.CompactModeEnabled() ? L"True" : L"False";
        hstring modeOutput = L"Game Bar Compact Mode Enabled: " + isEnabled + L"\r\n";
        OutputDebugString(modeOutput.c_str());

        GameBarCompactModeEnabledTextBlock().Text(isEnabled);
    }
}
