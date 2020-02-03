#pragma once

#include "Extension1.g.h"

namespace winrt::UIExtSample::implementation
{
    struct Extension1 : Extension1T<Extension1>
    {
        Extension1();

        int32_t MyProperty();
        void MyProperty(int32_t value);

        void MyButton_Click(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args);
    };
}

namespace winrt::UIExtSample::factory_implementation
{
    struct Extension1 : Extension1T<Extension1, implementation::Extension1>
    {
    };
}
