#pragma once

#include "Extension2.g.h"

namespace winrt::UIExtSample::implementation
{
    struct Extension2 : Extension2T<Extension2>
    {
        Extension2();

        int32_t MyProperty();
        void MyProperty(int32_t value);

        void MyButton_Click(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args);
    };
}

namespace winrt::UIExtSample::factory_implementation
{
    struct Extension2 : Extension2T<Extension2, implementation::Extension2>
    {
    };
}
