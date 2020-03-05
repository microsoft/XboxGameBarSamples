#pragma once

#include "Widget1.g.h"

namespace winrt::WidgetSample::implementation
{
    struct Widget1 : Widget1T<Widget1>
    {
        Widget1();

        int32_t MyProperty();
        void MyProperty(int32_t value);

        void MyButton_Click(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args);
    };
}

namespace winrt::WidgetSample::factory_implementation
{
    struct Widget1 : Widget1T<Widget1, implementation::Widget1>
    {
    };
}
