#include "pch.h"
#include "Widget1Settings.h"
#if __has_include("Widget1Settings.g.cpp")
#include "Widget1Settings.g.cpp"
#endif

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::WidgetAdvSample::implementation
{
    Widget1Settings::Widget1Settings()
    {
        InitializeComponent();
    }

    void Widget1Settings::ClickHandler(IInspectable const&, RoutedEventArgs const&)
    {
        Button().Content(box_value(L"Clicked"));
    }
}
