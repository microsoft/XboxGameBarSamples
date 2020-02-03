#pragma once

#include "Extension2.g.h"
#include <winrt/Windows.UI.Xaml.Navigation.h>

namespace winrt::UIExtAdvSample::implementation
{
    struct Extension2 : Extension2T<Extension2>
    {
        Extension2();

        virtual void OnNavigatedTo(winrt::Windows::UI::Xaml::Navigation::NavigationEventArgs e);

        int32_t MyProperty();
        void MyProperty(int32_t value);

        void MyButton_Click(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args);
    };
}

namespace winrt::UIExtAdvSample::factory_implementation
{
    struct Extension2 : Extension2T<Extension2, implementation::Extension2>
    {
    };
}
