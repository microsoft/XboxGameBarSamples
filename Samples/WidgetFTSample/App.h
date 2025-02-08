#pragma once
#include "App.xaml.g.h"
#include <winrt/Microsoft.Gaming.XboxGameBar.h>

namespace winrt::WidgetFTSample::implementation
{
    struct App : AppT<App>
    {
        App();

        void OnActivated(Windows::ApplicationModel::Activation::IActivatedEventArgs const& args);
        void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs const&);
        void OnSuspending(IInspectable const&, Windows::ApplicationModel::SuspendingEventArgs const&);
        void OnNavigationFailed(IInspectable const&, Windows::UI::Xaml::Navigation::NavigationFailedEventArgs const&);

        void Widget1WindowClosedHandler(IInspectable const&, IInspectable const&);

    private:

        Microsoft::Gaming::XboxGameBar::XboxGameBarWidget m_widget1{ nullptr };
        event_token m_widget1WindowClosedHandlerToken{};
    };
}
