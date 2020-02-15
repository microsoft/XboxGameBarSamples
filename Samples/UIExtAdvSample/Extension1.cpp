#include "pch.h"
#include "Extension1.h"
#include "Extension1.g.cpp"

using namespace winrt;
using namespace winrt::Windows::UI::Core;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::UI::Xaml;
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
        m_uiExtensionCoreWindow = Window::Current().CoreWindow();

        // Hook up event that's fired when our settings button is clicked
        m_settingsToken = m_uiExtension.SettingsClicked({ this, &Extension1::SettingsButton_Click });
        m_favoritedChangedToken = m_uiExtension.FavoritedChanged({ this, &Extension1::FavoritedChanged });
        m_displayModeChangedToken = m_uiExtension.GameBarDisplayModeChanged({ this, &Extension1::GameBarDisplayModeChanged });
        m_pinnedChangedToken = m_uiExtension.PinnedChanged({ this, &Extension1::PinnedChanged });
        m_themeChangedToken = m_uiExtension.RequestedThemeChanged({ this, &Extension1::RequestedThemeChanged });
        m_visibleChangedToken = m_uiExtension.VisibleChanged({ this, &Extension1::VisibleChanged });
        m_windowStateChangedToken = m_uiExtension.WindowStateChanged({ this, &Extension1::WindowStateChanged });

        //Favorited Changed
        hstring favoriteState{};
        hstring isFavorited = m_uiExtension.Favorited() ? L"true" : L"false";
        favoriteState = L"Favorited: \t" + isFavorited;
        FavoritedTextBlock().Text(favoriteState);

        //PinnedState intialization
        hstring isPinned = m_uiExtension.Pinned() ? L"\ttrue" : L"\tfalse";
        hstring pinnedOutput = L"Pinned State: " + isPinned;
        PinnedStateTextBlock().Text(pinnedOutput);

        //Requested theme intialization
        
        hstring theme{};
        switch (m_uiExtension.RequestedTheme())
        {
        case ElementTheme::Light:
            theme = L"\tLight";
            break;
        case ElementTheme::Dark:
            theme = L"\tDark";
            break;
        default:
            theme = L"\tDefault";
        }

        hstring requestedTheme = L"Requested Theme: " + theme;

        RequestedThemeTextBlock().Text(requestedTheme);

        //visible
        hstring result = m_uiExtension.Visible() ? L"true" : L"false";
        VisibleChangedTextBlock().Text(result);

        //Window State
        XboxGameBarUIExtensionWindowState windowState = m_uiExtension.WindowState();
        hstring window = windowState == XboxGameBarUIExtensionWindowState::Minimized ? L"WINDOW IS MINIMIZED" : L"WINDOW IS RESTORED"; 
        WindowStateChangedTextBlock().Text(window.c_str());

        //
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
        /*auto result =*/ co_await m_uiExtension.TryResizeWindowAsync(size);
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

        hstring favoriteState{};
        hstring isFavorited = m_uiExtension.Favorited() ? L"true" : L"false";
        favoriteState = L"Favorited: \t" + isFavorited;

        co_await winrt::resume_foreground(FavoritedTextBlock().Dispatcher());

        FavoritedTextBlock().Text(favoriteState);
    }

    void Extension1::GameBarDisplayModeChanged(
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Windows::Foundation::IInspectable const& e)
    {
        auto strongThis{ get_strong() };
        auto value = m_uiExtension.GameBarDisplayMode();
    }

    winrt::fire_and_forget Extension1::PinnedChanged(
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Windows::Foundation::IInspectable const& e) try
    {
        auto strongThis{ get_strong() };
        hstring isPinned = m_uiExtension.Pinned() ? L"\ttrue" : L"\tfalse";
        hstring pinnedOutput = L"Pinned State: " + isPinned;

        //co_await winrt::resume_foreground(m_uiExtensionCoreWindow.Dispatcher());
        co_await winrt::resume_foreground(PinnedStateTextBlock().Dispatcher());

        PinnedStateTextBlock().Text(pinnedOutput);
    }
    catch(...)
    {

    }

    winrt::fire_and_forget  Extension1::RequestedThemeChanged(
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Windows::Foundation::IInspectable const& e)
    {
        auto strongThis{ get_strong() };
        auto value = m_uiExtension.RequestedTheme();

        hstring theme{};
        switch (m_uiExtension.RequestedTheme())
        {
        case ElementTheme::Light:
            theme = L"\tLight";
            break;
        case ElementTheme::Dark:
            theme = L"\tDark";
            break;
        case ElementTheme::Default:
            theme = L"\tDefault";
        }

        hstring requestedTheme = L"Requested Theme: " + theme;


        co_await winrt::resume_foreground(RequestedThemeTextBlock().Dispatcher());

        RequestedThemeTextBlock().Text(requestedTheme);
    }

    winrt::fire_and_forget Extension1::VisibleChanged(
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Windows::Foundation::IInspectable const& e)
    {
        auto strongThis{ get_strong() };
        hstring result = m_uiExtension.Visible() ? L"true" : L"false";

        co_await winrt::resume_foreground(VisibleChangedTextBlock().Dispatcher());

        VisibleChangedTextBlock().Text(result);

        //investigate visibility
        hstring test = L"########################################################################\n";
        OutputDebugString(test.c_str());
        OutputDebugString(result.c_str());
    }

    winrt::fire_and_forget Extension1::WindowStateChanged(
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Windows::Foundation::IInspectable const& e)
    {
        auto strongThis{ get_strong() };
        auto value = m_uiExtension.WindowState();

        XboxGameBarUIExtensionWindowState windowState = m_uiExtension.WindowState();
        hstring window = windowState == XboxGameBarUIExtensionWindowState::Minimized ? L"WINDOW IS MINIMIZED\n" : L"WINDOW IS RESTORED\n";

        co_await winrt::resume_foreground(WindowStateChangedTextBlock().Dispatcher());

        //TODO: investigate bug of WindowState always being XboxGameBarUIExtensionWindowState::Restored

        //hstring test = L"########################################################################\n";
        //OutputDebugString(test.c_str());
        //OutputDebugString(window.c_str());
        WindowStateChangedTextBlock().Text(window.c_str());
    }
}


