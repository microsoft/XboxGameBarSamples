#pragma once

#include "Widget2.g.h"
#include <winrt/Windows.UI.Xaml.Navigation.h>

namespace winrt::WidgetAdvSample::implementation
{
    struct Widget2 : Widget2T<Widget2>
    {
        Widget2();

        virtual void OnNavigatedTo(winrt::Windows::UI::Xaml::Navigation::NavigationEventArgs e);

        int32_t MyProperty();
        void MyProperty(int32_t value);

        void MyButton_Click(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args);
    };
}

namespace winrt::WidgetAdvSample::factory_implementation
{
    struct Widget2 : Widget2T<Widget2, implementation::Widget2>
    {
    };
}
