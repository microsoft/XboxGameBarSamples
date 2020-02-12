#pragma once
#include "App.xaml.g.h"
#include <winrt/Microsoft.Gaming.XboxGameBar.h>

namespace winrt::UIExtAdvSample::implementation
{
    struct App : AppT<App>
    {
        App();

        void OnActivated(Windows::ApplicationModel::Activation::IActivatedEventArgs const& args);
        void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs const&);
        void OnSuspending(IInspectable const&, Windows::ApplicationModel::SuspendingEventArgs const&);
        void OnNavigationFailed(IInspectable const&, Windows::UI::Xaml::Navigation::NavigationFailedEventArgs const&);

    private:

        Microsoft::Gaming::XboxGameBar::XboxGameBarUIExtension m_uiExtension1{ nullptr };
        Microsoft::Gaming::XboxGameBar::XboxGameBarUIExtension m_uiExtension1Settings{ nullptr };
        Microsoft::Gaming::XboxGameBar::XboxGameBarUIExtension m_uiExtension2{ nullptr };
    };
}
