#include "pch.h"
#include "Widget1.h"
#include "Widget1.g.cpp"

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::WidgetSample::implementation
{
    Widget1::Widget1()
    {
        InitializeComponent();
    }

    int32_t Widget1::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void Widget1::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    void Widget1::MyButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
        myButton().Content(box_value(L"Clicked"));
    }
}
