#pragma once
#include "App.xaml.g.h"
#include <winrt/Microsoft.Gaming.XboxGameBar.h>


namespace winrt::WidgetSettingsSample::implementation
{
    struct App : AppT<App>
    {
        App();

        void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs const&);
        void OnActivated(Windows::ApplicationModel::Activation::IActivatedEventArgs const& args);
        void OnSuspending(IInspectable const&, Windows::ApplicationModel::SuspendingEventArgs const&);
        void OnNavigationFailed(IInspectable const&, Windows::UI::Xaml::Navigation::NavigationFailedEventArgs const&);

    private:
        Microsoft::Gaming::XboxGameBar::XboxGameBarWidget m_widget{ nullptr };
        Microsoft::Gaming::XboxGameBar::XboxGameBarWidget m_widgetSettings{ nullptr };
    };
}
